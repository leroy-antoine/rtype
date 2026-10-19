# Network protocol

> **Status: draft skeleton. The protocol is not designed yet.** This page lists
> what the subject requires and what must be written here. Nothing below the
> requirements is decided. When the protocol exists, this page becomes the
> reference: someone must be able to write a new client by reading it alone.

## Requirements (from the subject)

- The protocol is **binary**, not text.
- All in-game traffic (entities, movements, events) uses **UDP**. A second TCP
  connection is tolerated only for specific cases with a strong justification.
- The server notifies the clients when a monster spawns, moves, is destroyed,
  fires, kills a player, and when other players act.
- A malformed packet MUST NOT crash the client or the server, consume
  excessive memory, or compromise security (buffer overflows included).
- A crashing client must not stop the server, and the other clients are told.
- It must be documented. A formal document (an RFC) is accepted.

## What this document will contain

| Section | Content |
|---|---|
| Overview | roles, transport, ports, one datagram = one message? |
| Data types | sizes, byte order, how floats / ids / strings are encoded |
| Packet header | magic / version, message type, size, sequence number |
| Messages from the client | connect, input (the keys), disconnect, ... |
| Messages from the server | accept / refuse (game full), entity spawn, entity update, entity destroy, events, game over, ... |
| Connection lifecycle | connection, keep-alive, timeout, disconnection |
| Reliability | what happens when a datagram is lost, duplicated or reordered |
| Error handling | what is done with a packet that is too short, too long or invalid |
| Security | validation rules, rate limits |
| Examples | annotated hexadecimal dumps |

## Inputs the game logic expects

For the game logic to work, the server must be able to give it, for each player
and each tick, the state of five keys: **up, down, left, right, fire**
(`PlayerInput`). The state is held until the next update; it is not an event.
See [the game logic API](../src/games/rtype/doc/README.md#public-api-the-game-class).

The server also has to send the world to the clients as *kind of entity +
position* (plus events). The format of that snapshot is part of this protocol.

## Open questions

- Snapshot of all entities every tick, or only the changes?
- Which messages need reliability on top of UDP (connection, game over)?
- How are the four players identified (the logic gives each a slot 0 to 3)?
