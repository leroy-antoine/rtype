# Custom maps

You can play your own maps. A map is a JSON file: waves of monsters, bosses
and obstacles, cut into levels. No programming is needed.

- [Quick start](#quick-start)
- [File format](#file-format)
- [Waves](#waves)
- [Bosses](#bosses)
- [Obstacles](#obstacles)
- [Limits](#limits)
- [Errors](#errors)
- [For developers](#for-developers)

## Quick start

1. Open the `CustomMaps` folder, next to `r-type_client`. It is created when
   missing.
2. Copy `example.json` and change it.
3. Start the client. The map is read and checked; if it is wrong, you are told
   where and why (see [Errors](#errors)).

## File format

```json
{
  "name": "My map",
  "author": "Bob",
  "version": 1,
  "levels": [
    {
      "length": 2400,
      "zone": "DeepSpace",
      "deathRun": false,
      "waves": [],
      "bosses": [{"kind": "Standard", "at": 2000}],
      "obstacles": []
    }
  ]
}
```

| Field | Required | Meaning |
|---|---|---|
| `name` | yes | shown in the lobby, 40 characters at most |
| `author` | no | free text |
| `version` | no | format version, only `1` exists |
| `levels` | yes | 1 to 100 levels, played one after the other |

A level is one **segment** of the run:

| Field | Required | Meaning |
|---|---|---|
| `length` | yes | length of the level, from 100 to 100000 |
| `zone` | no | scenery: `DeepSpace` (default), `Nebula`, `Asteroids`, `Wreckage`, `Core`, `Warp`, `Arena` |
| `deathRun` | no | `true` for a level with obstacles only |
| `waves`, `bosses`, `obstacles` | no | lists, see below |

Every `at` is a distance from the **start of the level**, between `0` and the
`length` of the level. The map scrolls at 120 units per second, so `at: 1200`
happens 10 seconds after the level starts. The elements may be written in any
order, they are sorted for you.

The run is won when the **last boss is dead**: a map needs at least one boss.

## Waves

A wave sends `count` monsters of the same kind.

```json
{"at": 200, "kind": "Tank", "count": 6, "y": 250, "dy": 100,
 "interval": 0.4, "shape": "Line"}
```

| Field | Required | Meaning |
|---|---|---|
| `at` | yes | where the wave starts |
| `kind` | yes | `Basic`, `Tank`, `Kamikaze`, `Elite`, `Rocket`, `Bomber`, `MiniTank`, `MiniElite`, `MiniBomber` |
| `count` | yes | 1 to 50 |
| `y` | no | height of the first monster (0 to 1000), or `"random"` (default) |
| `dy` | no | height added at each monster, -500 to 500 (default 0) |
| `interval` | no | seconds between two monsters, 0 to 10 (default 0.5) |
| `shape` | no | `Line` (default), `Wedge`, `Column`, `Sine`; the shapes ignore `dy` |

With the `Line` shape, the last monster (`y + (count - 1) * dy`) must stay on
screen.

## Bosses

```json
{"kind": "Fortress", "at": 1500}
```

`kind` is `Standard`, `Dasher`, `Summoner` or `Fortress`.

## Obstacles

Every obstacle has a `type`. The map is 1080 units high.

**Solid pieces**: `Rock`, `Spike` (deadly), `Mine` (deadly), `Turret` (fires at
the nearest player), `Laser` (a post that fires a beam to the beat).

```json
{"type": "Rock", "at": 600, "y": 300, "width": 120, "height": 200,
 "amplitude": 80, "period": 3, "speed": 1}
```

| Field | Required | Meaning |
|---|---|---|
| `at`, `y`, `width`, `height` | yes | position and size |
| `amplitude`, `period` | no | goes up and down by `amplitude` every `period` seconds. An `amplitude` needs a `period` |
| `speed` | no | scrolling speed factor, 0.1 to 5 (default 1) |

**Gate**: two walls around a passage. The easiest way to write a course.

```json
{"type": "Gate", "at": 500, "gapY": 400, "gapHeight": 300, "width": 80,
 "wall": "Spike", "amplitude": 80, "period": 3, "pinch": false}
```

`gapHeight` must let the ship pass (96 at least) and `gapY + gapHeight` must
stay inside the map. `wall` is `Rock` (default) or `Spike`. With `pinch`, the
two walls move in opposite directions and the gap closes.

**Portals**: `ReversePortal` (inverts the controls), `FastPortal` (speeds the
ship up), `NormalPortal` (cancels the effects). A portal fills the whole
height, so it cannot be avoided.

```json
{"type": "FastPortal", "at": 900}
```

## Limits

They protect the server from files that would be too big. They are large: a
legitimate map never meets them.

| Limit | Value |
|---|---|
| file size | 256 KiB |
| levels per map | 100 |
| waves per level / bosses per level / obstacles per level | 300 / 20 / 2000 |
| monsters / bosses / obstacles per map | 20000 / 200 / 20000 |
| nesting of the JSON | 16 levels |

## Errors

A map with a mistake is **refused as a whole**. The message starts with the
path of the faulty field:

```
map.levels[0].waves[2].kind: unknown "Tonk" (expected: Basic, Tank, ...)
map.levels[1].obstacles[0].gapHeight: 50 is out of range [96, 1080]
map: needs at least one boss, the run ends when the last boss dies
```

`levels[0]` is the first level, `waves[2]` the third wave. An unknown field
(`"cout"` instead of `"count"`) is an error too, so that typos do not go
unnoticed.

## For developers

The reader is the library `rtype::mapjson`
(`src/client/game/Rtype/src/mapjson`, public headers in
`include/mapjson`). It uses [nlohmann/json](https://github.com/nlohmann/json),
installed by vcpkg.

```
JSON text -> parseMap() -> CustomMap { name, author, levels, zones }
                                  levels -> buildRun() -> Segments
```

- `parseMap(text)` never throws: it returns the map or the error message. The
  client and the server both call it on the same text, and the server must
  always do it, because a client is not trusted.
- `scanMaps(folder)` reads every `.json` file of a folder and returns, for each
  file, the map or the error, plus the hash of its content.
- One parser per element (`SpawnParser.cpp`, `ObstacleParser.cpp`,
  `LevelParser.cpp`), built on `Reader`, which checks types and ranges and
  builds the error path.
- To add a monster, a boss or an obstacle: add its name in `Names.hpp` (or in
  the table of `ObstacleParser.cpp`), and a line in this page.
- The tests are in `tests/MapJsonTests.cpp`.
