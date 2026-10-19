# Technical and comparative study

> **Status: draft.** The choices below are the ones present in the repository.
> The alternatives and the reasoning are a first version that the whole team
> should review and complete. Parts marked **TODO** need the owner of the topic.

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
| Randomness | injected generator (planned) | global `rand()` | tests stay deterministic |

## Network protocol

| Topic | Choice | Alternatives | Why |
|---|---|---|---|
| Transport | **UDP** | TCP | required for in-game traffic; no head-of-line blocking, a late position is useless anyway |
| Encoding | **binary** | JSON / text | required; smaller, faster to parse |

Reliability, packet layout and sequence handling: **TODO**, see
[the protocol page](protocol.md).

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
| Flooding by a client | rate limiting and timeouts on the server (**TODO**) |
| One client crashes the server | the server treats all network input as untrusted, and a failing client is removed without stopping the game |
| Vulnerable dependencies | dependencies come from a pinned vcpkg baseline (`vcpkg-configuration.json`); update the baseline regularly and watch the security advisories of Asio and SFML (**TODO**: who and how often) |

## Long-term monitoring (TODO)

- Update the vcpkg baseline on a schedule and rebuild.
- Run the test suite and a sanitizer build (address / undefined) in CI.
- Fuzz the packet decoder once it exists.
