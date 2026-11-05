# Gameplay

This page describes the game as a player sees it. The technical details are
in [the game logic documentation](../src/client/game/Rtype/doc/README.md).

- [Goal](#goal)
- [Lobby and maps](#lobby-and-maps)
- [Controls](#controls)
- [Enemies](#enemies)
- [Bosses](#bosses)
- [Walls](#walls)
- [Portals](#portals)
- [Music and scenery](#music-and-scenery)
- [Coins, time and level skip](#coins-time-and-level-skip)
- [Leaderboard](#leaderboard)
- [Winning and losing](#winning-and-losing)

## Goal

Up to four players fly together from the left of the screen, shoot the waves
of monsters that come from the right, beat the bosses and reach the end of the
map as fast as possible.

## Lobby and maps

When the server starts, and after every match, the game shows the **lobby**:
the list of maps and, under the cursor, the five best times of the highlighted
map. Everybody can move their cursor with Up and Down to look at the times.
The **host** (the first player connected) presses Enter to start the match on
the highlighted map; the other players wait for them.

| Map | Content |
|---|---|
| **CAMPAIGN** | 20 levels with a growing difficulty, rocks and two full death runs. A real boss every five levels (Standard, Dasher, Summoner, then the Leviathan), mini-bosses in between |
| **BOSS RUSH** | 5 short levels: the Standard boss, the Dasher, the Summoner, a pause level with three mini-bosses, then the Leviathan |
| **DEATH RUNS** | 4 relentless levels of about a minute, then the Leviathan. Gates, lasers, mines and turrets are mixed in the same course, without any monster, with portals |

The level number is shown at the top centre of the screen.

## Controls

| Key | Action |
|---|---|
| Arrow keys | move the ship |
| Space | fire (hold to keep firing) |
| Enter | come back after your ship is destroyed, if a teammate is still alive |
| Backspace | host only: pause the match and show the pause menu; again to resume |
| Enter (paused) | host only: leave the match and go back to the map menu |
| Escape | quit |

Leaving the screen with your ship, or touching an enemy, destroys it.

## Enemies

| Enemy | What it does |
|---|---|
| Basic | flies straight and shoots fireballs |
| Tank | slow, a lot of health, fires large shells |
| Kamikaze | flies straight, then locks on the nearest player and dives at full speed. It does not turn: move aside |
| Elite | zigzags up and down and fires two fast bolts |
| Rocket | stops for a moment before it fires a fast missile: that pause is your warning |
| Bomber | tall and slow; its bombs fall towards the player who is nearest |

## Zones

The campaign has four moods of five levels, one per boss, plus a warp for the
death runs (10 and 15). Each mood has its own scenery, music, formations and
hazard:

| Levels | Zone | Music | Waves | Hazard |
|---|---|---|---|---|
| 1-5 | Deep space | calm 120 BPM loop | straight lines | rocks and a few lasers |
| 6-9 | Nebula | slow 90 BPM pads | snakes and arrowheads | meteors fall between the waves |
| 11-14 | Wreckage | heavy 108 BPM industrial beat | walls and arrowheads | mines and turrets between the rocks |
| 16-20 | Core | fast 144 BPM | arrowheads, walls and snakes | five lasers per level |

A wave can be a line, an **arrowhead** (the leader first), a **wall** (all at
once, with gaps) or a **snake** that undulates. Tanks, Bombers and mini-bosses
keep their own placement.

## Bosses

| Boss | What it does |
|---|---|
| Standard | stops against the right edge, sways up and down and fires a fan of bullets |
| Dasher | a small alien that roams the whole right side of the screen and fires a wide fan of shards |
| Summoner | sways and calls three Kamikazes every few seconds (at most six at once) |
| Fortress (the Leviathan) | the final boss, 2400 HP and **immune to collisions** (ramming it only kills you). It sways fast, and chains four moves: **lightning columns** (red zones, then four to six vertical beams), a **charge** (it aims at you, a red lane shows its path, then it rushes across the screen), a **sweeping beam** (two walls of beams with a moving gap you must follow) and another charge. Every attack is announced. Under two thirds and one third of its health it gets faster, rests less and adds columns |

A type of boss appears only once in a map. **Mini-bosses** fill the levels between them: a Tank, an Elite or a Bomber with three times the health, 1.6 times the size, more damage and an extra shot. They are ordinary monsters: the level keeps scrolling.

The level stops scrolling while a boss is alive, and its health bar is shown
at the bottom of the screen. Monsters called by the Summoner drop no coin.

**The arena is dangerous.** While a boss is alive, lasers cross the screen on the beat and **meteors** (small fast balls with a red trail) fly diagonally across it. A meteor hurts the player it touches (20 damage) and explodes on any obstacle. The fight gets denser with every boss of the map: the last boss brings a laser about every 3 s and a meteor about every half second.

## Walls

Walls scroll with the background.

- **Rock** walls are solid asteroids. Your ship cannot cross them: they block
  and push it. They form caves, pillars and gates, they stop every bullet,
  yours and the enemies', and you can hide behind them. But a rock keeps
  coming from the right: if it pushes your ship against the left edge of the
  screen, the ship is **crushed**.
- **Spikes** are steel stars with glowing red tips. Touching one **destroys**
  your ship at once. The gates of the *death runs* are made of spikes.
- **Mines** are small dark balls with a red light; touching one destroys the ship. They often drift together up and down.
- **Lasers** are full-height beams that switch on and off on the beat (120 BPM). A blinking red column warns you for about half a second, then the beam is deadly for one second: cross while it is off.
- **Turrets** are fixed on the top and bottom edges and shoot at the nearest player every second. They cannot be destroyed.
- Some gates **swing** up and down while they come: follow the gap.
- Some gates **pinch**: the two walls close on the gap and open again.
- **Sweepers** are spike blocks that sweep up and down very fast between two
  gates: time your move.
- In the death runs the walls come **faster than the scrolling** (up to twice
  as fast) and follow each other closely.

In the normal levels of the campaign, about ten obstacles are spread along the whole level: asteroids (some of them fly faster than the scrolling and drift up and down) and, from the third level on, lasers. Lasers only hurt the players: they do not stop the bullets, and the monsters ignore them.

Enemies fly through walls, but they never *appear* inside one: a monster that would spawn in a wall is moved to the nearest free height, or waits a moment. Every gate leaves a gap wide enough for a ship.

## Portals

A portal is a glowing column across the whole screen: you cannot avoid it.
They appear in the death runs.

| Portal | Effect |
|---|---|
| Cyan, `>>` | **speed up**: the ship moves 1.5 times faster |
| Violet, `<>` | **reversed controls**: all four arrow keys are inverted, until the next violet portal |
| Green, `=` | back to the normal speed |

The effects stop at the end of the level. The text `CONTROLS REVERSED` or
`SPEED UP` reminds you of them at the bottom left of the screen. Every death
run is meant to be crossed by a player who keeps moving, with the speed that
the fast portal at its start gives.

## Music and scenery

Each boss has its own music (a march for the Standard boss, the
Dasher theme, the Summoner theme, a heavy slow beat for the Fortress), the death runs have a driving track of their own at 120 BPM, the tempo of the lasers, and the
levels share a lighter one.

The scenery changes with the part of the map: deep blue space, a purple nebula,
an asteroid belt, the wreckage of a station, the red heart of the Bydo core,
and streaks of light in the warp of the death runs. While a boss is on screen
the scenery takes its colour: a pulsing red glow for the Standard boss, fast
cyan streaks for the Dasher, a violet tint for the Summoner, a heavy
red vignette and a rumble for the Fortress. The Leviathan shakes for a quarter of a second every time it is hit. Nothing flashes.

## Shop

At the end of every level (the moment the next one begins) the game **stops**
and the shop opens: a few augments are on sale, more and more powerful as the
levels go by, and better after a boss level. Everybody can buy with the team
coins (left/right to choose, Enter to buy). The host presses **Tab** to leave
the shop and start the next level. The clock does not run while the shop is
open. There is no shop after the last level (the victory screen comes
instead).

## Coins, time and level skip

- Each monster that you destroy drops a coin. The coins are collected by
  touching them and go to a **purse shared by the team**, shown at the top
  right.
- The **timer** at the top left is the real time of the match. It stops when
  the match ends.
- Every monster you destroy also **speeds the level up**: the waves and the
  boss arrive sooner (half a second of scrolling per monster, at most 40 % of
  a level). Kill fast, finish fast. Bosses and monsters called by the Summoner
  do not count.

## Leaderboard

Every victory records its time and the number of players. The server keeps the
five best times of each map and shows them in the lobby. A solo run and a run
with four players are listed together, with the number of players next to the
time.

## Winning and losing

- **Victory**: every boss of the map is beaten.
- **Defeat**: every player is destroyed at the same time. In a **death run**, the first death of any player sends the whole team back to the start of that level: nobody is left behind. As long as one
  teammate is alive, the others can come back with Enter.

After a defeat the same map restarts from its first level when the countdown ends. After a victory the game shows a summary (deaths, coins, time) and returns to the lobby. The time is green when it is the best of the map, red otherwise.
