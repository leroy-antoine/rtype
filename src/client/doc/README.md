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

The `assets` folder (sprites and sounds) and the `CustomMaps` folder (maps
written by the players, see [Create your own map](../../../docs/custom-maps.md))
are copied next to the executable by CMake and are looked up there. Without sound files or without an audio device,
the game runs without sound.

## Architecture

```
 keyboard ─► Controls ─► Network ─► server
 server ─► Network ─► Replica (local copy of the world) ─► GameView ─► window
                              └─► Audio (sounds and music)
```

The client does **not** run the game logic. It keeps a *replica* of the
world: a small `Engine::Registry` filled from the `SPAWN`, `DESTROY` and
`STATES` messages, with a position, a size and a sprite kind for each entity.
The renderer only needs an appearance and a position, so it never learns what
an entity can do.

Two techniques hide the network delay:

- **Interpolation**: the other entities are not drawn at the last position
  received, but a few ticks in the past (`INTERPOLATION_DELAY`, 3 ticks, 50 ms),
  between the two snapshots that surround that moment. Each `STATES` carries
  the server `tick`, and the replica keeps a render clock that follows it.
- **Prediction** (`Prediction`): the ship of this player is moved at once with
  its own keys, with the same movement as the server. Each `INPUT` has a
  `sequence`. The server answers with `OWN_STATE`, its position and the last
  `sequence` it applied. The client starts from that position, replays the
  inputs not yet acknowledged, and smooths the difference over a few frames. A
  gap above 150 units is corrected at once.

## Files

The client is in `src/client`. The game code is in `src/client/game/Rtype`,
with the headers in `include/client/` and the sources in `src/`. The folder
`include/logic/` holds the game logic headers shared with the server and the
tests: the client only uses its types (components, events, status, config).

