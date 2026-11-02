# Gameplay

This page describes the game as a player sees it. The technical details are
in [the game logic documentation](../src/client/game/Rtype/doc/README.md).

- [Goal](#goal)
- [Lobby and maps](#lobby-and-maps)
- [Controls](#controls)
- [Enemies](#enemies)
- [Bosses](#bosses)
- [Walls](#walls)
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
| **DEATH RUNS** | 4 levels of deadly gates to cross without a single enemy, then two bosses |

The level number is shown at the top centre of the screen.

## Controls

| Key | Action |
|---|---|
| Arrow keys | move the ship |
| Space | fire (hold to keep firing) |
| Enter | come back after your ship is destroyed, if a teammate is still alive |
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

- **Rock** walls are solid grey blocks. They form caves, pillars and gates, and
  they are useful: they stop every bullet, yours and the enemies'.
- **Hazard** walls are red and black. They look like rocks but they are the
  danger of the *death runs*.

A ship that touches any wall is destroyed. Enemies fly through walls. Every
gate leaves a gap wide enough for a ship; the death runs can still be crossed
at about 60 % of the ship speed, so they are demanding but fair.

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
