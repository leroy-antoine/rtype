# Network module (`engine::network`)

The engine moves **messages** (a type and some bytes) over UDP. What a message
means is defined by the game, in [the game protocol](../../docs/protocol.md).

## Wire format

All integers are **big-endian**. A datagram is at most **1200 bytes** and
carries several messages:

```
[magic u16][sequence u16][token u32]  [message][message]...
```

| Field | Role |
|---|---|
| magic | always `0x5254`, anything else is dropped |
| sequence | +1 per datagram; a datagram not newer than the last one received is dropped |
| token | session token given by the server, `0` only during the handshake |

A message:

```
unreliable:  [0|type 7 bits][size varint][payload]
reliable:    [1|type 7 bits][id u16][size varint][payload]
```

The **varint** stores the size on 1 byte below 128, else on 2 bytes: each byte
holds 7 bits of the value, low bits first, and its high bit says "another byte
follows". `12` → `0C`, `300` → `AC 02`. More than 2 bytes, a missing second
byte or a value below 128 written on 2 bytes is invalid.

Sequence numbers and reliable ids are `u16` compared on a circle:
`a` is newer than `b` when `a != b` and `(a - b) mod 65536 < 32768`.

## Message types

| Type | Name | Payload |
|---|---|---|
| `0x00` | Connect | `version u8` (frozen: always the first byte) |
| `0x01` | Accept | `token u32` |
| `0x02` | Refuse | `reason u8`: 0 version, 1 full |
| `0x03` | Ping | `id u16` |
| `0x04` | Pong | `id u16` of the Ping |
| `0x05` | Disconnect | `reason u8`: 0 quit, 1 shutdown, 2 kicked |
| `0x06` | Ack | `id u16` of the reliable message |
| `0x07`–`0x0F` | reserved | |
| `0x10`–`0x7F` | game messages | |

If anything in a datagram is invalid (size, magic, varint, type, engine
payload), the **whole datagram is dropped**.

## Connection

- **Handshake.** The client sends Connect every 500 ms, 10 times at most. The
  server answers Accept with a random token, or Refuse. A Connect from a known
  address gets the same Accept again. Handshake datagrams have token 0 and one
  message only; after it, a datagram must carry the token **and** come from the
  session's address.
- **Ping.** Each side sends a Ping every second and measures the latency with
  the Pong. A peer silent for **5 seconds** is timed out.
- **Disconnect.** The side that leaves sends Disconnect 3 times; the timeout
  covers the case where all of them are lost.

## Reliability

The game chooses for each message whether it is reliable (events sent once:
spawn, death, game over) or not (states sent every tick: positions, inputs).

- The sender gives each reliable message an id and resends it every
  `2 × latency` (at least 100 ms) until an Ack comes back.
- The receiver keeps the next expected id: it applies messages **in order**,
  acknowledges duplicates again without applying them, and keeps up to 64
  early messages aside.
- More than 256 messages waiting for an Ack: the server kicks the client.

## Usage

| Class | Role |
|---|---|
| `BinaryWriter`, `BinaryReader` | write / read the types above; reads return `std::optional` |
| `Packet`, `Message` | one datagram; `serialize()` and `parse()` (all the validation) |
| `ReliableChannel` | ids, pending list, acks, ordering |
| `Connection` | one peer: sequences, ping, timeout, reliability |
| `UdpSocket` | Asio socket and its network thread |
| `Server`, `Client` | handshake and events, used by the game |

The network thread only receives and sends datagrams; everything else runs in
`update()` and `flush()`, on the game thread.

```cpp
Engine::Server server(4242, 4, VERSION);         // port, max clients, version

while (running) {
    server.update();
    while (std::optional<Engine::ServerEvent> event = server.pollEvent()) {
        // ServerEvents::Connected, Disconnected, Message
    }
    server.broadcast(SNAPSHOT, bytes);
    server.send(client, PLAYER_DIED, bytes, Engine::Delivery::Reliable);
    server.flush();
}
```

`Client` works the same way (`update`, `pollEvent`, `send`, `flush`) with the
events `Connected`, `Refused`, `Unreachable`, `Disconnected` and `Message`.
