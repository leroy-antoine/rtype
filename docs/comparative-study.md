# Technical and comparative study

> The choices below are the ones present in the repository, with the
> alternatives and the reasoning. Parts marked **TODO** still need the owner of
> the topic.

## Language and build

| Choice | Alternatives | Why |
|---|---|---|
| **C++20** | C++17, Rust | gives `std::optional`, `constexpr` improvements and concepts; low-level control for a real-time game; the language of the course |
| **CMake** (>= 3.25) with presets | Makefile, Meson, Bazel | the de facto C++ standard, understood by every IDE, one description for Linux and Windows |
| **vcpkg** as a submodule, manifest mode (`vcpkg.json`) | Conan, system packages, copying sources | the subject forbids copying dependency sources in the repository; a manifest pins what the project needs and works on both platforms |

## Libraries

| Need | Choice | Alternatives | Why |
|---|---|---|---|
| Rendering, input, audio | **SFML 3** | SDL2, Raylib | simple C++ API, good for 2D sprites, allowed by the subject (not a "too broad" engine). Hidden behind `engine::window`, so it can be replaced without touching the game |
| Networking | **Asio** | raw BSD / Windows sockets, ENet | cross-platform, UDP support, asynchronous model suits a server that must not block; raw sockets would need our own abstraction anyway |

## Architecture

| Topic | Choice | Alternatives | Why |
|---|---|---|---|
| Game representation | **ECS** (archetype based, `engine/core`) | class hierarchy of game objects | the renderer only needs appearance and position; data and behaviour are separated, new behaviour is a new component and system; cache friendly iteration |
| Communication model | **authoritative server** | peer-to-peer, lockstep | required by the subject; prevents cheating, one source of truth |
| Layer glue | **mediator in the `main`** | generic `IGame` interface | the game logic stays free of network code; the server stays free of R-Type code; simplest thing that works |
| Time | **fixed time step**, `dt` given by the caller | variable `dt` | same inputs give the same game; simpler physics; independent of CPU speed (required) |

## Algorithms and data structures (game logic)

| Topic | Choice | Alternatives | Why |
|---|---|---|---|
| Collision | **AABB** (axis-aligned boxes) tests, bullets against targets | circles, pixel-perfect, spatial grid / quadtree | boxes are cheap and exact enough for sprites; with 4 players and a few dozen entities a spatial structure would cost more than it saves. A grid can be added if the entity count grows |
| Entity lookup | `each<...>` queries then `std::vector<Entity>` | keeping lists of players / monsters | the ECS already knows the entities; no second list to keep in sync |
| Structural changes | collect first, act after the loop | changing the registry while iterating | iterating while creating or destroying invalidates the iteration; the engine forbids it |
| Player slots | table of 4 booleans, first free slot | counting players | counting gives the wrong slot after someone leaves |
| "No result" | `std::optional` | magic value `-1`, exceptions | the type states the case, the compiler forces the check |
| Constants | `constexpr` values in `Config.hpp` | `#define`, runtime config file | typed, scoped, checked at compile time, one place to tune the game |
| Randomness | generator owned by `Game`, seedable (`Game(seed)`) | global `rand()` | the height of the waves is random in play, and reproducible in the tests |
| Waves | a constant table of steps (`Waves.hpp`) | spawning from a timer | the level is data, changing it never touches a system; it follows the distance scrolled, not the CPU |
| Replication | the game gives events (`SPAWN`, `DESTROY`), states and a status; entities get a `NetId` | sending the whole registry | the network layer does not depend on the ECS, and the client rebuilds the world from small messages |

## Network protocol

| Topic | Choice | Alternatives | Why |
|---|---|---|---|
| Transport | **UDP** | TCP | required for in-game traffic; no head-of-line blocking, a late position is useless anyway |
| Encoding | **binary**, big-endian, hand-written reader and writer | JSON / text, a serialization library | required; smaller, faster to parse, every read is bounds-checked and returns `std::optional` |
| Reliability | **per message**: events are reliable (resend until Ack, ordered), positions and inputs are not | everything reliable, everything unreliable | a lost spawn would break the client's world, but a lost position is replaced by the next one |
| Sessions | handshake with a random token, ping every second, 5 s timeout | stateless packets | a crashed client is detected and removed, and datagrams from other addresses are rejected |
| State sent | the positions of all moving entities every tick, events on change | deltas only, compression | simple and robust for 4 players and a few dozen entities (at most 98 states per datagram) |
| Threads | a network thread for the socket, a game thread for everything else | one thread, a thread per client | the game never waits for a client, and the game state needs no lock |

The packet layout, the sequence numbers and the reliability rules are in
[the protocol page](protocol.md) and [the network module](../engine/docs/network.md).

## Storage

The game keeps its whole state in memory and has no persistence in Part 1: a
match lasts one level and is lost when the server stops.

| Option | Persistence | Reliability | Constraint | Verdict |
|---|---|---|---|---|
| In memory only | none | lost on crash | none, fastest | **chosen** |
| Flat file (JSON / binary) | yes | simple, can corrupt on crash without atomic write | trivial to add | for a high-score list, if needed |
| Embedded database (SQLite) | yes | transactions, crash safe | extra dependency | if scores or accounts are added |
| Server database (PostgreSQL) | yes | very reliable | deployment cost, overkill | not justified here |

## Security

| Risk | Mitigation |
|---|---|
| A client cheats | server authority: clients send keys only, never positions, hits or damage |
| Malformed or oversized packets | validate every size and field before use; bounded buffers (see [protocol](protocol.md)) |
| Flooding by a client | bounded memory: 1200 bytes per datagram, 4096 datagrams queued, 64 early reliable messages, a client with more than 256 unacknowledged messages is kicked. An `INPUT` only overwrites the previous one. There is no per-client rate limit yet (**TODO**) |
| Spoofed or replayed datagrams | a random session token and the address of the session are checked, and an old sequence number is dropped |
| One client crashes the server | the server treats all network input as untrusted, and a failing client is removed without stopping the game |
| Vulnerable dependencies | dependencies come from a pinned vcpkg baseline (`vcpkg-configuration.json`); update the baseline regularly and watch the security advisories of Asio and SFML (**TODO**: who and how often) |

## Long-term monitoring

Done:

- the dependencies are pinned by the vcpkg baseline (`vcpkg-configuration.json`);
- the decoders reject invalid payloads, and the tests check it (truncated or
  padded messages, out-of-range fields).

To do (**TODO**, decide who and when):

- update the vcpkg baseline regularly, rebuild, and watch the security
  advisories of Asio and SFML;
- run the tests and a sanitizer build (address / undefined) in a CI;
- fuzz the packet parser (`Packet::parse`) and the game decoders.
