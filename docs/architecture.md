# Architecture

The subject asks for visible and decoupled layers for **rendering**,
**networking** and **game logic**, around a small game engine.

## Layers

```
                     ┌──────────────┐        ┌──────────────┐
                     │  r-type_client│        │ r-type_server │   executables
                     └───┬───────┬──┘        └──┬─────────┬─┘
                         │       │              │         │
          ┌──────────────┘       │              │         └───────────────┐
          ▼                      ▼              ▼                         ▼
 ┌─────────────────┐      ┌─────────────────────────┐            ┌──────────────┐
 │ engine::window  │      │      rtype::common      │            │ rtype::game  │
 │ (rendering)     │      │ Vec2 types, return codes│            │ (game logic) │
 └─────────────────┘      └───────────┬─────────────┘            └──────┬───────┘
                                      │                                 │
                                      └─────────────┬───────────────────┘
                                                    ▼
                                           ┌─────────────────┐
                                           │  engine::core   │
                                           │  (ECS, timestep)│
                                           └─────────────────┘
```

| Target | Folder | Role |
|---|---|---|
| `engine::core` | `engine/core` | ECS: `Registry`, `Scheduler`, `CommandBuffer`, `FixedTimestep` |
| `engine::window` | `engine/window` | window, events, textures, renderer (wraps SFML) |
| `engine::audio` | `engine/audio` | sounds and music (wraps SFML) |
| `engine::network` | `engine/network` | UDP transport: sessions, ping, reliability, binary reader and writer (wraps Asio) |
| `rtype::common` | `src/include` | shared headers: vectors (`utils/Vectors.hpp`), return codes |
| `rtype::protocol` | `src/include/rtype/Protocol.hpp` | the encoding of the R-Type messages, shared by the server and the client |
| `rtype::game` | `src/client/game/Rtype` | the game logic, header-only: rules, monsters, bosses, walls, lobby, leaderboard |
| `rtype::maps` | `src/client/game/Rtype/src/maps` | the data of the maps (levels, waves, walls); linked by the server and the tests |
| `r-type_server` | `src/server` | the authoritative server |
| `r-type_client` | `src/client` | the display |

## Dependency rules

- `engine::core` depends on nothing of the project.
- `rtype::game` depends on `engine::core` and `rtype::common` only. It never
  includes SFML, Asio or any network code.
- `engine::network` knows messages and sessions, not R-Type. The meaning of a
  message is defined by `rtype::protocol`.
- The client renders through `engine::window` and never includes the game
  logic. It keeps a replica of the world made of *a kind of entity and a
  position*, filled from the network.
- The server's `App` class is the glue: it is the only place that includes both
  `engine::network` and `rtype::game`.

These rules keep each layer replaceable and testable alone: the game logic can
be run and tested without a window or a socket.

## Who talks to whom at runtime

```
 client                      server                              game logic
 ──────                      ──────                              ──────────
 keys ──── INPUT ─────────►  decode ──► Game::setInput(slot, in)
 map  ──── SELECT_MAP ────►  host? ───► Game::startMap(segments, id)
                             every tick:  Game::update(dt)
 draw ◄─── SPAWN / DESTROY / encode ◄── events, states, status
           STATES / STATUS / LEADERBOARD
```

## The life of a match

```
 Lobby ──(host: SELECT_MAP)──► Playing ──► Victory ─┐
   ▲                              │                 │ end screen
   │                              └──────► Defeat ──┤ (RESTART_DELAY)
   └─────────────────────────────────────────────────┘
```

The server starts in the lobby and comes back to it after each match, or when
the last player leaves. The server also records the time of every victory in
the leaderboard of the map.

- The **server has the final authority**. Clients send keys, never positions.
- The **game logic receives `dt`** and reads no clock. The server owns the
  clock (`Engine::FixedTimestep`).
- The **client draws from positions and sprite kinds only**.
- The server has two threads: the network thread (receives and sends
  datagrams) and the game thread (simulation). See
  [the server](../src/server/doc/README.md#threads).
- The messages are described in [the protocol](protocol.md).

## Where to read more

- Game logic: [src/client/game/Rtype/doc](../src/client/game/Rtype/doc/README.md)
- Server: [src/server/doc](../src/server/doc/README.md)
- Network module: [engine/docs/network.md](../engine/docs/network.md)
- Client: [src/client/doc](../src/client/doc/README.md)
