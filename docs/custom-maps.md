# Create your own map

You can build your own R-Type maps and play them with your friends. A map is
a small text file: you write which monsters come, when, where the bosses
are, and which walls and traps are on the way. **You do not need to know how
to program.** If you can edit a text file, you can make a map.

- [What you need](#what-you-need)
- [Your first map in two minutes](#your-first-map-in-two-minutes)
- [Play it](#play-it)
- [How a map is built](#how-a-map-is-built)
- [Waves of monsters](#waves-of-monsters)
- [Bosses](#bosses)
- [Obstacles](#obstacles)
- [Scenery and death runs](#scenery-and-death-runs)
- [A complete example](#a-complete-example)
- [When the game says your map is wrong](#when-the-game-says-your-map-is-wrong)
- [Tips to make a good map](#tips-to-make-a-good-map)
- [Limits](#limits)
- [Reference](#reference)

## What you need

- The game (`r-type_client`, and a server to connect to).
- A text editor (Notepad, VS Code, gedit...). Not a word processor: Word adds
  hidden characters that break the file.
- The folder `CustomMaps`, **next to `r-type_client`**. The game creates it
  when it is missing. Your map files go in it.

A map file ends with `.json`. JSON is a very simple way to write data, and the
next section shows everything you need to know about it.

## Your first map in two minutes

Create a file `first.json` in the `CustomMaps` folder, with this text (it is
also the file `simple.json` that comes with the game):

```json
{
  "name": "My first map",
  "author": "Your name",
  "levels": [
    {
      "length": 2000,
      "waves": [
        {"at": 200, "kind": "Basic", "count": 5}
      ],
      "bosses": [
        {"kind": "Standard", "at": 1800}
      ]
    }
  ]
}
```

That is a complete map. Here is what each part means:

| Part | Meaning |
|---|---|
| `"name"` | the name shown in the menu (40 characters at most) |
| `"author"` | your name, shown under the map (you can leave it out) |
| `"levels"` | the list of levels of your map. Here there is one |
| `"length": 2000` | how long the level is (see [distances](#distances-and-time)) |
| `"waves"` | the groups of monsters. Here: 5 `Basic` monsters at distance 200 |
| `"bosses"` | the boss fights. Here: the `Standard` boss at distance 1800 |

The rules of JSON, to avoid the usual mistakes:

- Text is between double quotes: `"Basic"`. Numbers are not: `200`.
- `{ ... }` groups the fields of one thing, `[ ... ]` is a list.
- Things in a list or fields in a group are separated by a **comma**, but there
  is **no comma after the last one**. This is the most common mistake.
- Capital letters matter: `"Basic"` works, `"basic"` does not.

## Play it

1. Start the game and connect to the server. You must be the **host** (the
   first player connected) to start a map.
2. In the lobby, press **Tab**: the **Custom map** tab opens and lists your
   maps. A map with a mistake is marked with `!` and in red.
3. Choose a map with **Up** and **Down**. For a map with a mistake, the reason
   is written under the list (see
   [When the game says your map is wrong](#when-the-game-says-your-map-is-wrong)).
4. Press **Enter**. The game sends the map to the server, which checks it
   again and starts it for every player. The other players do not need the
   file.

| Key (Custom map tab) | Action |
|---|---|
| Up / Down | choose a map |
| Enter | start the map (host only) |
| Right | open the **Leaderboard** tab: the five best times of this map |
| Left | back to the list (from the Leaderboard tab) |
| R | read the folder again, after you edited a file |
| Tab | back to the maps of the game |

You changed your file? Press **R** in the Custom map tab and start it again.
A map that you change gets a **new, empty leaderboard**: the times are kept per
version of the file, so nobody can make a map easier and keep the old records.

## How a map is built

A map is a list of **levels**, played one after the other. In each level, the
screen scrolls to the right and things appear at the right edge when the level
reaches their *distance*.

### Distances and time

Everything is placed with a distance from the **start of its level**, written
`"at"`. The screen scrolls at **120 units per second**:

| Distance | After | Distance | After |
|---|---|---|---|
| 600 | 5 s | 2400 | 20 s |
| 1200 | 10 s | 3600 | 30 s |
| 1800 | 15 s | 7200 | 1 min |

So `"length": 2400` is a level of 20 seconds. An `"at"` can never be bigger
than the `"length"` of its level. You can list things in any order: the game
sorts them.

### The screen

The playfield is **1920 units wide and 1080 high**. A height `y` is counted
from the **top**: `0` is the top edge, `540` the middle, `1080` the bottom.

```
 y = 0     ┌──────────────────────────────────────┐
           │                                      │
 y = 540   │ ► you                    monsters ◄  │
           │                                      │
 y = 1080  └──────────────────────────────────────┘
```

Players start on the left. Monsters, bosses and obstacles come in from the
right.

### The end of a map

The map is won when the **last boss is dead**, so **a map needs at least one
boss**. After a level, the players get a short shop break, then the next level
starts.

## Waves of monsters

A wave sends several monsters of the same kind, one after the other.

```json
{"at": 700, "kind": "Tank", "count": 3, "y": 250, "dy": 250, "interval": 0}
```

| Field | Required | Meaning |
|---|---|---|
| `at` | yes | the distance where the wave starts |
| `kind` | yes | the monster, see the table below |
| `count` | yes | how many monsters, from 1 to 50 |
| `y` | no | the height of the first monster, 0 to 1000, or `"random"` (the default: each monster gets its own random height) |
| `dy` | no | how much lower each next monster is, -500 to 500 (default 0) |
| `interval` | no | seconds between two monsters, 0 to 10 (default 0.5). With `0`, they all come at the same time |
| `shape` | no | the formation, see below (default `Line`) |

With `"y": 250, "dy": 250, "count": 3`, the monsters come at heights 250, 500
and 750: a diagonal. With `"dy": 0` they all fly at the same height. The last
monster must stay on screen: `y + (count - 1) * dy` has to be between 0 and
1000, or the game refuses the map.

**Shapes.** With a shape other than `Line`, `dy` is ignored and `y` is the
height of the middle of the formation:

| `shape` | Formation |
|---|---|
| `Line` | one after the other, at the height given by `y` and `dy` |
| `Wedge` | an arrow: the leader first, the others behind in a V |
| `Column` | a vertical wall of monsters, side by side |
| `Sine` | a snake that waves up and down |

**Monsters.**

| `kind` | What it is |
|---|---|
| `Basic` | the standard enemy, shoots a fireball |
| `Tank` | slow and tough, fires heavy shells |
| `Kamikaze` | small, fragile and very fast, rams into you |
| `Elite` | fast, fires two bolts at once |
| `Rocket` | slow launcher of fast missiles, hits hard |
| `Bomber` | a big, tough monster that drops slow bombs |
| `MiniTank`, `MiniElite`, `MiniBomber` | the same monsters as "mini-bosses": three times the health, bigger, hit harder, shoot faster |

## Bosses

```json
{"kind": "Fortress", "at": 2000}
```

| `kind` | What it is |
|---|---|
| `Standard` | the classic boss: enters, stops, then moves up and down |
| `Dasher` | fast, fragile, roams around the screen |
| `Summoner` | stays back and calls reinforcements |
| `Fortress` | the Leviathan: 2400 health, fires barrages |

While a boss is alive the level stops scrolling: no new wave or obstacle comes
until it is dead. You can put several bosses in a level.

## Obstacles

Obstacles are walls and traps. Each has a `"type"`. An obstacle comes in from
the right and moves left with the scenery.

### Solid pieces

`Rock`, `Spike`, `Mine`, `Turret` and `Laser` are placed with a position and a
size:

```json
{"type": "Rock", "at": 600, "y": 300, "width": 120, "height": 200}
```

| `type` | Effect |
|---|---|
| `Rock` | a solid wall. You cannot cross it, and it pushes you: do not get crushed against the left edge |
| `Spike` | deadly on touch |
| `Mine` | deadly on touch, small (about 56 x 56), often drifting up and down |
| `Turret` | a block (about 64 x 64) that shoots at the nearest player every second |
| `Laser` | a post that turns a full-height beam on and off to the beat: it blinks for 0.4 s (harmless), burns for 1 s (deadly), then rests for 0.6 s. Use `"y": 0`, `"width": 70`, `"height": 1080` |

| Field | Required | Meaning |
|---|---|---|
| `at` | yes | the distance |
| `y`, `width`, `height` | yes | the position of the top of the piece, and its size |
| `amplitude`, `period` | no | makes it go up and down by `amplitude` units, once every `period` seconds. `amplitude` needs a `period` |
| `speed` | no | how fast it comes at you compared to the scenery, 0.1 to 5 (default 1). With `2` it crosses the screen twice as fast |

For a `Laser`, `period` is different: it shifts the rhythm, in seconds. Two
lasers with the same `period` blink together, with `0` and `1` they alternate.

### Gates

A **gate** is two walls with a passage between them: the easiest way to make a
course. You only give the passage.

```json
{"type": "Gate", "at": 500, "gapY": 400, "gapHeight": 300, "width": 80}
```

| Field | Required | Meaning |
|---|---|---|
| `at` | yes | the distance |
| `gapY` | yes | the height of the top of the passage |
| `gapHeight` | yes | the height of the passage, at least 96 so that the ship can pass |
| `width` | yes | the thickness of the walls |
| `wall` | no | `"Rock"` (default) or `"Spike"` (deadly) |
| `amplitude`, `period` | no | the passage goes up and down |
| `pinch` | no | `true`: the two walls move in opposite directions, so the passage closes and opens |
| `speed` | no | as above |

`gapY + gapHeight` cannot be more than 1080: the passage must be inside the
screen.

### Portals

A portal fills the whole height of the screen, so you cannot avoid it. It
changes the ship of every player that crosses it:

```json
{"type": "FastPortal", "at": 900}
```

| `type` | Effect |
|---|---|
| `ReversePortal` | inverts the controls (crossing a second one gives them back) |
| `FastPortal` | the ship goes 1.5 times faster |
| `NormalPortal` | back to normal |

A portal has `at` and, if you want, `speed`.

## Scenery and death runs

Each level can have its own scenery (and music) with `"zone"`:

| `zone` | Look |
|---|---|
| `DeepSpace` | the default, open space |
| `Nebula` | a nebula, with meteors falling between the waves |
| `Asteroids` | a field of asteroids |
| `Wreckage` | the wreck of a fleet |
| `Core` | the inside of an alien base |
| `Warp` | a tunnel of light, made for death runs |
| `Arena` | a boss arena |

A level with `"deathRun": true` is a **death run**: a course of obstacles
where the first death of any player puts the whole team back at the start of
the level. Do not put monsters in it, and do not make it too long.

## A complete example

This is the file `example.json` that comes with the game. It has four levels:
a normal level, a level with lasers and a turret, a death run, and a final
boss. Copy it and change it step by step.

```json
{
  "name": "Example map",
  "author": "R-Type team",
  "version": 1,
  "levels": [
    {
      "length": 2600,
      "zone": "DeepSpace",
      "waves": [
        {"at": 200, "kind": "Basic", "count": 5, "y": "random",
         "interval": 0.5},
        {"at": 700, "kind": "Tank", "count": 3, "y": 250, "dy": 250,
         "interval": 0},
        {"at": 1300, "kind": "Elite", "count": 6, "shape": "Wedge",
         "y": 540}
      ],
      "obstacles": [
        {"type": "Gate", "at": 500, "gapY": 400, "gapHeight": 300,
         "width": 80},
        {"type": "FastPortal", "at": 900},
        {"type": "Gate", "at": 1100, "gapY": 300, "gapHeight": 260,
         "width": 80, "wall": "Spike", "amplitude": 80, "period": 3},
        {"type": "Rock", "at": 1600, "y": 300, "width": 120,
         "height": 200}
      ],
      "bosses": [
        {"kind": "Standard", "at": 2300}
      ]
    },
    {
      "length": 2400,
      "zone": "Nebula",
      "waves": [
        {"at": 200, "kind": "Kamikaze", "count": 8, "interval": 0.2},
        {"at": 800, "kind": "Rocket", "count": 4, "y": 150, "dy": 250,
         "interval": 0}
      ],
      "obstacles": [
        {"type": "Laser", "at": 600, "y": 0, "width": 70, "height": 1080,
         "period": 0},
        {"type": "Laser", "at": 900, "y": 0, "width": 70, "height": 1080,
         "period": 1},
        {"type": "Turret", "at": 1200, "y": 0, "width": 64, "height": 64},
        {"type": "Mine", "at": 1500, "y": 500, "width": 56, "height": 56,
         "amplitude": 150, "period": 3}
      ],
      "bosses": [
        {"kind": "Dasher", "at": 2100}
      ]
    },
    {
      "length": 2000,
      "zone": "Warp",
      "deathRun": true,
      "obstacles": [
        {"type": "ReversePortal", "at": 200},
        {"type": "Gate", "at": 500, "gapY": 400, "gapHeight": 200,
         "width": 80, "amplitude": 70, "period": 2.4},
        {"type": "NormalPortal", "at": 800},
        {"type": "Gate", "at": 1000, "gapY": 300, "gapHeight": 200,
         "width": 80, "amplitude": 70, "period": 1.5, "pinch": true},
        {"type": "Gate", "at": 1500, "gapY": 500, "gapHeight": 220,
         "width": 80}
      ]
    },
    {
      "length": 2400,
      "zone": "Arena",
      "waves": [
        {"at": 200, "kind": "Bomber", "count": 3, "y": 150, "dy": 300,
         "interval": 0},
        {"at": 900, "kind": "MiniTank", "count": 1, "y": 300}
      ],
      "bosses": [
        {"kind": "Fortress", "at": 2000}
      ]
    }
  ]
}
```

## When the game says your map is wrong

A map with a mistake is **refused as a whole**, and the game tells you where.
The message starts with the **path** of the faulty field:

```
map.levels[0].waves[2].kind: unknown "Tonk" (expected: Basic, Tank, ...)
```

Read it from left to right: in the map, `levels[0]` is the **first** level
(counting starts at 0, so `[1]` is the second), `waves[2]` is its **third**
wave, and its `kind` is wrong: `"Tonk"` is not a monster.

| Message | What to do |
|---|---|
| `not valid JSON, error near byte N` | a comma, a quote or a bracket is missing or in excess, near the N-th character of the file. Look for a comma after the last item |
| `missing field` | a required field is not there: add it |
| `unknown field "cout"` | a typo in a field name (`count`?). Fields are not ignored, so mistakes show up |
| `unknown "X" (expected: ...)` | a name that does not exist. The list tells you the right ones, with the right capital letters |
| `N is out of range [a, b]` | the number is too small or too big: keep it between `a` and `b` |
| `expected a number` / `an integer` / `a string` | the type is wrong: `"200"` instead of `200`, or `2.5` monsters |
| `the last monster would be at y = ...` | `y + (count - 1) * dy` leaves the screen: reduce `dy` or `count` |
| `the gap goes below the map` | `gapY + gapHeight` is more than 1080 |
| `amplitude needs a period greater than 0` | add a `period` next to the `amplitude` |
| `needs at least one boss` | add a boss: it is how the map ends |
| `a map needs at least one level` | the `levels` list is empty |
| `file bigger than 256 KiB` / `more than N ...` | see [Limits](#limits) |

When you fix the file, press **R** in the Custom map tab to read it again.

## Tips to make a good map

- **Start small.** Begin with `simple.json`, play it, add one thing, play it
  again. A big file that was never played is full of surprises.
- **Test every gate.** Fly through the passage yourself. 96 is the minimum:
  with four players, 200 or more is kinder, and the first levels should be
  generous.
- **Leave time to breathe.** A good level has quiet moments between the
  difficult ones. Space your waves by 400 to 600 units.
- **Teach one idea at a time.** A first gate, then a moving gate, then a gate
  with spikes. A portal is a surprise: leave a quiet stretch after it.
- **Give a place to a boss.** Nothing comes while a boss is alive, but what is
  already on screen is still there: end the walls well before the boss.
- **Check the wave heights.** Monsters at `y` 0 or 1000 are at the very edge
  of the screen. The middle (300 to 800) is the comfortable area.

## Limits

They protect the server from files that would be too big, and they are large:
a real map never meets them.

| Limit | Value |
|---|---|
| size of the file | 256 KiB |
| levels per map | 100 |
| waves, bosses, obstacles per level | 300, 20, 2000 |
| monsters, bosses, obstacles per map | 20000, 200, 20000 |
| name | 40 characters: use letters, digits and simple signs, the menu font has no accents |

## Reference

### Map

| Field | Required | Type | Meaning |
|---|---|---|---|
| `name` | yes | text | name of the map |
| `author` | no | text | your name |
| `version` | no | number | the format, always `1` |
| `levels` | yes | list | the levels |

### Level

| Field | Required | Type | Meaning |
|---|---|---|---|
| `length` | yes | number, 100 to 100000 | length of the level |
| `zone` | no | text | the scenery (default `DeepSpace`) |
| `deathRun` | no | `true` / `false` | a death run (default `false`) |
| `waves` | no | list | the [waves](#waves-of-monsters) |
| `bosses` | no | list | the [bosses](#bosses) |
| `obstacles` | no | list | the [obstacles](#obstacles) |

### Obstacle ranges

| Field | Range |
|---|---|
| `y` | -1080 to 2160 |
| `width` | 1 to 2000 |
| `height` | 1 to 2160 |
| `gapY` | 0 to 1080 |
| `gapHeight` | 96 to 1080 |
| `amplitude` | -1080 to 1080 |
| `period` | 0 to 60 |
| `speed` | 0.1 to 5 |

For how the reader works, how to add a new kind of monster or obstacle, and
the protocol, see [custom maps, for developers](custom-maps-internals.md).
