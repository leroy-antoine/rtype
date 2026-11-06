# R-Type

A networked clone of the arcade shoot'em up **R-Type**, written in C++20 for the
Epitech R-Type project. Several players, each with their own client, connect to
a central server that has the final authority on the game.

The project is built around a small game engine (an ECS and a window module) and
three decoupled layers: **rendering**, **networking** and **game logic**.

## Features

- **Up to four players**, each with their own client, on a server that has the
  final authority. The clients only send their keys.
- **Three maps**, chosen in a lobby by the host: a 20-level *campaign*, a
  *boss rush* and *death runs*.
- **Six kinds of monsters** with their own behaviour (diving kamikazes,
  zigzagging elites, rockets that charge, bombers that drop bombs) and **four
  bosses** (a standard boss, a roaming Dasher, a Summoner that calls
  reinforcements and a Fortress that fires barrages).
- **Your own maps**: write a map in a JSON file (waves, bosses, obstacles,
  levels), pick it in the lobby, play it and compete for its leaderboard. See
  [Create your own map](docs/custom-maps.md).
- **Obstacles**: solid rocks to hide behind, deadly spikes, swinging gates, and portals that speed the ship up or reverse the controls.
- **Music and scenery** that change with every boss and every zone of the map.
- **Coins** shared by the team, a match **timer**, and a **leaderboard** with
  the best times of each map.
- A custom **engine**: ECS, window, audio and a reliable UDP transport,
  documented and unit tested.

See [Gameplay](docs/gameplay.md) for the rules as a player sees them.

## Requirements

- C++20 compiler (GCC, Clang or MSVC)
- CMake 3.25 or newer
- Git
- Linux or Windows

Dependencies (Asio, SFML 3, nlohmann-json) are installed by vcpkg, which is a submodule of this
repository.

## Quick start

```
git clone --recurse-submodules git@github.com:EpitechPGE3-2026/G-ING-500-PAR-5-1-rtype-6.git
cd G-ING-500-PAR-5-1-rtype-6
./external/vcpkg/bootstrap-vcpkg.sh -disableMetrics
cmake --preset linux-debug
cmake --build --preset linux-debug
```

The executables are written to `build/binaries`: `r-type_server` and
`r-type_client`. On Windows, or for IDE setup and troubleshooting, see
[Build and run](docs/build.md).

## Play

```
./build/binaries/r-type_server 4242
./build/binaries/r-type_client <server ip> 4242
```

The server uses **UDP**: open the port in UDP in the firewall to play from
another machine.

| Key | Action |
|---|---|
| Arrow keys | move the ship (in the lobby, Up and Down choose a map) |
| Tab | in the lobby, next tab: **Custom map** (your own maps), then its **Leaderboard** |
| Space | fire (hold to keep firing) |
| Enter | come back after a death; in the lobby, start the map (host only) |
| Escape | quit |

The first player connected is the *host*: they choose the map in the lobby.
After a victory or a defeat, the game returns to the lobby.

## Repository layout

| Folder | Content |
|---|---|
| `engine/core` | ECS and fixed time step |
| `engine/window` | window, events, textures, renderer |
| `engine/network` | UDP transport: sessions, ping, reliability |
| `engine/audio` | sounds and music |
| `src/include` | headers shared by all parts |
| `src/client/game/Rtype` | the game logic and the client game code (and the sprites in `assets/`) |
| `src/client/game/Rtype/CustomMaps` | example maps in JSON, copied next to the client: put yours there |
| `src/server` | the authoritative server |
| `src/client` | the display |
| `docs` | documentation that concerns the whole project |
| `tests` | automated tests of the game logic, the maps, the custom maps, the leaderboard and the protocol |

## Documentation

Each part keeps its documentation next to its code:

| Part | Documentation |
|---|---|
| Game logic | [src/client/game/Rtype/doc](src/client/game/Rtype/doc/README.md): rules, architecture, systems, the `Game` API, how-to guides |
| Server | [src/server/doc](src/server/doc/README.md): role, loop, how it plugs the game in |
| Client | [src/client/doc](src/client/doc/README.md): rendering, input, interpolation and prediction, lobby screen |

Documentation that concerns the whole project is in [docs](docs/README.md):

- [Gameplay](docs/gameplay.md)
- [Architecture](docs/architecture.md)
- [Build and run](docs/build.md)
- [Create your own map](docs/custom-maps.md) (players) and
  [custom maps for developers](docs/custom-maps-internals.md)
- [Network protocol](docs/protocol.md)
- [Technical and comparative study](docs/comparative-study.md)
- [Accessibility](docs/accessibility.md)
- [Contributing](docs/contributing.md)

## Tests

```
cmake --build --preset linux-debug
./build/binaries/rtype_tests
```

The suite covers the game rules, the 3 maps, the leaderboard and the encoding
of every network message. See [Build and run](docs/build.md#tests).

## Authors

Thiago Semedo-Rocha, Antoine Leroy, Maxime Kournikov.

## License

This project is released under the **MIT License**.

```
MIT License

Copyright (c) 2026 The R-Type team (Thiago Semedo-Rocha, Antoine Leroy,
Maxime Kournikov)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

The sprites in `src/client/game/Rtype/assets` come from the original R-Type
sprite sheets and belong to their rights holders; they are used here for an
educational project only.
