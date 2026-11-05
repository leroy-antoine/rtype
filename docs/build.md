# Build and run

## Requirements

- A C++20 compiler (GCC, Clang or MSVC)
- CMake 3.25 or newer
- Git (the dependency manager is a submodule)
- Platform libraries needed by SFML. On Linux that means the X11 / OpenGL /
  audio development packages; vcpkg prints the exact missing packages when a
  build fails.

Dependencies (`vcpkg.json`): **Asio**, **SFML 3**, installed by vcpkg. vcpkg
itself is the submodule `external/vcpkg`. Nothing is copied into the repository.

Supported platforms: Linux (preset `linux-debug`) and Windows
(preset `windows-debug`).

## Get the sources

```
git clone --recurse-submodules git@github.com:EpitechPGE3-2026/G-ING-500-PAR-5-1-rtype-6.git
```

If you cloned without `--recurse-submodules`:

```
git submodule update --init
```

## Build on Linux

```
./external/vcpkg/bootstrap-vcpkg.sh -disableMetrics
cmake --preset linux-debug
cmake --build --preset linux-debug
```

## Build on Windows

```
.\external\vcpkg\bootstrap-vcpkg.bat
cmake --preset windows-debug
cmake --build --preset windows-debug
```

The first configure downloads and builds the dependencies, so it takes a while.
The next ones are fast.

## Run

The executables are written to `build/binaries`:

```
./build/binaries/r-type_server <port>
./build/binaries/r-type_client <host> <port>
```

Start the server first, then one client per player (up to four). The server
listens in **UDP** on every IPv4 interface. To play from another machine, give
the client the IP address of the server and open the port in UDP in the
firewall (for example `sudo ufw allow 4242/udp`).

A client needs the `assets` folder that CMake copies next to its executable.

## IDE setup (CLion)

Open the **root** `CMakeLists.txt` (not one of the sub-folders), then either use
the `linux-debug` preset, or add this CMake option to the profile:

```
-DCMAKE_TOOLCHAIN_FILE=<repo>/external/vcpkg/scripts/buildsystems/vcpkg.cmake
```

After changing the profile, reset the CMake cache.

## Troubleshooting

| Message | Cause and fix |
|---|---|
| `vcpkg submodule is missing` | run `git submodule update --init`, then configure again |
| `No project() command is present` | a sub-folder `CMakeLists.txt` was loaded as the root; open the root one |
| `SFML ... version 3 required` / `found 2.x` | the toolchain file is not set, so the system SFML was used; use the preset or the option above |
| `vcpkg` not found | the bootstrap script was not run, or its name was mistyped |
| odd errors after changing the profile | reset the CMake cache and reconfigure |
| `CMake Error: ... Ninja` or an empty `build/binaries` | install Ninja (`sudo apt install ninja-build`), the Linux preset uses it |
| `Makefile`, `CMakeCache.txt` or `CMakeFiles/` appear at the root | `cmake .` was run in the source tree. Always configure with `cmake --preset linux-debug`, and remove those files |
| the client prints `server unreachable` | wrong IP or port, the server is not running, or the port is not open in UDP |

## Tests

The tests cover the game logic (waves, bosses, monster behaviours, walls,
coins, level skip, victory and defeat, respawn, the lobby, the events sent to
the clients), the three maps (ordering, reachability of the gates, bosses),
the leaderboard and the encoding of the protocol messages (round trips,
truncated or invalid payloads). They are built with the rest of the project:

```
./build/binaries/rtype_tests
```

or `ctest --test-dir build/linux-debug`. The server loop, the client and the
transport layer are not covered by automated tests yet.
