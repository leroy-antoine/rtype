# Client

Documentation of the **client** (`src/client`).

> **Status: prototype.** The executable opens a window and draws one sprite.
> This page describes what exists, what the subject requires, and the planned
> architecture. Items marked **TODO** are to be confirmed by the client owner.

- [Role](#role)
- [Current state](#current-state)
- [Rendering module](#rendering-module)
- [Planned architecture](#planned-architecture)
- [Gameplay requirements](#gameplay-requirements)

## Role

The client is the **display** of the game. It reads the keyboard, sends the
state of the keys to the server, receives the state of the world and draws it.
It does not decide anything about the game: deaths, hits and spawns come from
the server.

The subject allows the client to predict things such as the movement of its own
ship or of missiles, as long as the server has the final word.

## Current state

| Item | State |
|---|---|
| Executable `r-type_client` | opens an 800x600 window at 60 FPS and draws a ship sprite |
| CMake | links `rtype::common` and `engine::window` |
| Events | the window closes on a close event |
| `src/client/network/`, `src/client/game/` | empty (`.gitkeep`) |

## Rendering module

The client draws through the engine's window module (`engine::window`), which
hides SFML behind a small interface:

| Class | Role |
|---|---|
| `Engine::Window` | opens a window, polls events, clears and displays, limits the frame rate |
| `Engine::TextureManager` | loads textures and returns a `TextureID` |
| `Engine::Renderer` | `draw(textureId, position, sourceRect, scale)` |
| `Engine::Event` | a variant: closed, resized, focus, `KeyPressed` / `KeyReleased` (`Key::Up/Down/Left/Right/Space/Enter/Escape`) |

The renderer only needs **an appearance and a position**. It does not know what
an entity can do. This is why the game logic never talks about sprites: the
server will describe the world as *kind of entity + position*, and the client
maps each kind to a texture.

## Planned architecture

**TODO**, proposal:

```
 keyboard ─► Engine::Event ─► input state ─► network ─► server
 server ─► network ─► world snapshot ─► renderer ─► window
```

- **Input**: turn key events into the five booleans of the player input (up,
  down, left, right, fire) and send them when they change, or at a fixed rate.
- **World state**: keep the last snapshot received from the server; draw
  entities from their position and kind.
- **Star-field**: purely visual, scrolls slowly from right to left. The game
  logic does not know about it; it must use timers, not the CPU speed.
- **Sprites**: the provided sheets are in `src/games/rtype/assets`. The four
  players must be distinguishable (colour or sprite), the server gives each one
  a *slot* from 0 to 3 that can select it.
- **Sound**: basic sound design is required by the subject.

## Gameplay requirements

From the subject, on the client side:

- a slow horizontal scrolling star-field background;
- the players move with the **arrow keys**;
- Bydos slaves and missiles are visible;
- the four players are distinctly identifiable;
- the timing of the scrolling and of the animations must not depend on the
  speed of the CPU.
