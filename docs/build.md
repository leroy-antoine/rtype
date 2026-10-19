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
./build/binaries/r-type_server
./build/binaries/r-type_client
```

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

## Tests

There is no automated test suite yet (`tests/` is empty). Adding one is planned;
see the "Current status" section of the
[game logic documentation](../src/games/rtype/doc/README.md#current-status).
