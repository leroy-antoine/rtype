# Client

Documentation of the **client** (`src/client`).

The client is the **display** of the game. It reads the keyboard, sends the
state of the keys to the server, receives the state of the world and draws it.
It does not decide anything about the game: deaths, hits and spawns come from
the server.

- [Usage](#usage)
- [Architecture](#architecture)
- [Files](#files)
- [A frame](#a-frame)
- [Controls](#controls)
- [Rendering module](#rendering-module)
- [Sound](#sound)
- [Gameplay requirements](#gameplay-requirements)
- [Limits](#limits)

## Usage

```
./build/binaries/r-type_client <host> <port>
```

The `assets` folder (sprites and sounds) is copied next to the executable by
CMake and is looked up there. Without sound files or without an audio device,
the game runs without sound.

## Architecture

```
 keyboard ─► Controls ─► Network ─► server
 server ─► Network ─► Replica (local copy of the world) ─► GameView ─► window
                              └─► Audio (sounds and music)
```

The client does **not** include the game logic. It keeps a *replica* of the
world: a small `Engine::Registry` filled from the `SPAWN`, `DESTROY` and
`STATES` messages, with a position, a size and a sprite kind for each entity.
The renderer only needs an appearance and a position, so it never learns what
an entity can do.

## Files

All the code is in `src/client`.

| File | Role |
|---|---|
| `main.cpp` | parses `<host> <port>`, runs `client::App` |
| `game/App` | the main loop: poll the input, send it, receive, draw |
| `network/Network` | wraps `Engine::Client`: connection events, sending the keys, decoding the messages into the replica |
| `game/Replica` | the local copy of the world, the last `STATUS`, the slot of this player, the sound cues |
| `game/Controls` | turns window events into the five keys of a `PlayerInput` |
| `game/GameView` | draws the entities of the replica |
| `game/SpriteBank` | maps each kind of entity to a texture and a source rectangle |
| `game/Background` | the star-field |
| `game/Hud` | health bars, boss bar, respawn hint, end screens |
| `game/Font` | small bitmap font for the HUD, drawn with rectangles |
| `game/Audio` | sounds and music |

## A frame

`App::loop` does, for each frame (60 FPS):

1. `Controls::poll`: handle window events (keys, close);
2. `sendControls`: send `RESPAWN` if requested, send `INPUT`, flush;
3. `Network::update`: receive messages and apply them to the replica;
4. `Replica::update(dt)`: advance local effects such as explosions;
5. play the sounds, then draw the background, the entities and the HUD.

`dt` is measured with a steady clock. The scrolling of the star-field and the
animations use this time, never the speed of the CPU.

## Controls

| Key | Action |
|---|---|
| Arrow keys | move the ship |
| Space | fire (hold to keep firing) |
| Enter | come back after being killed (while the match is running) |
| Escape | quit |

The client sends the **state** of the keys every frame (see `INPUT` in
[the protocol](../../../docs/protocol.md#input-0x10)).

## Rendering module

The client draws through the engine's window module (`engine::window`), which
hides SFML behind a small interface:

| Class | Role |
|---|---|
| `Engine::Window` | opens a window, polls events, clears and displays, limits the frame rate |
| `Engine::TextureManager` | loads textures and returns a `TextureID` |
| `Engine::Renderer` | `draw(textureId, position, sourceRect, scale)` and `drawRect` for the HUD |
| `Engine::Event` | a variant: closed, resized, focus, `KeyPressed` / `KeyReleased` |

The window is 1280 x 720. The world is 1920 x 1080, so everything is scaled by
`WORLD_TO_SCREEN` (2/3).

- **Star-field**: three layers of stars with parallax, scrolling from right to
  left at the speed of the level (`SCROLL_SPEED`). Purely visual.
- **Players**: the four players are told apart by their slot (a different
  sprite row for each one).
- **HUD**: one health bar per player, a boss bar while the boss is there, a
  hint to respawn, and a message on victory or defeat.

## Sound

`Audio` plays sounds (`shoot`, `hit`, `explosion`, `boss_alert`, `victory`,
`defeat`) and two music tracks (level, boss). The replica turns what it sees
into *cues* (a player bullet appeared, an entity exploded, the boss appeared),
and `Audio` plays them. The sounds of the world received at connection are
skipped for half a second so they are not replayed. The music follows the
phase of the game.

## Gameplay requirements

From the subject, on the client side:

- a slow horizontal scrolling star-field background: done;
- the players move with the **arrow keys**: done;
- Bydos slaves and missiles are visible: done;
- the four players are distinctly identifiable: done, by slot;
- the timing of the scrolling and of the animations must not depend on the
  speed of the CPU: done, they use measured time.

## Limits

- The client does no prediction: it draws the last positions received from the
  server. Over a good connection the movement is smooth, over a bad one it can
  look jerky.
- If the server cannot be reached, the client closes with an error message.
  There is no menu and no reconnection.
- Keys cannot be remapped yet.
