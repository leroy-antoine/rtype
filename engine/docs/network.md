# Network module (`engine::network`)

This page describes the **engine layer** of the network protocol: the envelope
that every packet uses, how a connection is opened, kept alive and closed, and
how some messages are made reliable on top of UDP.

The engine knows nothing about the game. It moves **messages**: a type number
and some bytes. What a message means (a player input, an entity position, a
game over) is defined by the game, in [the game protocol](../../docs/protocol.md).

| Layer | Owns | Lives in |
|---|---|---|
| Engine (this page) | datagram header, message framing, handshake, ping, timeout, disconnection, reliability, byte encoding | `engine/network` |
| Game | the list of game messages and their payloads, which ones are reliable | `src/` |

- [Overview](#overview)
- [Data types](#data-types)
- [Datagram layout](#datagram-layout)
- [Message types](#message-types)
- [Decoding and validation](#decoding-and-validation)
- [Connection lifecycle](#connection-lifecycle)
- [Liveness: ping and timeout](#liveness-ping-and-timeout)
- [Disconnection](#disconnection)
- [Reliability](#reliability)
- [Sequence numbers](#sequence-numbers)
- [Security](#security)
- [Constants](#constants)
- [Examples](#examples)
- [Design choices](#design-choices)
- [Using the module](#using-the-module)

## Overview

- Transport: **UDP** only, one socket per side.
- Encoding: **binary**, every integer in **big-endian** (network byte order).
- One datagram carries **several messages**, packed one after the other.
- A datagram is at most **1200 bytes**. Above that size IP may fragment it, and
  losing one fragment loses the whole datagram.
- The server is the authority. The engine does not decide anything about the
  game; it only delivers messages.

## Data types

| Type | Size | Encoding |
|---|---|---|
| `u8` | 1 | unsigned |
| `u16` | 2 | unsigned, big-endian |
| `u32` | 4 | unsigned, big-endian |
| `float` | 4 | IEEE 754 single precision, its 32 bits written as a `u32` |
| `varint` | 1 or 2 | unsigned, see below |

### Varint

A varint stores a number on **1 byte when it is small, 2 bytes otherwise**.
In each byte, the high bit is a flag that says "another byte follows", and the
7 other bits carry the number, low bits first.

```
  ┌── 0 = last byte, 1 = another byte follows
  │
  X  X X X X X X X
     └─────┬─────┘
       7 bits of the number
```

| Value | Bytes | Reading |
|---|---|---|
| 12 | `0C` | flag 0: done, value 12 |
| 127 | `7F` | flag 0: done, value 127 |
| 128 | `80 01` | `80`: flag 1, low bits 0; `01`: flag 0, 1 × 128 → 128 |
| 300 | `AC 02` | `AC`: flag 1, low bits 44; `02`: flag 0, 2 × 128 → 300 |

A varint is at most **2 bytes**, so its range is 0 to 16383. The reader
rejects, and the whole datagram is dropped:

| Case | Example | Why |
|---|---|---|
| truncated | `8C` at the end of the datagram | the second byte is missing |
| too long | `8C 81 ...` | the second byte has its flag set: never more than 2 bytes |
| overlong | `8C 00` (12 on two bytes) | a value below 128 must use one byte, so every value has exactly one encoding |

## Datagram layout

```
┌────────────── header (8 bytes) ──────────────┐
│ magic u16 │ sequence u16 │ token u32          │
└───────────────────────────────────────────────┘
┌──────────── message ────────────┐┌── message ──┐
│ type │ [id] │ size │ payload     ││ ...         │   until the end of the datagram
└─────────────────────────────────┘└─────────────┘
```

### Header

| Field | Type | Role |
|---|---|---|
| magic | `u16` | always `0x5254` ("RT"). Anything else is not ours and is dropped |
| sequence | `u16` | number of the datagram, +1 for each datagram a side sends. Drops late and duplicated datagrams, see [Sequence numbers](#sequence-numbers) |
| token | `u32` | session token given by the server. `0` only in handshake datagrams |

The header has **no protocol version**: the version is checked once, at the
handshake (see [Connection lifecycle](#connection-lifecycle)).

The header has **no message count and no total size**: UDP gives the size of
the datagram, and the messages are read until its end.

### Message

```
unreliable:  [0|type 7 bits] [size varint] [payload]
reliable:    [1|type 7 bits] [id u16] [size varint] [payload]
```

| Field | Type | Role |
|---|---|---|
| type | `u8` | bit 7: reliable flag. Bits 0–6: message type (0 to 127) |
| id | `u16` | only when the reliable flag is set: number of the reliable message |
| size | `varint` | number of payload bytes |
| payload | bytes | content, defined by the message type |

## Message types

| Range | Owner |
|---|---|
| `0x00`–`0x06` | engine messages, below |
| `0x07`–`0x0F` | reserved for the engine; receiving one drops the datagram |
| `0x10`–`0x7F` | game messages |

### Engine messages

| Type | Name | Direction | Payload |
|---|---|---|---|
| `0x00` | Connect | client → server | `version u8`, then possibly more bytes (ignored) |
| `0x01` | Accept | server → client | `token u32` (never 0) |
| `0x02` | Refuse | server → client | `reason u8`: 0 = wrong version, 1 = server full |
| `0x03` | Ping | both | `id u16` |
| `0x04` | Pong | both | `id u16`, copied from the Ping |
| `0x05` | Disconnect | both | `reason u8`: 0 = quit, 1 = server shutdown, 2 = kicked |
| `0x06` | Ack | both | `id u16` of the reliable message received |

Engine messages are never reliable. An engine message with the reliable flag,
or with a payload of the wrong size or an unknown reason, drops the datagram.

The **Connect message is frozen**: its type stays `0x00` and the version stays
its first byte in every future version of the protocol. This way any server can
read the version of any client and refuse it cleanly.

## Decoding and validation

A datagram is read with a cursor that moves forward through the bytes. Before
each read, the reader checks that enough bytes remain, so it never reads past
the end of the buffer.

Decoding is done in **two passes**:

1. decode every message of the datagram into a list, without acting on any;
2. only if all of them are valid, apply them in order.

If anything is invalid, the **whole datagram is dropped** and nothing is
applied. An honest peer never sends a malformed datagram (UDP already drops
corrupted ones with its checksum), so a malformed datagram comes from a bug or
an attacker and nothing in it is worth keeping. The two passes also guarantee
that the state is never half updated.

A datagram is dropped when:

| Check | Example |
|---|---|
| larger than 1200 bytes | a 1500 bytes datagram |
| shorter than the header, or no message after it | 5 bytes |
| wrong magic | `47 45 54 20` (an HTTP request) |
| unknown or reserved type | type `0x09` |
| reliable flag on an engine message | `0x83` (reliable Ping) |
| truncated id, size or payload | size 300 with 50 bytes left |
| invalid varint | see [Varint](#varint) |
| engine payload of the wrong size or value | Ping with 3 bytes, Refuse reason 7 |
| handshake rule broken | see below |
| unknown token, or token of another address | a forged packet |
| sequence not newer than the last one received | a late or duplicated datagram |

## Connection lifecycle

### Handshake

```
client                                   server
  │  Connect { version }   token = 0       │
  │ ─────────────────────────────────────► │  known address?   → same Accept again
  │                                         │  wrong version?   → Refuse(0)
  │                                         │  server full?     → Refuse(1)
  │  Accept { token }      token = 0       │  otherwise: new session, new token
  │ ◄───────────────────────────────────── │
  │                                         │
  │  game datagrams        token = T       │
  │ ◄────────────────────────────────────► │
```

Rules:

- **token = 0 ⇔ handshake.** A datagram with token 0 holds exactly one message:
  Connect (to the server), Accept or Refuse (to the client). A datagram with
  another token never holds Connect, Accept or Refuse.
- Before the handshake a client is identified by its **address (IP:port)**. It
  keeps the same socket, so the same port, for all its attempts.
- After the handshake a datagram is accepted only if its **token** belongs to a
  session **and** comes from that session's address.
- The token is a random `u32`, never 0, unique among the sessions. It is a
  secret: someone who forges the address of a player cannot guess it.
- "Server full" means the number of sessions reached the maximum given by the
  game when it created the server.

### Lost or duplicated handshake

- The client sends Connect again every **500 ms**, at most **10 times**. After
  that it gives up: the server is unreachable.
- If the Accept is lost, the next Connect comes from an address the server
  already knows. The server sends **the same Accept with the same token** and
  creates nothing. Receiving a Connect once or ten times gives the same result
  (the handshake is *idempotent*).
- If the first Accept was only late, the client receives two. Once connected,
  it ignores any other Accept or Refuse.

## Liveness: ping and timeout

Each side sends a **Ping every second**. The other side answers at once with a
**Pong** carrying the same id.

```
A → B :  Ping { id = 41 }      A keeps (41, sent at t = 1000 ms)
B → A :  Pong { id = 41 }      received at t = 1048 ms → RTT = 48 ms
```

- The sender keeps only its **last** Ping (id and time). A Pong with another
  id (late, or invented) is ignored.
- The send time stays on the sender's side and never travels. A peer cannot
  fake its own latency by editing a time in the Pong.
- The measured round trip time (RTT) is used to time the resends of reliable
  messages, and the game can read it (to display it, for example).

Every valid datagram received updates the **time of the last datagram** of
that peer. A peer silent for more than **5 seconds** is considered dead. Pings
guarantee traffic even when the game sends nothing (in a menu, for example),
so a living peer is never silent that long.

When the server times a client out, it deletes the session and tells the game,
which can then remove the player and tell the other clients. When the client
times the server out, it reports the connection as lost.

## Disconnection

The side that leaves sends **Disconnect 3 times**, in 3 datagrams, then stops.
It does not wait for an answer.

- If one Disconnect arrives, the other side deletes the session at once.
- The next copies carry a token that no longer exists, so they are dropped:
  duplicates are harmless.
- If all 3 are lost, the timeout does the same job 5 seconds later.

| Reason | Sent by | When |
|---|---|---|
| 0 quit | client | the player leaves |
| 1 shutdown | server | the server stops |
| 2 kicked | server | the game kicks the player, or the client stopped acknowledging reliable messages |

## Reliability

UDP may lose, duplicate or reorder datagrams. For most game traffic this does
not matter: a position sent every tick is replaced by the next one. A message
that happens **once** (an entity spawns, a player dies, the game ends) must
arrive. The game chooses, for each message it sends, whether it is reliable.
The engine provides the mechanism without knowing the content.

| Usually unreliable | Usually reliable |
|---|---|
| states sent again and again: positions, inputs | events sent once: spawn, destroy, death, game over |

### Sending

- Each reliable message gets an **id**: 0, 1, 2, ... (a `u16` that wraps).
- The sender keeps it in a **pending** list until the receiver acknowledges it.
- If no Ack arrived after **2 × RTT** (at least **100 ms**), the message is sent
  again in the next datagram.
- An Ack removes the message from the pending list.

### Receiving

The receiver keeps one number: the **next expected id**.

| Received id | Expected | Action |
|---|---|---|
| 7 | 7 | apply it, send Ack 7, expect 8 |
| 7 (again) | 8 | older than expected: already applied, do not apply, **send Ack 7 again** |
| 9 | 8 | early: keep it aside, send Ack 9 |
| 8 | 8 | apply 8, then 9 kept aside, expect 10 |

- Reliable messages are applied **in the order they were sent**. A spawn sent
  before a destroy is always applied before it, even if UDP swapped them.
- A duplicate is acknowledged again: its first Ack may have been lost, and
  without a new one the sender would resend it forever.
- An Ack is never acknowledged. If it is lost, the message is resent and
  acknowledged again.
- Only one integer is stored per peer to detect duplicates, whatever the length
  of the game.

### Limits

| Limit | Value | When it is reached |
|---|---|---|
| messages kept aside (received early) | 64 ids ahead of the expected one | a message further ahead is dropped **without** Ack; the sender will resend it later |
| pending messages (sent, not acknowledged) | 256 per peer | the peer does not acknowledge: the server kicks it, the client reports the connection as lost |

These limits bound the memory one peer can make the other use.

## Sequence numbers

The sequence number of a datagram is a `u16`: at 60 datagrams per second it
goes back to 0 after about 18 minutes. A plain `a > b` would then call every
new datagram "old". Sequence numbers are compared on a circle instead: `a` is
newer than `b` when it is less than half a turn ahead of it.

```
a is newer than b  ⇔  a ≠ b  and  (a − b) mod 65536 < 32768
```

| a | b | (a − b) mod 65536 | a newer? |
|---|---|---|---|
| 10 | 5 | 5 | yes |
| 5 | 10 | 65531 | no |
| 2 | 65530 | 8 | yes (wrapped) |

The same comparison is used for reliable message ids. A receiver drops every
datagram whose sequence is not newer than the last one it accepted from that
peer.

## Security

| Threat | Defense |
|---|---|
| random traffic on the port | magic, full validation, the datagram is dropped |
| buffer overflow with a lying size | every size is compared with the bytes really received |
| datagram of a huge size | 1200 bytes maximum, extra bytes are never read |
| forging the address of a player | the token is secret and random |
| replaying an old datagram | the sequence must be newer than the last one |
| filling the memory | bounded queue of received datagrams, 64 early messages, 256 pending messages |
| endless Connects | a known address gets the same session; at most the maximum number of sessions |
| a client that never acknowledges | kicked when its pending list is full |

## Constants

| Constant | Value |
|---|---|
| magic | `0x5254` |
| maximum datagram size | 1200 bytes |
| header size | 8 bytes |
| maximum payload of one message | 1187 bytes (1200 − 8 header − 1 type − 2 id − 2 size) |
| Connect retry | every 500 ms, 10 attempts |
| Ping interval | 1 s |
| timeout | 5 s without a valid datagram |
| Disconnect copies | 3 |
| reliable resend delay | 2 × RTT, at least 100 ms |
| early messages kept | 64 |
| pending messages | 256 |

## Examples

### Connect (client → server, 11 bytes)

```
52 54 | 00 00 | 00 00 00 00 | 00 | 01 | 01
magic   seq 0   token 0       Connect, size 1, version 1
```

### Accept (server → client, 14 bytes)

```
52 54 | 00 00 | 00 00 00 00 | 01 | 04 | 8F 3A 21 C4
magic   seq 0   token 0       Accept, size 4, token given to the client
```

### A game datagram (client → server, 19 bytes)

```
52 54 | 00 2A | 8F 3A 21 C4 | 03 02 00 05 | 10 01 05 | 06 02 00 07
magic   seq 42  token          Ping id 5     game 0x10   Ack id 7
                                             size 1
                                             payload 05
```

### A reliable game message (server → client)

```
93 | 00 07 | 02 | 00 02
│     │      │    └── payload (2 bytes)
│     │      └── size 2
│     └── reliable id 7
└── 1 0010011: reliable flag + type 0x13
```

## Design choices

| Topic | Choice | Alternatives | Why |
|---|---|---|---|
| messages per datagram | **several** | one message per datagram | each datagram costs 28 bytes of IP and UDP headers; packing a tick into one datagram divides the traffic by about 3 |
| message size field | **varint** (1 or 2 bytes) | `u8`, `u16` | almost every message is below 128 bytes and costs 1 byte, larger ones are still possible |
| splitting large messages | **never**, the game sends several self-contained messages | fragmentation and reassembly | reassembly needs buffers an attacker can fill, and a lost piece makes the others useless |
| sequence number | **`u16`** with a circular comparison | `u32` | 2 bytes less in every datagram; the comparison is a one-liner |
| protocol version | **checked once in Connect** | a version field in every header | the token proves the handshake was passed, so a version in each datagram would cost a byte for nothing |
| session identification | **random token** + address | address only | an address can be forged, a 32 bits random secret cannot be guessed |
| liveness | **ping every second + timeout** | heartbeat only | the same traffic also measures the latency, needed for resend delays and later for lag compensation |
| ping timestamp | **kept by the sender** | sent and echoed | an echoed time could be edited by the peer to fake its latency |
| reliable flag | **high bit of the type** | a separate byte, an id on every message | free for unreliable messages, 2 bytes for reliable ones; 112 game types are enough |
| ordering of reliable messages | **in order**, next expected id | apply on arrival | a destroy must never be applied before the spawn it refers to |
| malformed datagram | **dropped as a whole**, two passes | skip the bad message | honest peers never send one; the state is never half applied |

## Using the module

### Classes

| Class | Header | Role |
|---|---|---|
| `BinaryWriter`, `BinaryReader` | `BinaryWriter.hpp`, `BinaryReader.hpp` | write and read the [data types](#data-types); every read is bounds-checked |
| `Message` | `Message.hpp` | one message: type, reliable flag, id, payload |
| `Packet` | `Packet.hpp` | one datagram: header and messages; `serialize()` and `parse()`, where all the [validation](#decoding-and-validation) happens |
| `ReliableChannel` | `ReliableChannel.hpp` | [reliability](#reliability): ids, pending list, acks, ordering |
| `Connection` | `Connection.hpp` | one remote peer: sequences, ping, timeout, reliable channel; turns messages into packets |
| `UdpSocket` | `UdpSocket.hpp` | the Asio socket and the network thread (Asio is hidden behind a pimpl) |
| `Server`, `Client` | `Server.hpp`, `Client.hpp` | handshake, sessions, events: the classes the game uses |
| constants, enums | `Protocol.hpp`, `Event.hpp`, `Address.hpp` | protocol constants, `Delivery`, reasons, events, IPv4 address |

### Threads

Each `Server` and `Client` starts **one network thread** that only receives
and sends datagrams. Every protocol decision (validation, handshake, ping,
reliability, timeout) runs on the thread that calls `update()` and `flush()`,
usually the game thread. The game never waits for the network: `update()` only
reads what the network thread already received.

A `Server` or a `Client` must be used from one thread only.

### Server

```cpp
#include "engine/network/Server.hpp"

Engine::Server server(4242, 4, PROTOCOL_VERSION);   // port, max clients, version

while (running) {
    server.update();                                  // receive, ping, resend, timeouts
    while (std::optional<Engine::ServerEvent> event = server.pollEvent()) {
        // ServerEvents::Connected { client }
        // ServerEvents::Disconnected { client, reason }
        // ServerEvents::Message { client, type, payload }
    }
    // ... game tick ...
    server.broadcast(SNAPSHOT, bytes);                        // unreliable by default
    server.send(client, PLAYER_DIED, bytes, Engine::Delivery::Reliable);
    server.flush();                                   // pack and send the datagrams
}
```

### Client

```cpp
#include "engine/network/Client.hpp"

Engine::Client client("127.0.0.1", 4242, PROTOCOL_VERSION);

while (running) {
    client.update();
    while (std::optional<Engine::ClientEvent> event = client.pollEvent()) {
        // ClientEvents::Connected, Refused { reason }, Unreachable,
        // Disconnected { reason }, Message { type, payload }
    }
    if (client.connected()) {
        client.send(INPUT, bytes);
    }
    client.flush();
}
```

### Encoding payloads

`BinaryWriter` and `BinaryReader` write and read the [data types](#data-types) in
big-endian. Payloads are `std::vector<std::uint8_t>`. Every read returns a
`std::optional`, empty when not enough bytes remain, so a payload is decoded
without ever reading out of bounds:

```cpp
Engine::BinaryWriter writer;

writer.writeU32(entity);
writer.writeFloat(x);
writer.writeFloat(y);
server.broadcast(ENTITY_UPDATE, writer.data());
```

```cpp
Engine::BinaryReader reader(payload);
std::optional<std::uint32_t> entity = reader.readU32();
std::optional<float> x = reader.readFloat();
std::optional<float> y = reader.readFloat();

if (!entity || !x || !y || reader.remaining() != 0) {
    return;                                           // malformed: ignore it
}
```

### Errors

| Situation | Result |
|---|---|
| game message type outside `0x10`–`0x7F` | `std::invalid_argument` |
| payload larger than 1187 bytes | `std::invalid_argument` |
| `Client::send` while not connected | `std::logic_error` |
| `Server::send` to a client that is gone | ignored (it may have left during the tick) |
| port already in use, unknown host | `std::system_error` |
