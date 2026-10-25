# R-Type

A networked clone of the arcade shoot'em up **R-Type**, written in C++20 for the
Epitech R-Type project. Several players, each with their own client, connect to
a central server that has the final authority on the game.

The project is built around a small game engine (an ECS and a window module) and
three decoupled layers: **rendering**, **networking** and **game logic**.

> Status: Part 1 (architecture and first prototype) is playable. Up to four
> players, each with their own client, fight waves of monsters and a boss on an
> authoritative server. See each part's documentation for the details.

## Requirements

- C++20 compiler (GCC, Clang or MSVC)
- CMake 3.25 or newer
- Git
- Linux or Windows

Dependencies (Asio, SFML 3) are installed by vcpkg, which is a submodule of this
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
another machine. Controls: arrow keys to move, Space to fire, Enter to come
back after a death, Escape to quit.

## Repository layout

| Folder | Content |
|---|---|
| `engine/core` | ECS and fixed time step |
| `engine/window` | window, events, textures, renderer |
| `engine/network` | UDP transport: sessions, ping, reliability |
| `engine/audio` | sounds and music |
| `src/include` | headers shared by all parts |
| `src/client/game/Rtype` | the game logic and the client game code (and the sprites in `assets/`) |
| `src/server` | the authoritative server |
| `src/client` | the display |
| `docs` | documentation that concerns the whole project |
| `tests` | automated tests of the game logic and of the protocol |

## Documentation

Each part keeps its documentation next to its code:

| Part | Documentation |
|---|---|
| Game logic | [src/client/game/Rtype/doc](src/client/game/Rtype/doc/README.md): rules, architecture, systems, the `Game` API, how-to guides |
| Server | [src/server/doc](src/server/doc/README.md): role, loop, how it plugs the game in |
| Client | [src/client/doc](src/client/doc/README.md): rendering, input, planned architecture |

Documentation that concerns the whole project is in [docs](docs/README.md):

- [Architecture](docs/architecture.md)
- [Build and run](docs/build.md)
- [Network protocol](docs/protocol.md)
- [Technical and comparative study](docs/comparative-study.md)
- [Accessibility](docs/accessibility.md)
- [Contributing](docs/contributing.md)

## Authors

Thiago Semedo-Rocha, Antoine Leroy, Maxime Kournikov.

## License

No license has been chosen yet.
