# Server

Documentation of the **server** (`src/server`).

The server is the authority of the game. It runs the simulation, receives the
keys of the players and tells every client what happens.

- [Role](#role)
- [Usage](#usage)
- [Architecture](#architecture)
- [Threads](#threads)
- [The game loop](#the-game-loop)
- [What is sent, and when](#what-is-sent-and-when)
- [Clients that join, leave or crash](#clients-that-join-leave-or-crash)
- [Robustness](#robustness)
- [Limits and open questions](#limits-and-open-questions)

## Role

From the subject, the server MUST:

- notify each client when a monster spawns, moves, is destroyed, fires, kills a
  player, and notify the other clients of a player's actions;
- not block on the clients: the game runs frame after frame whatever they do;
- survive a crashing client and tell the other clients of that game;
- use a **binary protocol over UDP** for all in-game communication (see
  [the protocol](../../../docs/protocol.md)).

## Usage

```
./build/binaries/r-type_server <port>
```

The server listens on every IPv4 interface, in **UDP**. Open the port in UDP
in the firewall to play from another machine. `Ctrl+C` (SIGINT) or SIGTERM
stops it cleanly.

## Architecture

The code is in `src/server/game`. `main.cpp` parses the port and runs
`server::App`, which is the glue between the network and the game logic:

```
 ┌────────────────┐ events  ┌────────────┐  calls  ┌───────────────┐
 │ Engine::Server │ ──────► │ server::App│ ──────► │ rtype::Game   │
 │ (UDP, Asio)    │ ◄────── │  (glue)    │ ◄────── │ (game logic)  │
 └────────────────┘ packets └────────────┘  state  └───────────────┘
```

`App` is the only place that knows both the network (`engine::network`) and
the game (`rtype::game`). The network module knows nothing of R-Type and the
game logic knows nothing of the network.

| Network event | What `App` does |
|---|---|
| a client connects | `game.addPlayer()`, remembers `client id -> slot`, sends `YOU` and the whole world (`SPAWN` for each entity). If the game is full, it disconnects the client |
| `INPUT` | decodes the keys and calls `game.setInput(slot, input)`; an invalid payload is ignored |
| `RESPAWN` | calls `game.respawnPlayer(slot)` |
| `LEAVE_MATCH` | if the sender is the host, calls `game.openLobby()`: the match is dropped and the lobby is shown again |
| `PAUSE` | if the sender is the host, `game.setPaused(paused)`: the simulation and the clock stop until the host resumes |
| `SELECT_MAP` | if the sender is the host (`game.host()`) and the map exists, builds it with `buildMap(id)` and calls `game.startMap`; ignored otherwise |
| `BUY` | decodes the augment id, calls `game.buyAugment(slot, id)` and answers the buyer with `BUY_RESULT`; an invalid payload is ignored |
| `READY` | decodes the flag and calls `game.setReady(slot, ready)`; an invalid payload is ignored |
| a client disconnects or times out | `game.removePlayer(slot)`; the ship is destroyed and every client is told by a `DESTROY` |
| any message from an unknown client | ignored |

The message formats are in [the protocol](../../../docs/protocol.md). The game
API is in
[the game logic README](../../client/game/Rtype/doc/README.md#public-api-the-game-class).

## Threads

| Thread | Does |
|---|---|
| **network thread** (inside `Engine::UdpSocket`, Asio) | receives datagrams and queues them (bounded), sends the outgoing ones. It never touches the game |
| **game thread** (`main`) | owns `Game` and all the sessions: reads the queue in `update()`, simulates, writes the packets in `flush()` |

The game thread never waits for a client: it takes what the network thread
has already received, and goes on. A slow or silent client costs nothing to
the others. Because `Game` is only touched by the game thread, it needs no lock.

## The game loop

`App::run` gives the real elapsed time to `Engine::FixedTimestep`
(`engine/core`), which returns how many fixed steps (1/60 s) to run:

```
while (running) {
    steps = timestep.advance(elapsedSeconds);
    repeat steps times: tick()
    sleep until the next step
}
```

One `tick()` does:

1. `receive()`: `server.update()` and handle every network event;
2. `game.update(dt)`, then `recordVictory()`: on the tick when the phase becomes
   `Victory`, the time is added to the leaderboard and sent;
3. `broadcastEvents()`: `SPAWN` and `DESTROY` of this tick (reliable);
4. `broadcastStates()`: the positions (`STATES`, unreliable);
5. every 6th tick, `broadcastStatus()`: phase, boss and player health, coins
   of the shared purse, elapsed time, level number, map (`STATUS`, about 10 times per second);
6. `server.flush()`: sends the datagrams.

The game never reads a clock itself, it receives `dt`. Time does not depend on
the speed of the CPU.

## What is sent, and when

| Message | When | Delivery |
|---|---|---|
| `YOU`, `SPAWN` (whole world) | a client connects | reliable |
| `SPAWN`, `DESTROY` | an entity appears or disappears | reliable |
| `STATES` | every tick | unreliable |
| `STATUS` | every 6 ticks | unreliable |
| `OWN_STATE` | every tick, to one client | unreliable (position and the effects of the portals) |
| `LEADERBOARD` | a client connects, a match is won | reliable |

Monster movement and shots reach the clients as positions (`STATES`) and
spawns of bullets (`SPAWN`). The kill of a player or of a monster is a
`DESTROY`, and the end of the game is the `phase` of `STATUS`.

## Clients that join, leave or crash

- **Join**: a free slot (0 to 3) is given. The client receives `YOU` and the
  current world, so it can join in the middle of a match.
- **Leave**: the client sends Disconnect and its ship is destroyed.
- **Crash or network loss**: the transport sends a Ping every second and times
  a peer out after 5 seconds of silence. The server then handles it like a
  departure: `removePlayer`, `DESTROY` for the others. The match goes on.
- **Nobody connected**: the level does not advance, and starts again from zero
  when the first player comes back.
- **Lobby**: the server starts in the `Lobby` phase, and comes back to it
  when every player has left and after the end screen of a match. The *host*
  is the connected player with the lowest slot. Only the host can leave the
  lobby, with `SELECT_MAP`; the server builds the map with `buildMap(id)` and
  calls `Game::startMap`. A message from another player, an unknown map, or a
  message outside the lobby is ignored.
- **End of a match**: the end screen is shown for a few seconds
  (`RESTART_DELAY`). A defeat then restarts the same map; a victory returns to
  the lobby. A dead
  player can ask to come back with `RESPAWN` while the match is running.
- **Leaderboard**: after each victory the server adds the time and the number
  of players to the best times of the map (5 per map, `rtype::Leaderboard`),
  writes them in `leaderboard.txt` (working directory of the server, one
  `map time_ms players` line per record, read again at startup) and sends
  `LEADERBOARD` to every client. A new client receives it when it connects.

## Robustness

- A malformed datagram is dropped by the transport; a malformed game payload is
  ignored by the decoders (exact size, fields in range, finite floats). Nothing
  malformed reaches the game.
- A client sends *keys*, never a position or a hit.
- Memory is bounded: 1200 bytes per datagram, 4096 datagrams queued, and a
  client that does not acknowledge more than 256 reliable messages is kicked.
- A message from an address without a session, or with a wrong token, is
  dropped.

## Limits and open questions

- There is **one game per server**: every client joins the same lobby and the
  same match, up to 4 players. Several rooms are not implemented.
- There is no per-client rate limit on `INPUT` messages beyond the bounded
  queue. Each `INPUT` only overwrites the previous one, so flooding does not
  grow the game state, but a limit could be added.
- The server sends the same `STATES` to everybody: there is no interest
  management, which is fine for 4 players and a few dozen entities.
- The tests cover the game logic and the message encoding (see
  [Build and run](../../../docs/build.md#tests)). The server loop itself is not
  tested automatically.
