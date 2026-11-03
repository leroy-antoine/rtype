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
| **CAMPAIGN** | 20 levels with a growing difficulty, more and more bosses, rocks and two full death runs |
| **BOSS RUSH** | 5 short levels, one to three bosses each, ending with the Fortress, the Summoner and the Dasher |
| **DEATH RUNS** | 4 relentless levels of spike gates, with portals, without a single enemy, then two bosses |

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

## Bosses

| Boss | What it does |
|---|---|
| Standard | stops against the right edge, sways up and down and fires a fan of bullets |
| Dasher | a small alien that roams the whole right side of the screen and fires a wide fan of shards |
| Summoner | sways and calls three Kamikazes every few seconds (at most six at once) |
| Fortress | huge and slow; alternates calm phases with two-second barrages |

The level stops scrolling while a boss is alive, and its health bar is shown
at the bottom of the screen. Monsters called by the Summoner drop no coin.

## Walls

Walls scroll with the background.

- **Rock** walls are solid asteroids. Your ship cannot cross them: they block
  and push it. They form caves, pillars and gates, they stop every bullet,
  yours and the enemies', and you can hide behind them. But a rock keeps
  coming from the right: if it pushes your ship against the left edge of the
  screen, the ship is **crushed**.
- **Spikes** are steel stars with glowing red tips. Touching one **destroys**
  your ship at once. The gates of the *death runs* are made of spikes.
- Some gates **swing** up and down while they come: follow the gap.
- Some gates **pinch**: the two walls close on the gap and open again.
- **Sweepers** are spike blocks that sweep up and down very fast between two
  gates: time your move.
- In the death runs the walls come **faster than the scrolling** (up to twice
  as fast) and follow each other closely.

Enemies fly through walls. Every gate leaves a gap wide enough for a ship.

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
run can be crossed, but only by a player who keeps moving: the gates of the
last levels of the *Death runs* map need a ship at least 1.2 times faster than
the normal speed, which is what the fast portal gives.

## Music and scenery

Each boss has its own music (a march for the Standard boss, a frantic
arpeggio for the Dasher, an eerie drone for the Summoner, a heavy slow beat for
the Fortress), the death runs have a driving track of their own, and the
levels share a lighter one.

The scenery changes with the part of the map: deep blue space, a purple nebula,
an asteroid belt, the wreckage of a station, the red heart of the Bydo core,
and streaks of light in the warp of the death runs. While a boss is on screen
the scenery takes its colour: a pulsing red glow for the Standard boss, fast
cyan streaks for the Dasher, expanding violet rings for the Summoner, a heavy
red vignette and a rumble for the Fortress. Nothing flashes.

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
- **Defeat**: every player is destroyed at the same time. As long as one
  teammate is alive, the others can come back with Enter.

After the end screen, the game returns to the lobby.