| File | Role |
|---|---|
| `main.cpp` | parses `<host> <port>`, runs `client::App` |
| `network/Network` | wraps `Engine::Client`: connection events, sending the keys, decoding the messages into the replica, sending a custom map in pieces |
| `App` | the main loop: receive, poll the input, send it, draw |
| `Replica` | the local copy of the world, the interpolation, the last `STATUS`, the slot of this player, the lobby cursor, the leaderboard, the sound cues |
| `Prediction` | the predicted position of this player's ship, with the effects of the portals and the push of the rocks, and its reconciliation with `OWN_STATE` |
| `Controls` | turns window events into the five keys of a `PlayerInput`, plus the menu actions (cursor up or down, left or right, Enter, Escape) |
| `GameView` | draws the entities of the replica |
| `SpriteBank` | maps each kind of entity to a texture and a source rectangle |
| `Background` | the scenery: sky gradient, nebula, zone decor, star-field and boss aura (see [Background](#background)) |
| `Hud` | health bars, boss bar, coin counter, timer, level, respawn hint, end screens |
| `CustomMapList`, `HudLobby.cpp` | the maps written by the players that the client found in `CustomMaps`, with their errors (`CustomMapList`), and the two screens that show them: the list and their leaderboard (`HudLobby.cpp`). The screen shown is a step of `LobbyMenu` (`Custom`, `CustomScores`) |
| `Font` | small bitmap font for the HUD, drawn with rectangles |
| `Audio` | sounds and music |

## A frame

`App::loop` does, for each frame (60 FPS):

1. `Network::update`: receive messages and apply them to the replica;
2. `Controls::poll`: handle window events (keys, close);
3. `sendControls(dt)`: send `RESPAWN` if requested, predict the movement of
   this ship (which gives the `sequence`), send `INPUT`, flush;
4. `Replica::update(dt)`: advance the render clock, interpolate the entities,
   apply the prediction to this ship, advance effects such as explosions;
5. play the sounds, then draw the background, the entities and the HUD.

`dt` is measured with a steady clock. The scrolling of the star-field and the
animations use this time, never the speed of the CPU.

## Controls

| Key | Action |
|---|---|
| Arrow keys | move the ship |
| Space | fire: a tap shoots once, holding charges a piercing shot released with the key |
| X | fire the secondary weapon (rocket launcher, laser or chain lightning) |
| Enter | come back after being killed (while the match is running); in the lobby, open the settings of the selected map (host only), start the match from the settings, open the leaderboard |
| Up / Down | move the ship; in the lobby, move the cursor on the list of games |
| Tab | in the lobby, go to the next tab: maps of the game, **Custom map**, **Leaderboard** of the selected custom map |
| Right | in the Custom map tab, open the leaderboard of the selected map (Left / Right then change the difficulty) |
| R | in the Custom map tab, read the `CustomMaps` folder again |
| Escape | back one screen from the settings or the scores; quit from the first screen |

The client sends the **state** of the keys every frame, with a sequence number
(see `INPUT` in
[the protocol](../../../docs/protocol.md#input-0x10)).

## Rendering module

The client draws through the engine's window module (`engine::window`), which
hides SFML behind a small interface:

| Class | Role |
|---|---|
| `Engine::Window` | opens a window, polls events, clears and displays, limits the frame rate |
| `Engine::TextureManager` | loads textures and returns a `TextureID` |
| `Engine::Renderer` | `draw(textureId, position, sourceRect, scale)`, `drawResized` to a given size, and `drawRect` for the HUD |
| `Engine::Event` | a variant: closed, resized, focus, `KeyPressed` / `KeyReleased` |

The window is 1280 x 720. The world is 1920 x 1080, so everything is scaled by
`WORLD_TO_SCREEN` (2/3).

- **Star-field**: three layers of stars with parallax, scrolling from right to
  left at the speed of the level (`SCROLL_SPEED`). Purely visual.
- **Players**: the four players are told apart by their slot (a different
  sprite row for each one).
- **HUD**: one health bar per player, a boss bar while the boss is there, a
  hint to respawn, and a message on victory or defeat. The top right corner
  shows the shared purse: the spinning coin and the number of coins in yellow.
  The top left corner shows the elapsed time of the match (`mm:ss.d`), aligned with
  the `P1` label, followed by an animated stopwatch (`stopwatch_sheet.png`, 16 frames), above the health
  bars. The time and the coin counter share the same text size. The server counts it, so a player who joins late sees the
  same time as the others. It stops when the match ends and is drawn above the
  end screen, so the final time stays readable.
  The top centre shows the level being played (`LEVEL 3`), in the same text
  size. The server sends the number in `STATUS`.
- **Lobby**: when the phase is `Lobby`, a screen covers the game. It has
  steps kept by `LobbyMenu` (client side only; `Custom` and `CustomScores` show
  the maps written by the players, see `CustomMapList`):
  - *Games*: the title `SELECT A GAME`, the list (the maps, then
    `LEADERBOARD`) with a highlighted cursor (each player moves their own with
    Up / Down) and the three best times of the map under the cursor for one difficulty
    (`1. 02:13.4  2P  BRUTAL FF`, the difficulty in its color; Left / Right
    change the difficulty of the ranking, brutal first). Enter on a map
    opens the settings for the host (the first player of the `STATUS` list);
    the others see `WAITING FOR P1 TO CHOOSE`. Enter on `LEADERBOARD` opens
    the scores for anyone.
  - *Settings* (host only): `GAME SETTINGS`, the rows difficulty and friendly
    fire (Up / Down to change row, Left / Right to change the value), and a
    summary of the multipliers of the difficulty. Every change is sent with
    `SETTINGS`; Enter starts the match with `SELECT_MAP`, Escape goes back.
  - *Scores*: every victory of one map, fastest first (Left / Right changes
    the map, Up / Down scrolls, Enter or Escape goes back). The list is asked
    with `HISTORY_REQUEST` when the entry is opened and rebuilt from the
    `HISTORY` pages by the `Replica`.

  The times come from `LEADERBOARD`. The difficulty colors (green, orange,
  red, crimson) are in `DifficultyStyle.hpp`.
- **Lives**: small squares at the right of each health bar, `OUT` when the
  player is eliminated. They come from the `lives` of each player in `STATUS`.
- **Walls and portals**: drawn by repeating a 64 x 64 tile (`obstacle_rock.png`,
  `obstacle_spike.png`) over the size of the wall; a portal repeats an animated
  tile (`portal_reverse.png`, `portal_fast.png`, `portal_normal.png`). The
  HUD writes `CONTROLS REVERSED` or `SPEED UP` at the bottom left while the
  effect lasts.
- **Charge gauges**: under the ship, a yellow bar (cyan when full) fills while
  Space is held for more than 0.3 s, and an orange bar fills while X is held
  when the laser is equipped (`Replica::chargeTime`, `altTime`,
  `laserEquipped`, drawn by `GameView::drawCharge`). The client measures the
  hold time itself, from the keys it sends.
- **Weapons**: the player rocket uses its own sprite (`player_rocket.png`);
  the laser flash, the lightning segments and the drone are drawn with
  rectangles by `GameView` (`drawPlayerLaser`, `drawBolt`, `drawDrone`).
- **Coins**: dropped by monsters, drawn from a sprite sheet of 6 frames
  (`coin_spin_sheet.png`). The animated GIF is not used directly, because SFML
  only reads its first image.

## Background

`Background` draws, from back to front: a vertical gradient, a nebula, the
decor of the **zone**, three layers of stars and the **aura** of the boss that
is on screen. It never flashes: everything pulses slowly or drifts.

| Zone | Levels | Look |
|---|---|---|
| Deep space | 1-4 | dark blue sky, blue nebula |
| Nebula | 5-8 | purple sky and nebula |
| Asteroids | 9, 11, 12 | brown sky, two layers of drifting rocks |
| Wreckage | 13, 14, 16 | green sky, the girders of a derelict station |
| Core | 17-20 | deep red sky and a slow red glow, oppressive |
| Warp | 10, 15, the *Death runs* map | fast streaks of light, stars stretched, faster while the ship is boosted |
| Arena | the *Boss rush* map | dark crimson sky |

The zone comes from the map and the level of the `STATUS` (`zoneOf`). When it
changes, the screen fades to black and back in about a second.

| Boss | Aura (fades in over a few seconds) |
|---|---|
| Standard | a red vignette that pulses slowly |
| Dasher | very fast cyan streaks |
| Summoner | a violet tint and rings that expand from the right |
| Fortress | a heavy red vignette, red bars and a slight rumble |

The textures are plain png files of the `assets` folder (`bg_nebula_*.png`,
`bg_rocks.png`, `bg_girders.png`), drawn for the project.

## Sound

`Audio` plays sounds (`shoot`, `hit`, `explosion`, `boss_alert`, `victory`,
`defeat`, `coin_pickup`) and several music tracks: the level, one for each boss, and one for the death runs (zone Warp). The replica turns
what it sees into *cues* (a player bullet appeared, an entity exploded, the
boss appeared, the purse grew), and `Audio` plays them. The coin sound is
played when the number of coins of a `STATUS` is higher than the previous one,
so it is heard up to about 100 ms late, and only once if several coins are
picked up in that time. The sounds of the world received at connection are
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

- Only the ship of this player is predicted. The other entities are shown
  about 50 ms in the past, because of the interpolation, so what the player
  sees is slightly older than the server state.
- The prediction moves the ship with the same speed as the server
  (`PLAYER_SPEED`). If the server ever changes the speed of a ship (a bonus),
  the client must do the same, or it will be corrected at every `OWN_STATE`.
- A lost `STATUS` can make the coin sound late or missing.
- If the server cannot be reached, the client closes with an error message.
  There is no reconnection, and the lobby is the only menu.
- Keys cannot be remapped yet.
