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
| `rtype::common` | `src/include` | shared headers: vectors (`utils/Vectors.hpp`), return codes |
| `rtype::game` | `src/games/rtype` | the game logic, header-only |
| `r-type_server` | `src/server` | the authoritative server (links `rtype::game` once the glue exists) |
| `r-type_client` | `src/client` | the display |

## Dependency rules

- `engine::core` depends on nothing of the project.
- `rtype::game` depends on `engine::core` and `rtype::common` only. It never
  includes SFML, Asio or any network code.
- The client renders through `engine::window` and never includes the game
  logic. It only needs *a kind of entity and a position*.
- The server library knows packets and a tick, not R-Type. Only the glue code
  (the `main`) includes `rtype::game`.

These rules keep each layer replaceable and testable alone: the game logic can
be run and tested without a window or a socket.

## Who talks to whom at runtime

```
 client                      server                              game logic
 ──────                      ──────                              ──────────
 keys ──── UDP input ─────►  decode ──► Game::setInput(slot, in)
                             every tick:  Game::update(dt)
 draw ◄─── UDP snapshot ───  encode ◄── state of the world
```

- The **server has the final authority**. Clients send keys, never positions.
- The **game logic receives `dt`** and reads no clock. The server owns the
  clock (`Engine::FixedTimestep`).
- The **client draws from positions and sprite kinds only**.

## Where to read more

- Game logic: [src/games/rtype/doc](../src/games/rtype/doc/README.md)
- Server: [src/server/doc](../src/server/doc/README.md)
- Client: [src/client/doc](../src/client/doc/README.md)
