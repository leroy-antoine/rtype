# Server

Documentation of the **server** (`src/server`).

> **Status: skeleton.** The executable only prints a banner for now
> (`src/server/main.cpp`). This page describes what the subject requires, the
> architecture the rest of the project is built for, and what remains to be
> decided. Items marked **TODO** are to be confirmed by the server owner.

- [Role](#role)
- [Current state](#current-state)
- [Planned architecture](#planned-architecture)
- [The game loop](#the-game-loop)
- [Plugging a game in](#plugging-a-game-in)
- [Robustness requirements](#robustness-requirements)
- [Open questions](#open-questions)

## Role

The server is the **authority**. Clients send what the players do (their keys),
the server runs the game and tells every client what happened. Whatever the
server decides, the clients comply with.

From the subject, the server MUST:

- notify each client when a monster spawns, moves, is destroyed, fires, kills a
  player, and notify the other clients of a player's actions;
- be **multithreaded**, so that the game keeps running frame after frame
  whatever the clients do;
- survive a crashing client and tell the other clients of that game;
- use a **binary protocol over UDP** for all in-game communication (see
  [the protocol](../../../docs/protocol.md)).

## Current state

| Item | State |
|---|---|
| Executable `r-type_server` | builds, prints `R-Type server` |
| CMake | links `rtype::common` only (not `rtype::game`, on purpose, see below) |
| `src/server/network/`, `src/server/game/` | empty (`.gitkeep`) |
| Networking library | Asio is declared in `vcpkg.json`, not used yet |

## Planned architecture

The server is **generic**: it knows clients, packets and a tick, but nothing of
R-Type. The only place that knows both the network and the game is the `main`
(or a small class next to it), which acts as glue.

```
 ┌────────────┐   events    ┌──────────────┐  calls   ┌───────────────┐
 │ network    │ ──────────► │ main (glue)  │ ───────► │ rtype::Game   │
 │ (UDP, Asio)│ ◄────────── │              │ ◄─────── │ (game logic)  │
 └────────────┘  state out  └──────────────┘  state   └───────────────┘
```

| Network event | What the glue does |
|---|---|
| a client connects | `game.addPlayer()`; keeps the `client id <-> slot` table; refuses the client if it returns `std::nullopt` (game full) |
| a client sends its keys | decodes them into a `PlayerInput`, then `game.setInput(slot, input)` |
| a client disconnects or times out | `game.removePlayer(slot)`, tells the other clients |

The game logic API is documented in
[the game logic README](../../games/rtype/doc/README.md#public-api-the-game-class).

Suggested threads (**TODO**, to confirm):

- a **network thread** that receives datagrams and pushes decoded messages in a
  thread-safe queue, and sends outgoing packets;
- the **game thread** that owns the `Game` object, empties the queue at the
  start of each tick and runs `update`.

The `Game` object is only touched by the game thread, so it needs no lock.

## The game loop

The engine provides `Engine::FixedTimestep` (`engine/core`). The loop gives it
the real elapsed time and gets back how many fixed steps to run:

```
FixedTimestep timestep(rtype::config::FIXED_DT);

while (running) {
    steps = timestep.advance(elapsedSeconds);
    repeat steps times:
        apply the queued inputs   (game.setInput ...)
        game.update(timestep.step())
        send the new state to the clients
    sleep a little
}
```

The game never reads a clock itself: it receives `dt`. Time does not depend on
the speed of the CPU, as the subject requires.

## Plugging a game in

Two ways were discussed:

1. **The `main` links `rtype::game` and calls `Game` directly** (the current
   plan). Simple, no extra abstraction. The server library stays free of R-Type
   code.
2. **A generic `IGame` interface** that `rtype::Game` implements, so the server
   library could run any game.

Option 1 is enough for this project. Option 2 can be added later without
changing `rtype::Game`.

## Robustness requirements

From the subject, to keep in mind when writing the network code:

- A malformed packet MUST NOT crash the server, use excessive memory or
  compromise its security. Check every size and every field before using it.
- Never trust a client: a client sends *keys*, never a position or a hit.
- A slow or dead client must not block the others.

## Open questions

- How are the inputs encoded on the wire? (decides the shape of `PlayerInput`)
- Who owns the loop: the `main`, or a server class?
- How does the glue read the world to send it? (`Game` has no read API yet)
- How are client disconnections detected over UDP (timeout, heartbeat)?
- How many games can run at once (one game per server, or lobbies)?
