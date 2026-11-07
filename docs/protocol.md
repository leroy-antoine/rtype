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
- [Rooms](#rooms)
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
| Protocol version | `26` (`rtype::protocol::VERSION`), checked at connection |
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
   │ ── Connect(version=26) ─────────────►  │  every 500 ms, 10 tries at most
   │ ◄───────────── Accept(token) ──────── │  or Refuse(0 version, 1 full)
   │ ◄──────────────────── ROOM_LIST ───── │  reliable, the client is in the lobby
   │ ── ROOM_CREATE / ROOM_JOIN(room) ──►  │  reliable
   │ ◄─────── ROOM_JOINED / ROOM_REFUSED ─ │  reliable
   │ ◄───────────────────── YOU(slot) ──── │  reliable
   │ ◄────────────────── SPAWN × n ─────── │  reliable, the whole world
   │ ◄──────────────────── LEADERBOARD ─── │  reliable, the best times
   │ ── HISTORY_REQUEST ────────────────►  │  reliable, to open the leaderboard tab
   │ ◄───────────────────── HISTORY × n ── │  reliable, every win, in pages
   │ ── SETTINGS(difficulty, fire) ─────►  │  reliable, the host, in the lobby
   │ ── SELECT_MAP(map) ────────────────►  │  reliable, the host, in the lobby
   │ ◄───────────────── AUGMENTS, SHOP ─── │  reliable, augments owned and on sale
   │ ── INPUT(sequence, keys) ──────────►  │  every frame of the client
   │ ◄─────────────── STATES, STATUS ───── │  every tick / every 6 ticks
   │ ◄──────────────────── OWN_STATE ───── │  every tick, to this client only
   │ ◄─────────────── SPAWN, DESTROY ───── │  reliable, when something happens
   │ ◄─────────────── AUGMENTS, SHOP ───── │  reliable, when they change
   │ ── BUY(id) ────────────────────────►  │  reliable, when the player buys
   │ ◄─────────────────── BUY_RESULT ───── │  reliable, to the buyer only
   │ ── Disconnect ─────────────────────►  │
```

- A client is refused when the version differs (`reason 0`) or when the
  server is full (`reason 1`, 32 clients). Everything after `ROOM_JOINED`
  happens inside a room (see [Rooms](#rooms)).
- Both sides send a Ping every second. A peer silent for **5 seconds** is
  timed out and removed.
- When a client leaves or times out, its ship is destroyed in the game. The
  server announces it to **the other clients of its room** with a `DESTROY`
  message, so they see the ship disappear. The server keeps running.
- When the last client of a room leaves, the room is closed.

## Rooms

One server runs up to **8 rooms** (`MAX_ROOMS`) of 4 players each. Every room
has its own game and its own thread. A client that is not in a room is in the
**lobby**: it only receives `ROOM_LIST`, and the server ignores its game
messages. The game messages of a client in a room are routed to that room
only.

| Type | Name | From | Delivery | Payload size |
|---|---|---|---|---|
| `0x31` | `ROOM_CREATE` | C | R | 0 |
| `0x32` | `ROOM_JOIN` | C | R | 2 |
| `0x33` | `ROOM_LEAVE` | C | R | 0 |
| `0x38` | `ROOM_LIST` | S | R | 1 + 4 × n |
| `0x39` | `ROOM_JOINED` | S | R | 2 |
| `0x3A` | `ROOM_REFUSED` | S | R | 1 |

- `ROOM_CREATE` opens a new room and puts the client in it, as its host.
- `ROOM_JOIN` (`room u16`) joins a room, even when its match has started: the
  player gets a free slot and the whole world right away.
- `ROOM_LEAVE` takes the client back to the lobby. When the host leaves, the
  player with the lowest slot becomes the host. An empty room is closed.
- `ROOM_LIST` is sent when a client enters the lobby, then again to every
  client in the lobby each time a room changes.
- `ROOM_JOINED` (`room u16`) is followed by `YOU`, the world, `AUGMENTS`,
  `SHOP` and `LEADERBOARD`. The waiting room is the `Lobby` phase of the game:
  the host picks the map with `SELECT_MAP`.

The leaderboards are shared by every room of the server.

### `ROOM_LIST` (0x38)

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | count | `u8` | number of rooms, at most 8 |
| 1 + 4i | id | `u16` | room id |
| 3 + 4i | players | `u8` | players in the room, at most 4 |
| 4 + 4i | phase | `u8` | `Phase` of its game, as in `STATUS` |

The size must be exactly `1 + 4 × count`.

### `ROOM_REFUSED` (0x3A)

| Value | Reason |
|---|---|
| 0 | `NotFound`: no room with this id |
| 1 | `Full`: the room has 4 players |
| 2 | `TooManyRooms`: the server already runs 8 rooms |
| 3 | `AlreadyInRoom`: the client must leave its room first |

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
| `0x15` | `BUY` | C | R | 1 |
| `0x16` | `READY` | C | R | 1 |
| `0x17` | `SETTINGS` | C | R | 2 |
| `0x18` | `HISTORY_REQUEST` | C | R | 0 |
| `0x19` | `MAP_UPLOAD_BEGIN` | C | R | 4 |
| `0x1A` | `MAP_UPLOAD_CHUNK` | C | R | 1 to 1100 |
| `0x1B` | `MAP_UPLOAD_END` | C | R | 0 |
| `0x1C` | `CUSTOM_RECORDS_GET` | C | R | 8 |
| `0x20` | `SPAWN` | S | R | 22 |
| `0x21` | `DESTROY` | S | R | 4 |
| `0x22` | `STATES` | S | U | 5 + 12 × n |
| `0x23` | `STATUS` | S | U | 22 + 6 × n |
| `0x24` | `YOU` | S | R | 1 |
| `0x25` | `OWN_STATE` | S | U | 14 |
| `0x26` | `AUGMENTS` | S | R | 1 + Σ (2 + n) |
| `0x27` | `SHOP` | S | R | 1 + n |
| `0x28` | `BUY_RESULT` | S | R | 2 |
| `0x29` | `LEADERBOARD` | S | R | 12 to 228 |
| `0x2A` | `HISTORY` | S | R | 5 + 7 × n |
| `0x2B` | `CUSTOM_INFO` | S | R | 10 + name + n |
| `0x2C` | `MAP_UPLOAD_RESULT` | S | R | 2 + text |
| `0x2D` | `CUSTOM_RECORDS` | S | R | 12 + 6 × n |

### `INPUT` (0x10)

The **state** of the keys, not an event: "right is held, fire is held". The
server keeps it until the next one. The client sends it every frame.

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | sequence | `u32` | number of this input, grows by one at each frame |
| 4 | keys | `u8` | bit 0 up, bit 1 down, bit 2 left, bit 3 right, bit 4 fire (Space), bit 5 secondary weapon (X) |

Bits 6 and 7 of `keys` must be `0`, otherwise the message is ignored. The
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

### `SETTINGS` (0x17)

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | difficulty | `u8` | `0` easy, `1` normal, `2` hard, `3` brutal |
| 1 | friendly fire | `u8` | `0` off, `1` on |

Sent by the host while it chooses the settings, before `SELECT_MAP`. The
server ignores it when the sender is not the host, when the game is not in
the lobby phase, or when a value is out of range. Otherwise it stores the
settings, which apply to the next match (the lives are given again when the
match starts), and every client sees them in `STATUS`.

### `HISTORY_REQUEST` (0x18)

Empty payload. Asks for the list of every victory since the start of the
game (the leaderboard tab). The server answers with one or more
[`HISTORY`](#history-0x2a) pages, to this client only.

### Maps written by the players

A player writes a map as a JSON file (see [custom maps](custom-maps.md)). The
files stay on the client: the host sends the one it chooses, the server checks
it and plays it. The server never reads a file of its own for this.

```
 host client                                   server
 │ ── MAP_UPLOAD_BEGIN (size) ───────────────► │  size 1 to 262144, else refused
 │ ── MAP_UPLOAD_CHUNK (≤ 1100 bytes) × n ───► │  appended in order
 │ ── MAP_UPLOAD_END ────────────────────────► │  size must match, JSON is parsed
 │ ◄── MAP_UPLOAD_RESULT (ok, text) ───────── │  and checked again
 │ ◄── CUSTOM_INFO (to every client) ──────── │  if ok: the match starts
```

The messages are reliable and applied in order, so the pieces arrive in the
order they were sent. The client sends at most 8 pieces per frame. A full upload is 239 pieces at
most, so even with no Ack at all it stays under the 256 waiting messages that
get a client kicked.

The map has the id `3` (`CUSTOM_MAP`) in the `map` field of `STATUS`, which
accepts the values 0 to 3. `SELECT_MAP` still only accepts 0 to 2. The host
chooses the [`SETTINGS`](#settings-0x17) (difficulty, friendly fire) before
sending the map, as for any other map; they are in `STATUS` and in the records.

### `MAP_UPLOAD_BEGIN` (0x19)

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | size | `u32` | size in bytes of the JSON, 1 to 262144 |

Starts an upload and drops the previous one. Ignored when the sender is not
the host or when the game is not in the lobby. A wrong size is answered with a `MAP_UPLOAD_RESULT` that is not ok.

### `MAP_UPLOAD_CHUNK` (0x1A)

The payload is a piece of the JSON, 1 to 1100 bytes, with no other field. The
upload is **dropped** when the sender is not the one that began it, when no
upload is running, or when the pieces together would be bigger than the size
announced. The server therefore never keeps more than 256 KiB for an upload.

### `MAP_UPLOAD_END` (0x1B)

No payload. The server checks that every byte arrived, parses the JSON, checks
the map (types, ranges, totals, at least one boss) and starts it. It answers
with `MAP_UPLOAD_RESULT`. Ignored when the sender is not the host.

### `MAP_UPLOAD_RESULT` (0x2C)

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | ok | `u8` | 1 when the map starts, 0 otherwise |
| 1 | size `n` | `u8` | length of the text, at most 200 |
| 2 | text | `n` bytes | why the map was refused (empty when ok) |

The text starts with the path of the faulty field, e.g.
`map.levels[0].waves[2].kind: unknown "Tonk"`. A map is refused as a whole.
It is also refused when a game is already running.

### `CUSTOM_INFO` (0x2B)

Sent to every client when a custom map starts, and to a client that connects
while a custom map is played.

| Field | Type | Meaning |
|---|---|---|
| name size `n` | `u8` | at most 40 |
| name | `n` bytes | printable characters only |
| hash | `u64` | FNV-1a of the JSON: identifies the map and its records |
| levels `m` | `u8` | at most 100 |
| zone (× m) | `u8` | scenery of each level, `0` to `6` (`Zone`) |

### `CUSTOM_RECORDS_GET` (0x1C)

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | hash | `u64` | hash of the map, as in `CUSTOM_INFO` |

Asks for the best times of a map. The server answers with `CUSTOM_RECORDS`
(empty when the map was never won).

### `CUSTOM_RECORDS` (0x2D)

| Field | Type | Meaning |
|---|---|---|
| hash | `u64` | hash of the map |
| records | 4 blocks | one block per difficulty, in the order of the enum, as in `LEADERBOARD`: count `n` (`u8`, at most 3), then per record the time (`u32`, milliseconds, fastest first), the players (`u8`, 1 to 4) and the settings (`u8`) |

The best times of one map for each difficulty. A block only holds records of
its own difficulty. Sent to a client that asks for them, and to everybody after
the victory on a custom map. The server keeps the records of at most 500 maps,
in `custom_leaderboard.txt`. Changing one byte of a map changes its hash, so a
modified map starts with an empty leaderboard.

### `BUY` (0x15)

Asks the server to buy one augment for the sender.

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | id | `u8` | augment id, see [`AUGMENTS`](#augments-0x26) |

The payload size must be exactly 1 and the id must be known, otherwise the
message is ignored. The client sends the **id** and not the position of the
offer: if the offers change while the message travels, the purchase is simply
refused instead of buying another augment. The server checks every rule
(the game is in the shop phase, the augment is on sale, the team has
enough coins) and always answers with
`BUY_RESULT`.

### `READY` (0x16)

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | ready | `u8` | `1` ready, `0` not ready |

Only accepted during the shop phase. When every connected player is ready,
the server starts a countdown (shown in `STATUS`); it stops as soon as a
player is no longer ready. When it ends, the next level starts.

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
| 30 | player rocket |
| 31 | player laser (flash) |
| 32 | lightning segment going down |
| 33 | lightning segment going up |
| 34 | drone |

A value of 35 or more is invalid, and so is a player slot of 4 or more.

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
| 0 | phase | `u8` | bits 0-6: 0 playing, 1 victory, 2 defeat, 3 lobby, 4 shop; bit 7 (`0x80`): the match is paused |
| 1 | boss hp | `u16` | `0` when there is no boss |
| 3 | boss max hp | `u16` | the boss hp cannot exceed it |
| 5 | coins | `u32` | the shared purse of the players |
| 9 | elapsed time | `u32` | milliseconds since the start of the match, frozen when it ends |
| 13 | level | `u8` | number of the level being played, starting at 1 |
| 14 | map | `u8` | index of the map being played (or last played), 0 to 2 |
| 15 | deaths | `u16` | number of times a player was destroyed in this match |
| 17 | difficulty | `u8` | `0` to `3`, see [`SETTINGS`](#settings-0x17) |
| 18 | friendly fire | `u8` | `0` or `1` |
| 19 | count `n` | `u8` | number of players, at most 4 |
| 20 + 6 × i | slot | `u8` | 0 to 3 |
| 21 + 6 × i | hp | `u16` | |
| 23 + 6 × i | alive | `u8` | `0` or `1` |
| 24 + 6 × i | ready | `u8` | `0` or `1`, only meaningful in the shop phase |
| 25 + 6 × i | lives | `u8` | lives left, `0` when the player is eliminated |
| 20 + 6 × n | countdown | `u16` | milliseconds before the next level, `0` when the countdown is not running |

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

### `LEADERBOARD` (0x29)

The three best victory times of every map **and every difficulty**, sent to
a client when it connects and to everybody after each victory. Three maps,
each with four blocks (easy, normal, hard, brutal, in the order of the
difficulty byte), so twelve blocks one after the other:

| Field | Type | Meaning |
|---|---|---|
| count `n` | `u8` | records of this block, at most 3 |
| time (× n) | `u32` | milliseconds, fastest first |
| players (× n) | `u8` | number of players of that run, 1 to 4 |
| settings (× n) | `u8` | bits 0-1: difficulty, bit 2: friendly fire, other bits `0` |

Each record takes 6 bytes (time, players, settings). A payload with trailing
bytes, a count above 3, `0` players, unknown bits in the settings, or a
record whose difficulty is not the one of its block is rejected.

### `HISTORY` (0x2A)

A page of every victory since the start of the game, sent after a
`HISTORY_REQUEST`. The history can hold hundreds of runs, so it is cut in
pages that fit in one datagram (168 runs at most). An empty history is still
one page.

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | start | `u16` | index of the first run of this page |
| 2 | total | `u16` | number of runs in the whole history |
| 4 | count `n` | `u8` | runs in this page |
| 5 + 7 × i | map | `u8` | 0 to 2 |
| 6 + 7 × i | time | `u32` | milliseconds |
| 10 + 7 × i | players | `u8` | 1 to 4 |
| 11 + 7 × i | settings | `u8` | same bits as in `LEADERBOARD` |

Pages are reliable, so they arrive in order. The client starts again from an
empty list on the page that starts at `0` and ignores a page that does not
follow the previous one. The runs are in the order they were won; the client
sorts them by time.

### `AUGMENTS` (0x26)

The augments owned by every player. Sent when they change (a purchase, a
player who leaves) and once to a client that just connected.

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | count `p` | `u8` | number of players listed, at most 4 |

Then, for each listed player:

| Field | Type | Meaning |
|---|---|---|
| slot | `u8` | 0 to 3, a slot appears at most once |
| count `n` | `u8` | number of augments, at most 32 |
| ids | `u8` × n | augment ids, see the table below |

A player without augments is not listed, so an empty message is the single
byte `0`. The message is rejected if a slot is out of range or listed twice,
an id is unknown, a count is too large, or bytes are missing or left over.
The message replaces the whole previous list on the client.

| Id | Augment |
|---|---|
| 0 | DamageUp |
| 1 | SpeedUp |
| 2 | MaxHpUp |
| 3 | FireRateUp |
| 4 | Regeneration |
| 5 | Shield |
| 6 | MultiShot |
| 7 | Rocket (secondary weapon) |
| 8 | Laser (secondary weapon) |
| 9 | Lightning (secondary weapon) |
| 10 | Drone (gives the drone, level 1) |
| 11 | DroneBoost (drone level 2) |
| 12 | DroneTwin (drone level 3) |

The three weapons replace each other: the equipped one is the last weapon id
of the list, and the three drone ids give the drone the level 1, 2 or 3 (never lowering it). The
server fires the weapons from the `KEY_ALT` bit of `INPUT`; nothing about the
weapons travels on the network except the entities they create (rockets,
flashes, lightning segments, the drone and its bullets) and the augment ids.

Prices and rarities are not sent: the client reads them from the shared
augment table (`logic/Augments.hpp`).

### `SHOP` (0x27)

The augments currently on sale.

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | count `n` | `u8` | number of offers, at most `SHOP_OFFER_COUNT` |
| 1 + i | id | `u8` | augment id |

The payload size must be exactly `1 + n` and every id must be known. Sent
when the offers change and once to a client that just connected. An empty
list means the shop is closed. The server opens the shop at the end of every
level and after a victory, as the shop phase: the game waits until every
player sent `READY`.

### `BUY_RESULT` (0x28)

The answer to a `BUY`, sent to the buyer only.

| Offset | Field | Type | Meaning |
|---|---|---|---|
| 0 | id | `u8` | the augment of the `BUY` |
| 1 | ok | `u8` | `1` bought, `0` refused |

The payload size must be exactly 2, the id must be known and `ok` must be `0`
or `1`. After a successful purchase, the new `AUGMENTS`, `SHOP` and `STATUS`
messages tell every client what changed.

## Reliability

UDP can lose, duplicate and reorder datagrams. The protocol picks the delivery
per message:

- **Reliable** (every `ROOM_*` message, `SPAWN`, `DESTROY`, `YOU`, `RESPAWN`, `SELECT_MAP`, `LEAVE_MATCH`, `PAUSE`, `LEADERBOARD`, `HISTORY`,
  `HISTORY_REQUEST`, `SETTINGS`, `AUGMENTS`, `SHOP`, `BUY`, `READY`,
  `BUY_RESULT`, and the messages of the custom maps): a spawn, a death or a purchase must
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
  a boss hp above its maximum, a map that does not exist, an unknown zone) or a non-finite float makes the message be
  ignored. A malformed message never reaches the game.
- Bounds against memory abuse: 1200 bytes per datagram, 4096 datagrams queued
  by the socket, 64 early reliable messages kept, and a client with more than
  256 messages waiting for an Ack is kicked.
- The server ignores any game message from an address that has no session.
- A custom map comes from a client, so it is never trusted: only the host may
  send one, the upload is capped at 256 KiB, one upload at a time, and the JSON
  is parsed again with limits on its size, its nesting and its contents (see
  [custom maps](custom-maps.md#limits)). A bad file is refused, it never
  crashes the server.
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
