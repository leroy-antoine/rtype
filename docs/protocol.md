# Network protocol

The protocol is **binary** and runs over **UDP** only. It has two layers:

1. the **transport layer** (`engine/network`): datagrams, sessions, ping,
   reliability. It does not know R-Type. Its full description, with the
   validation rules, is in [the network module](../engine/docs/network.md);
2. the **game layer** (`src/include/rtype/Protocol.hpp`): the messages of
   R-Type, described on this page.

A new client can be written from these two pages alone.

- [Overview](#overview)
- [Data types](#data-types)
- [Transport summary](#transport-summary)
- [Connection lifecycle](#connection-lifecycle)
- [Game messages](#game-messages)
- [Reliability](#reliability)
- [Error handling and security](#error-handling-and-security)
- [Example](#example)

## Overview

| Topic | Value |
|---|---|
| Transport | UDP, IPv4, one server port chosen on the command line |
| Direction | the client sends keys, the server sends the world |
| Datagram | at most **1200 bytes**, carries one or several messages |
| Byte order | **big-endian** for every integer and float |
| Protocol version | `18` (`rtype::protocol::VERSION`), checked at connection |
| Tick | the server simulates at 60 Hz and sends the states every tick |

The server is the authority. A client never sends a position, a hit or a
damage: only the state of its keys.

## Data types

| Type | Size | Encoding |
|---|---|---|
| `u8`, `u16`, `u32` | 1, 2, 4 | unsigned, big-endian |
| `float` | 4 | IEEE 754 binary32, sent as a big-endian `u32`; a `NaN` or an infinity is rejected |
| `varint` | 1 or 2 | size of a payload, see the network module |
| `bool` | 1 | `0` or `1`, any other value is rejected |
| `Vec2` | 8 | `x float`, `y float` |

Coordinates are in **map units**: the map is 1920 x 1080, the origin is the
top-left corner, `x` grows to the right and `y` grows downwards.

## Transport summary

```
[magic u16 = 0x5254][sequence u16][token u32]  [message] [message] ...
```

Each message has a type (7 bits), a reliable flag, a payload size and the
payload. Types `0x00`-`0x06` belong to the transport (Connect, Accept,
Refuse, Ping, Pong, Disconnect, Ack), `0x07`-`0x0F` are reserved and
`0x10`-`0x7F` are the game messages below. See
[the network module](../engine/docs/network.md#wire-format).

## Connection lifecycle

```
 client                                   server
   │ ── Connect(version=12) ─────────────►  │  every 500 ms, 10 tries at most
   │ ◄───────────── Accept(token) ──────── │  or Refuse(0 version, 1 full)
   │ ◄───────────────────── YOU(slot) ──── │  reliable
   │ ◄────────────────── SPAWN × n ─────── │  reliable, the whole world
   │ ◄──────────────────── LEADERBOARD ─── │  reliable, the best times
   │ ── SELECT_MAP(map) ────────────────►  │  reliable, the host, in the lobby
   │ ── INPUT(sequence, keys) ──────────►  │  every frame of the client
   │ ◄─────────────── STATES, STATUS ───── │  every tick / every 6 ticks
   │ ◄──────────────────── OWN_STATE ───── │  every tick, to this client only
   │ ◄─────────────── SPAWN, DESTROY ───── │  reliable, when something happens
   │ ── Disconnect ─────────────────────►  │
```

- A client is refused when the version differs (`reason 0`) or when the
  server has no free slot (`reason 1`). The server also disconnects a client
  right after accepting it if the game has no free player slot.
- Both sides send a Ping every second. A peer silent for **5 seconds** is
  timed out and removed.
- When a client leaves or times out, its ship is destroyed in the game. The
  server announces it to **all the other clients** with a `DESTROY` message,
  so the others see the ship disappear. The server keeps running.
- When the last client leaves, the game goes back to the lobby.

## Game messages

Sender: **C** is the client, **S** is the server. Delivery: **R** is reliable
(resent until acknowledged, applied in order), **U** is unreliable.

| Type | Name | From | Delivery | Payload size |
|---|---|---|---|---|
| `0x10` | `INPUT` | C | U | 5 |
| `0x11` | `RESPAWN` | C | R | 0 |
| `0x12` | `SELECT_MAP` | C | R | 1 |
| `0x13` | `LEAVE_MATCH` | C | R | 0 |
| `0x14` | `PAUSE` | C | R | 1 |
| `0x20` | `SPAWN` | S | R | 22 |
| `0x21` | `DESTROY` | S | R | 4 |
| `0x22` | `STATES` | S | U | 5 + 12 × n |
| `0x23` | `STATUS` | S | U | 16 + 4 × n |
| `0x24` | `YOU` | S | R | 1 |
| `0x25` | `OWN_STATE` | S | U | 14 |
| `0x26` | `LEADERBOARD` | S | R | 3 to 28 |

### `INPUT` (0x10)

The **state** of the keys, not an event: "right is held, fire is held". The
server keeps it until the next one. The client sends it every frame.

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | sequence | `u32` | number of this input, grows by one at each frame |
| 4 | keys | `u8` | bit 0 up, bit 1 down, bit 2 left, bit 3 right, bit 4 fire |

Bits 5 to 7 of `keys` must be `0`, otherwise the message is ignored. The
server remembers the last `sequence` it applied and sends it back in
`OWN_STATE`, so the client can predict its own ship.

### `RESPAWN` (0x11)

No payload. Asks the server to bring the player back after its death. The
server accepts it only while the game is running and the player is dead.

### `LEAVE_MATCH` (0x13)

No payload. Sent by the host to abandon the running match and return to the
lobby. Used from the pause menu. The server ignores it when the sender is not the host.

### `PAUSE` (0x14)

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | paused | `u8` | 1 to pause the match, 0 to resume it |

Sent by the host. While paused, the server stops the simulation and the
clock, and sets bit 7 of the `phase` of `STATUS`. Ignored when the sender is
not the host or when the game is not running.

### `SELECT_MAP` (0x12)

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | map | `u8` | index of the map, 0 to 2 |

Sent by the host to leave the lobby and start the match on this map. The
**host** is the player with the lowest slot among those connected. The server
ignores the message when the sender is not the host, when the game is not in
the lobby phase, or when the map is unknown.

### `SPAWN` (0x20)

An entity appeared. The server sends one `SPAWN` for every entity of the world
to a client that has just connected, then one for each new entity.

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | id | `u32` | identifier of the entity, unique during the match |
| 4 | sprite | `u8` | kind of entity, see below |
| 5 | slot | `u8` | player slot 0 to 3 (meaningful for players) |
| 6 | position | `Vec2` | top-left corner |
| 14 | size | `Vec2` | width and height of the hit box |

| `sprite` | Kind |
|---|---|
| 0 | player |
| 1 | monster |
| 2 | player bullet |
| 3 | monster bullet |
| 4 | explosion |
| 5 | boss |
| 6 | coin |
| 7 | tank |
| 8 | kamikaze |
| 9 | elite |
| 10 | rocket |
| 11 | bomber |
| 12 | dasher boss |
| 13 | summoner boss |
| 14 | fortress boss |
| 15 | rocket missile |
| 16 | bomb |
| 17 | alien shard |
| 18 | rock wall |
| 19 | spike wall |
| 20 | reverse portal |
| 21 | fast portal |
| 22 | normal portal |
| 23 | laser post |
| 24 | turret |
| 25 | mine |
| 26 | laser beam (deadly) |
| 27 | laser warning |
| 28 | meteor |
| 29 | Leviathan bolt |

A value of 30 or more is invalid, and so is a player slot of 4 or more.

### `DESTROY` (0x21)

| Offset | Field | Type |
|---|---|---|
| 0 | id | `u32` |

### `STATES` (0x22)

The positions of the moving entities, sent every tick.

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | tick | `u32` | server tick of this snapshot, used by the client to interpolate |
| 4 | count `n` | `u8` | |
| 5 + 12 × i | id | `u32` | |
| 9 + 12 × i | position | `Vec2` | |

The payload size must be exactly `5 + 12 × n`. At most 98 entities fit in one
message, so the server splits a large world into several `STATES` messages.
A state for an unknown id is ignored by the client.

### `STATUS` (0x23)

What the HUD shows, sent about 10 times per second.

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | phase | `u8` | bits 0-6: 0 playing, 1 victory, 2 defeat, 3 lobby; bit 7 (`0x80`): the match is paused |
| 1 | boss hp | `u16` | `0` when there is no boss |
| 3 | boss max hp | `u16` | the boss hp cannot exceed it |
| 5 | coins | `u32` | the shared purse of the players |
| 9 | elapsed time | `u32` | milliseconds since the start of the match, frozen when it ends |
| 13 | level | `u8` | number of the level being played, starting at 1 |
| 14 | map | `u8` | index of the map being played (or last played), 0 to 2 |
| 15 | deaths | `u16` | number of times a player was destroyed in this match |
| 17 | count `n` | `u8` | number of players, at most 4 |
| 18 + 4 × i | slot | `u8` | 0 to 3 |
| 19 + 4 × i | hp | `u16` | |
| 21 + 4 × i | alive | `u8` | `0` or `1` |

The server is in the **lobby** phase when it starts, and again after the end
screen of a victory (or when every player has left). After a defeat, the same map restarts from its first level. The players are listed in
increasing slot order, so the first one is the host.

### `YOU` (0x24)

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | slot | `u8` | the slot of this client, 0 to 3 |

The client uses it to know which ship is its own (HUD, respawn hint).

### `OWN_STATE` (0x25)

The authoritative position of the ship of one client, sent every tick to that
client only.

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | last input | `u32` | `sequence` of the last `INPUT` the server applied |
| 4 | position | `Vec2` | top-left corner of the ship |
| 12 | flags | `u8` | bit 0: the keys are inverted (a reverse portal was crossed); other bits must be `0` |
| 13 | speed | `u8` | speed of the ship in percent of the normal speed (`100` normal, `150` after a fast portal), never `0` |

The flags and the speed are the effects of the portals on this ship. The
client needs them to predict the movement with the same rules as the server.

The client replays the inputs sent after `last input` on top of this position
(prediction), so its ship answers at once and is corrected if the server
disagrees. It is only sent once the server has received an `INPUT` from this
client.

### `LEADERBOARD` (0x26)

The best victory times of every map, sent to a client when it connects and to
everybody after each victory. Three maps, so three blocks one after the other:

| Field | Type | Meaning |
|---|---|---|
| count `n` | `u8` | records of this map, at most 5 |
| time (× n) | `u32` | milliseconds, fastest first |
| players (× n) | `u8` | number of players of that run, 1 to 4 |

Each record takes 5 bytes (time then players). A payload with trailing bytes,
a count above 5, or `0` players is rejected.

## Reliability

UDP can lose, duplicate and reorder datagrams. The protocol picks the delivery
per message:

- **Reliable** (`SPAWN`, `DESTROY`, `YOU`, `RESPAWN`, `SELECT_MAP`, `LEAVE_MATCH`, `PAUSE`, `LEADERBOARD`): a spawn or a death must
  not be missed, or the client would draw a wrong world. The sender resends
  until an Ack comes back, the receiver applies the messages in order.
- **Unreliable** (`INPUT`, `STATES`, `STATUS`, `OWN_STATE`): they are repeated every tick
  or every frame, so a lost one is replaced by the next. Resending an old
  position would be useless. A datagram that is not newer than the last one
  received is dropped, so a late `STATES` never moves a ship backwards.

## Error handling and security

- A datagram with a wrong magic, size, varint, type or engine payload is
  **dropped entirely**.
- After the handshake, a datagram must carry the session token **and** come
  from the address of the session.
- Every game payload is decoded with a bounds-checked reader. The size must
  match exactly, a field out of range (`sprite`, `slot`, `alive`, keys bits,
  a boss hp above its maximum, a map that does not exist) or a non-finite float makes the message be
  ignored. A malformed message never reaches the game.
- Bounds against memory abuse: 1200 bytes per datagram, 4096 datagrams queued
  by the socket, 64 early reliable messages kept, and a client with more than
  256 messages waiting for an Ack is kicked.
- The server ignores any game message from an address that has no session.
- A client can only choose the state of its five keys. It cannot move, hit or
  spawn anything by itself.

## Example

A client holds *right* and *fire* (keys `0b11000` = `0x18`) at its input
number 42. Inside a datagram, the unreliable message `INPUT` is:

```
10 05 00 00 00 2A 18
│  │  │           └─ keys: right (bit 3) + fire (bit 4)
│  │  └───────────── sequence 42
│  └──────────────── payload size: 5 (varint)
└─────────────────── type 0x10, reliable flag 0
```

The server announces a monster (`id 7`, kind 1, slot 0, at x = 2020, y = 300,
box 80 x 80) with a reliable `SPAWN`. The message is split in fields below:

| Bytes | Field |
|---|---|
| `A0` | type `0x20` with the reliable bit set |
| `00 05` | reliable id 5, used by the Ack |
| `16` | payload size 22 (varint) |
| `00 00 00 07` | entity id 7 |
| `01` | sprite 1, a monster |
| `00` | slot 0 |
| `44 FC 80 00` | x = 2020.0 |
| `43 96 00 00` | y = 300.0 |
| `42 A0 00 00` | box width 80.0 |
| `42 A0 00 00` | box height 80.0 |
