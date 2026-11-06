# Custom maps: for developers

How the maps written by the players are read, checked, sent and played. The
guide for the players is [Create your own map](custom-maps.md).

- [Overview](#overview)
- [Who knows whom](#who-knows-whom)
- [The reader: `rtype::mapjson`](#the-reader-rtypemapjson)
- [From the file to the match](#from-the-file-to-the-match)
- [Client side](#client-side)
- [Server side](#server-side)
- [Records](#records)
- [Security](#security)
- [How-to](#how-to)
- [Tests](#tests)

## Overview

```
 client                                              server
 ──────                                              ──────
 CustomMaps/*.json
   │ scanMaps()  ─ parse + check each file
   ▼
 CustomMapList + LobbyMenu step "Custom"
   │ host: Enter, then the settings screen, then Enter
   │ readSelected()  ─ read the file again
   ▼
 Network::uploadMap ── MAP_UPLOAD_BEGIN / CHUNK × n / END ──►  MapUpload
                                                                 │ parseMap()  (checks again)
                                                                 ▼
                                                      buildRun(levels) ─► Segments
                                                                 │
                                       Game::startMap(segments, CUSTOM_MAP, zones)
 Replica ◄── CUSTOM_INFO (name, hash, zones) ◄───────────────────┘
 Replica ◄── MAP_UPLOAD_RESULT (ok / reason)
```

The idea that keeps it simple: a custom map is just a
`std::vector<LevelDef>`, the same list that `map1()`, `map2()` and `map3()`
return. Everything after that (`buildRun`, `Segments`, `Game`) is unchanged.

## Who knows whom

```
 rtype::game  ◄── rtype::mapjson           (JSON → LevelDef)
      ▲                 ▲
      │                 │  constants only (mapjson/Limits.hpp)
 rtype::protocol ───────┘
```

- **`rtype::game` knows nothing of the network, the window, the JSON or the
  upload.** It only gained what any map needs: `CUSTOM_MAP` (an id after the
  three maps), `customZone()`, a `zones` argument for `startMap`, and
  `CustomLeaderboard` (records, like `Leaderboard`, in plain C++).
- `rtype::mapjson` depends on `rtype::game` (it builds `LevelDef`) and on
  nlohmann-json, which stays **private** to the library: no header of
  `include/mapjson` includes it, so the client and the server do not see it.
- The messages are in `rtype/CustomMapProtocol.hpp` (next to `Protocol.hpp`).
  They use the *limits* of `mapjson/Limits.hpp`, which is a header of
  constants, not the reader.
- `server::App` is still the only place that knows both the network and the
  game; `server::CustomMaps` holds what is specific to these maps.

## The reader: `rtype::mapjson`

Sources in `src/client/game/Rtype/src/mapjson`, public headers in
`include/mapjson`.

| File | Role |
|---|---|
| `include/mapjson/CustomMap.hpp` | `CustomMap` (name, author, levels, zones), `parseMap(text)`, `contentHash(text)` |
| `include/mapjson/Folder.hpp` | `scanMaps(folder)`, `readMapFile(path, text)`, `MapEntry` |
| `include/mapjson/Limits.hpp` | every cap and range |
| `Reader.hpp/.cpp` | reads the fields of one JSON object, checks type and range, builds the error path |
| `Names.hpp` | the words of the JSON and what they mean (monsters, bosses, shapes, zones, gate walls) |
| `SpawnParser.cpp` | `parseWave`, `parseBoss` |
| `ObstacleParser.cpp` | `parseObstacle`: solid pieces, gates, portals |
| `LevelParser.cpp` | `parseLevel`: the three lists, sorted by distance |
| `MapParser.cpp` | `parseJson`, `parseMap`, `contentHash` |
| `Checks.cpp` | `checkTotals`: totals of the whole map, at least one boss |
| `Folder.cpp` | the folder scan |

`parseMap` **never throws**. Inside, a wrong value throws a `MapError` that
carries the path (`map.levels[0].waves[2].kind: ...`) and `parseMap` turns it
into the `error` of its result. The `Reader` is what keeps each parser short:

```cpp
const Reader wave(json, "map.levels[0].waves[2]");

wave.allow({"at", "kind", "count"});                  // unknown field = error
step.at = wave.number("at", {0.f, length});           // type + range checked
step.kind = wave.choice("kind", MONSTER_NAMES);       // name → enum, or error
```

Rules the parsers follow:

- Unknown fields are errors, so that typos are found.
- The lists are sorted by distance (`std::stable_sort`), because `Game` reads
  `waves`, `bossSpawn` and `obstacles` in order.
- The checks that depend on the game are written next to the field (the last
  monster of a wave on screen, the gap of a gate at least two ship heights, an
  `amplitude` with a `period`).
- Anything that depends on the whole map is in `Checks.cpp`.

## From the file to the match

1. `scanMaps` calls `parseMap` on each `.json` file. The result keeps, for
   each file, the map **or** the error, and the hash of the content.
2. The host chooses a valid map, then the settings (the settings screen of the
   lobby serves both kinds of maps and sends `SETTINGS` as usual).
   `CustomMapList::readSelected` reads the file again, so a change made after
   the scan is taken into account.
3. `Network::uploadMap` queues the text. At each `Network::update` it sends
   `MAP_UPLOAD_BEGIN` once, then at most 8 pieces of 1100 bytes
   (`MAP_UPLOAD_CHUNK`), then `MAP_UPLOAD_END`. The pacing keeps the number of
   messages waiting for an Ack far below the limit that gets a client kicked.
4. On the server, `rtype::MapUpload` (in `src/include/rtype/MapUpload.hpp`)
   rebuilds the text. At the end, `CustomMaps::finish` calls `parseMap`
   again: **the server never trusts the client**.
5. `App::startCustom` calls
   `Game::startMap(buildRun(levels), CUSTOM_MAP, zones)`, broadcasts
   `CUSTOM_INFO` and answers the host with `MAP_UPLOAD_RESULT`.

The message formats are in [the protocol](protocol.md#maps-written-by-the-players).

## Client side

| File | Role |
|---|---|
| `CustomMapList` | the maps found by `scanMaps`, the cursor, the message. It knows nothing of the network or of the drawing |
| `LobbyMenu` | which screen of the lobby is shown. Two steps are for custom maps (`Custom`, `CustomScores`), and `Settings` serves both kinds of maps (`settingsForCustom`). `back()` goes back one screen |
| `HudLobby.cpp` | draws the custom screens: tab bar, list, error of the selected map, leaderboard |
| `Network` | `uploadMap`, `requestCustomRecords`, decoding of the three new messages |
| `Replica` | `customInfo()` (name, hash, zones), `zone()` (scenery of the current level, built-in or custom), `customRecords(hash)`, `takeUploadResult()` |
| `App` | `useCustomStep`, `useCustomScoresStep`, `startCustomMap`: Tab, Right, R, Enter and the difficulty of the ranking |

The folder is `CustomMaps` **next to the executable** (`argv[0]`), like
`assets`. CMake copies `src/client/game/Rtype/CustomMaps` there after each
build. The client only draws: the zones it shows come from `CUSTOM_INFO`.

## Server side

| File | Role |
|---|---|
| `server/game/CustomMaps` | the upload (`begin`, `add`, `finish`), the map being played, the records |
| `rtype/MapUpload.hpp` | rebuilds the text from the pieces; one upload at a time, dropped at the first mistake |
| `App::handleCustom` | dispatches the four client messages |

Only the **host** may send a map; a pending upload is dropped when its owner
disconnects. A map is refused when the game is not in the lobby.

## Records

`rtype::CustomLeaderboard` keeps the five best times of each map, indexed by
the **hash** of the file (FNV-1a, 64 bits). The map is identified by its exact
content: editing it, even by one space, starts a new leaderboard. The file is
`custom_leaderboard.txt` (`hash time players` per line), in the folder where
the server runs; at most 500 maps are kept.

A client asks for the times of a map with `CUSTOM_RECORDS_GET`; after a victory
on a custom map the server sends `CUSTOM_RECORDS` to everybody.

## Security

A map comes from a player, so each step assumes a hostile file:

- the size is checked **before** reading a file (256 KiB), and again by
  `MapUpload` against the size announced, so a client cannot make the server
  keep more than that;
- the nesting of the JSON is limited (16 levels), and the numbers of items are
  capped per level and per map (`Limits.hpp`);
- every number has a range, every text a length and no control characters,
  every name is looked up in a table;
- `parseMap` and `scanMaps` do not throw: a bad file is an error message;
- the protocol decoders check sizes, ranges and zones like the others (see
  [the protocol](protocol.md#error-handling-and-security)).

## How-to

### Add a monster, a boss, a zone

Add the enum value in the game logic as usual, then one line in `Names.hpp`
(`MONSTER_NAMES`, `BOSS_NAMES`, `ZONE_NAMES`) and update the arrays' size.
Document it in [the guide](custom-maps.md).

### Add a kind of obstacle

Add it to `PIECE_NAMES` in `ObstacleParser.cpp`, and to `kindOf`. If it has
its own fields, give it a `read...` function that starts with `allow({...})`.

### Add a field

Read it in the parser of the element with the `Reader`, give it a range in
`Limits.hpp`, add it to `allow({...})`, and add a test that breaks it.

### Change the format

`version` exists for this. Only `1` is accepted today: when the format
changes, accept both values in `checkVersion` (`MapParser.cpp`), and keep the
old one working.

## Tests

| File | Covers |
|---|---|
| `tests/MapJsonTests.cpp` | the reader (valid maps, one test per kind of error, the caps), the folder scan, the two example maps, a map played by `Game` |
| `tests/CustomMapTests.cpp` | `MapUpload`, the five new messages, `CustomLeaderboard` (per difficulty, saved and loaded), `startMap` with zones |
| `tests/CustomMapListTests.cpp` | `CustomMapList` (cursor, rescan, reading of the chosen map), the steps of `LobbyMenu`, `utils::wrap` |
| `tests/ProtocolTests.cpp` | `STATUS` with the custom map id |
