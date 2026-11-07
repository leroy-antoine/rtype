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
the list of games (the three maps below, then **LEADERBOARD**) and, under the
cursor, the three best times of the highlighted map for one difficulty. Everybody
can move their cursor with Up and Down to look at the times, and change the
difficulty of the ranking with Left and Right (brutal first, then hard,
normal and easy).

The **host** (the first player connected) presses Enter on a map to open the
[settings](#difficulty-and-settings), then Enter again to start the match;
Escape goes back to the list. The other players wait for them. Enter on
**LEADERBOARD** opens the list of every victory since the start of the game,
for any player (see [Leaderboard](#leaderboard)).

| Map | Content |
|---|---|
| **CAMPAIGN** | 20 levels with a growing difficulty, rocks and two full death runs. A real boss every five levels (Standard, Dasher, Summoner, then the Leviathan), mini-bosses in between |
| **BOSS RUSH** | 5 short levels: the Standard boss, the Dasher, the Summoner, a pause level with three mini-bosses, then the Leviathan |
| **DEATH RUNS** | 4 relentless levels of about a minute, then the Leviathan. Gates, lasers, mines and turrets are mixed in the same course, without any monster, with portals |

A **Custom map** tab (press Tab in the lobby) lists the maps written by the
players in the `CustomMaps` folder. A map with a mistake is marked and its
error is shown. The host picks a valid one with Enter, chooses the
difficulty and friendly fire like for any map, and starts it. Right opens the
leaderboard, on the page of the custom maps. See [Create your own map](custom-maps.md).

The level number is shown at the top centre of the screen.

## Controls

| Key | Action |
|---|---|
| Arrow keys | move the ship |
| Space | tap: fire one shot; hold: charge a piercing shot, released when you let go of the key |
| X | fire the secondary weapon bought in the shop (see [Weapons](#weapons)) |
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

## Weapons

### Charged shot

The base weapon on **Space** fires one shot each time you tap the key (a tap
is shorter than 0.3 s). The shot leaves when you release the key, at the pace
of the weapon cooldown. If you keep the key pressed for more than 0.3 s, a
gauge under your ship fills for up to 1.2 s. When you release it, a large
bullet goes through every enemy on its way and hits each of them once: twice
the damage at the start of the charge, six times the damage and four times
the size when the gauge is full (it turns cyan). The charge is lost when your
ship is destroyed, when the game is paused and when the shop opens.

### Secondary weapons

Three weapons are sold in the shop like augments. They go in a **secondary
slot** and are fired with **X**. Only one is equipped at a time: buying
another one replaces it, and the last one bought comes back when you respawn.

| Weapon | How it works |
|---|---|
| **Rocket launcher** | hold X: a rocket every 1.2 s, slow (half the speed of a bullet), that explodes when it passes within 60 of an enemy, on a wall or at the right edge of the map. The explosion hurts every enemy within 120 and deals three times your damage; it never hurts the players |
| **Laser ray** | hold X for 2 s while a gauge fills under your ship, release to fire a flash across the screen up to the first wall. It deals eight times your damage, once, to everything on its line (bosses too). Releasing too early cancels it for free. 6 s before the next one |
| **Chain lightning** | hold X: a bolt every 0.8 s hits the first enemy on the screen in front of your ship (within 40 of the height of the nose, up to 600 away) for one and a half times your damage, then jumps to the enemies stuck to it (100 at most between two enemies, 4 jumps), 20 % weaker at each jump. You aim by moving the ship. Without a target nothing is fired |

### Drone

The drone is a small ship that flies ahead of your ship and above it and fires
by itself at the closest enemy within 700 that is already on the screen, with half your damage. It cannot be
hit. It disappears with your ship and comes back with it when you respawn.
Three shop items, each one better and more expensive than the last, give it
a level: **Drone** (level 1) gives you the drone, **Drone Boost** (level 2)
makes it fire faster (0.45 s instead of 0.6 s) and **Drone Twin** (level 3)
also makes it fire two bullets at a time. An item gives its level at once,
whatever you bought before, and a cheaper one never lowers it. Like every
augment, an item bought by one player is no longer sold to the team.

## Difficulty and settings

Before the match, the host picks a difficulty and decides whether friendly
fire is on. The other players cannot change them. The difficulty changes the
economy, the enemies and the number of lives; nothing else.

| | Easy | Normal | Hard | Brutal |
|---|---|---|---|---|
| Lives per player | 5 | 3 | 2 | 1 |
| Enemy health (bosses too) | x0.75 | x1 | x1.5 | x2 |
| Enemy damage | x0.75 | x1 | x1.25 | x1.5 |
| Your damage | x1.25 | x1 | x0.85 | x0.75 |
| Enemy fire rate | x0.75 | x1 | x1.25 | x1.5 |
| Enemy density (size of the waves) | x0.75 | x1 | x1.25 | x1.5 |
| Coins gained | x1.25 | x1 | x0.85 | x0.75 |
| Shop quality (rarity level) | -1 | 0 | +1 | +2 |
| Offers in the shop | 3 | 3 | 3 | 4 |

- **Brutal escort**: every boss comes with mini-bosses, one for the first
  boss of the map, two for the second, three from the third on, one second
  apart. They are mini-bosses of the Brutal difficulty, so tougher than usual.
  The waves of mini-bosses are not made denser.
- **Lives**: a death costs one life. Unless it was the last one, the player
  comes back by themselves after three seconds (Enter makes it faster). A
  player without lives is eliminated; the defeat comes when nobody is alive
  and nobody can come back. Eliminated players return with one life when the
  shop opens. In a **death run**, lives do not matter: if somebody dies,
  everybody restarts the level.
- **Friendly fire**: when it is on, a shot of a player hurts the other
  players, with the normal damage of the shot. It never hurts the shooter.
- The lives are shown as small squares at the right of each health bar
  (`OUT` when a player is eliminated).

## Shop

At the end of every level (the moment the next one begins) and after the last
victory, the game **stops** and the shop opens: a few augments (the buffs, the weapons and the drone) are on sale,
more and more powerful as the levels go by, and better after a boss level.
The shop offers more augments, and rarer ones, as the difficulty rises.
Everybody can buy with the team coins (left/right to choose, Enter to buy).
Each card shows the three letters of the augment, its full name, a short text
that says what it does (with the real numbers, for example `+5 DAMAGE ON EVERY
SHOT`), its rarity and its price.
Press **Space** to say you are ready: when every player is ready a five second
countdown starts, then the next level begins (after the last victory the game
goes back to the lobby). The clock does not run while the shop is open.

## Coins, time and level skip

- Each monster that you destroy drops a coin. The coins are collected by
  touching them and go to a **purse shared by the team**, shown at the top
  right. It is kept from one level to the next.
- **Every new game starts from scratch**: after a defeat, after a victory, and
  when another map is chosen, the purse is back to **0**, the augments you
  bought are gone (and can be sold again), and the lives, health, timer and
  deaths start again. The settings (difficulty, friendly fire) stay the ones
  the host chose.
- The **timer** at the top left is the real time of the match. It stops when
  the match ends.
- Every monster you destroy also **speeds the level up**: the waves and the
  boss arrive sooner (half a second of scrolling per monster, at most 40 % of
  a level). Kill fast, finish fast. Bosses and monsters called by the Summoner
  do not count.

## Leaderboard

Every victory records its time, the number of players, the difficulty and
whether friendly fire was on. Each difficulty has its own ranking, so an easy run never pushes a brutal
one out. A solo run and a run with four players are ranked together. The
difficulty is written next to the time, in a color that goes from green (easy)
to orange, red and crimson (brutal), followed by `FF` when friendly fire was
on.

- The lobby shows the **three best times** of the highlighted map for the
  chosen difficulty (brutal at first). Left and Right change the difficulty.
- The **LEADERBOARD** entry of the list shows **every victory** since the
  start of the game for one map, fastest first. Left and Right change the
  map, Up and Down scroll, Enter or Escape goes back.
- The server keeps everything in `leaderboard.txt`, so the scores survive a
  restart.
- The leaderboard has one page for each map of the game, then a page for the
  **custom maps** (Left and Right go from page to page; Up and Down choose
  the map). Each map written by a player has its own times, kept as long as
  its file is not changed: the three best for each difficulty (see
  [Create your own map](custom-maps.md)).

## Winning and losing

- **Victory**: every boss of the map is beaten.
- **Defeat**: every player is destroyed at the same time. In a **death run**, the first death of any player sends the whole team back to the start of that level: nobody is left behind. As long as one
  teammate is alive, the others can come back with Enter.

After a defeat the same map restarts from its first level when the countdown ends. After a victory the game shows a summary (deaths, coins, time) and returns to the lobby. The time is green when it is the best of the map, red otherwise.
