# R-Type game logic

Technical documentation of the **game logic** (`src/client/game/Rtype`).

This part decides *what happens in the game*: where entities are, who shoots,
who gets hit, who dies. It knows nothing about the network, the window, the
sprites or the sound. It is a plain C++ library built on top of the ECS of
`engine/core`.

- [Overview](#overview)
- [Architecture](#architecture)
- [Game rules](#game-rules)
- [Kinds of monsters and bosses](#kinds-of-monsters-and-bosses)
- [Obstacles](#obstacles)
- [Maps and lobby](#maps-and-lobby)
- [The first map: 20 levels](#the-first-map-20-levels)
- [Level skip](#level-skip)
- [The tick: `Game::update`](#the-tick-gameupdate)
- [Public API: the `Game` class](#public-api-the-game-class)
- [Components](#components)
- [Systems](#systems)
- [Configuration](#configuration)
- [How-to](#how-to)
- [Coding conventions](#coding-conventions)
- [Current status](#current-status)

## Overview

The game logic is **server-authoritative**: clients only send the state of
their keys, the server runs the simulation and decides what is true.

```
 client keys ──► network ──► main ──► Game::setInput ┐
                                                     ▼
                              main ──► Game::update(dt)  (60 times / second)
                                                     │
 clients  ◄── network ◄── main ◄── state of the world┘
```

The logic never reads a clock: the caller gives it a fixed time step `dt`
(`config::FIXED_DT`, 1/60 s). The same inputs always produce the same game,
which makes the logic easy to test and to debug.

The server and the engine do not know R-Type. The executable that glues them
together (the `main`) is the only place that includes `rtype::game`.

## Architecture

### Data oriented design

The logic follows the ECS (Entity Component System) pattern:

| Concept | In this project | Rule |
|---|---|---|
| Entity | an id created by `Engine::Registry` | has no data of its own |
| Component | a small struct (`Health`, `Weapon`, ...) | **pure data**, no behaviour |
| System | a free function taking the `Registry` | holds the behaviour, owns no state |
| Factory | `createPlayer`, `createMonster`, `createBullet` | assembles an entity from components |

A player is not a `Player` class: it is an entity that happens to carry
`PlayerTag`, `PlayerInput`, `Vec2Pos`, `Health`, `Weapon`, ... Systems then act
on *every entity that has the components they ask for*. Adding a behaviour
means adding a component and a system, not editing a class hierarchy.

### Layers

Headers only include the layers below them, which avoids circular includes.

```
                       Game.hpp            facade, owns the Registry
                          │
        Monsters.hpp   Systems.hpp         behaviours (one function = one rule)
                          │
                      Queries.hpp          "give me all the players", slots...
                          │
                     Colisions.hpp         colliders, overlaps, bounds
                          │
                      Damages.hpp          applyDamages(victim, attacker)
                          │
       Factories.hpp  Movements.hpp  Bullet.hpp
                          │
              Components.hpp   Config.hpp   utils/Vectors.hpp
```

| File | Role |
|---|---|
| `Config.hpp` | every tunable constant (sizes, speeds, hp, cooldowns) |
| `Components.hpp` | the component structs |
| `utils/Vectors.hpp` (`src/include`) | `Vec2` and its typed variants, `length`, `normalize` |
| `Movements.hpp` | `inputDirection` (keys to direction), `calculatePos` |
| `Bullet.hpp` | helpers for bullets: `isShooter`, `projectileOf`, `bulletStartPos` |
| `Factories.hpp` | `createPlayer`, `createMonster`, `createBullet` |
| `Damages.hpp` | `applyDamages` |
| `Colisions.hpp` | `Collider`, `overlaps`, `canHit`, bounds tests |
| `Queries.hpp` | read-only searches in the world (`collectWith<Tag>`, `findFreeSlot`, ...) |
| `Systems.hpp` | the systems (see below) |
| `Monsters.hpp` | monster spawning |
| `Obstacles.hpp` | the systems of the obstacles: swing, push of the rocks, spikes, portals, bullets |
| `Solids.hpp` | `pushOutOf` and `resolveSolids`, shared by the server and the client prediction |
| `Zones.hpp` | `Zone` and `zoneOf(map, level)`: which scenery goes with a level |
| `Kinds.hpp` | `MonsterKind` and `BossKind`, the kinds of monsters and bosses |
| `BossDefs.hpp` | the table of definitions of the bosses, indexed by `BossKind` (it reuses `MonsterDef`) |
| `MonsterDefs.hpp` | `MonsterDef` and the table of definitions indexed by `MonsterKind` (stats, size, sprite) |
| `Boss.hpp` | the boss behaviour (`bossSystem`) and its fan of bullets |
| `MonsterBehavior.hpp` | one movement function per `MonsterKind` (`MONSTER_BEHAVIORS`) and `monsterSystem` |
| `Collectibles.hpp` | coins: `spawnCoin`, `coinPickupSystem` |
| `LevelSkip.hpp` | `SkipBank` and `grantSkip`: the scrolling won by killing monsters |
| `Waves.hpp` | the first table of waves of monsters (`WAVES`) |
| `LevelBuilder.hpp` | `LevelDef` (one level, distances relative to its start), `wave()` and `shifted()`, the bricks used to write the levels of a map |
| `MapList.hpp` | the number and the names of the maps (`MAP_COUNT`, `MAP_NAMES`), shared with the client |
| `MapHelpers.hpp` | `addGates` and `addCave`, to write walls in a map |
| `Leaderboard.hpp` | `Leaderboard`: best times of each map, sorted, 5 per map, saved in a text file |
| `builder.hpp` | `buildRun` (puts the levels of a map one after another), `MAPS` (one function per map id) and `buildMap(id)`; the maps themselves are in `src/maps/` of this folder (library `rtype_maps`) |
| `Levels.hpp` | `Segments`: the waves, the boss and the level numbers of the whole run, each with its distance |
| `Events.hpp` | `EntitySpawned`, `EntityDestroyed`, `EntityState`: what is told to the clients |
| `Status.hpp` | `GameStatus` and `Phase`: what the HUD shows |
| `Replication.hpp` | helpers that describe the entities to replicate (`NetId`, spawns, states) |
| `Game.hpp` | the `Game` class, entry point of the library |

The library is header-only: CMake target `rtype::game`
(`src/client/game/Rtype/CMakeLists.txt`), which links `engine::core`.

### The ECS rules to remember

These come from the engine (`engine/core`, not part of the game logic):

- Components must be **distinct, copyable structs**. Two components with the
  same layout still need two different types (that is why there are
  `Vec2Pos`, `Vec2Velocity`, `Vec2Hitbox`, ... instead of a bare `Vec2`).
- `Registry::each<Ts...>(lambda)` calls `lambda(Entity, Ts&...)` for every
  entity having all the `Ts`.
- **Never create, destroy, add or remove inside `each`.** The pattern used
  everywhere is: collect the entities in a `std::vector` first, then act on
  them once the loop is over (see `deathSystem`, `collectWith`).

## Game rules

- The map is **1920 x 1080**, in screen coordinates: the origin is the
  top-left corner, `x` grows to the right and `y` grows **downwards**.
- Up to **4 players** (`MAX_PLAYERS`), each one in a *slot* from 0 to 3. A slot
  decides the starting height of the player.
- The level scrolls from left to right. The display draws the scrolling, but
  the logic counts the distance scrolled (`Game::scroll`) to time the waves and
  the boss. It only runs while at least one player is connected.
- **Players** move with the keys, shoot to the right, and die when their
  centre leaves the map (`PLAYERS_DIE_OUT_OF_MAP`). A dead player can come back
  with `respawnPlayer` while the match is running.
- **Monsters** come in **waves** (`Waves.hpp`): a wave launches several
  monsters one after the other, at a fixed height or (`RANDOM_Y`) at a random
  height drawn separately for each monster. They fly to the
  left and shoot continuously. A monster that touches a player kills him.
- The level is described by `Segments` (`Levels.hpp`): waves, bosses and level
  numbers, each with the distance at which it starts. The first level has the
  boss at `BOSS_SPAWN_SCROLL` (20 s of play): it enters, stops against the
  right edge (`MAP_WIDTH` minus its width and `BOSS_RIGHT_MARGIN`;
  `BOSS_STOP_X` is that point for the Standard boss), sways vertically and
  fires a fan of bullets. The distance
  does not advance while a boss is alive.
- **Bullets** belong to a team. A bullet only hurts the other team, damages the
  first target it overlaps, then disappears.
- An entity whose `hp` falls to 0 or below is destroyed.
- **End of the game**: *victory* when every boss of the map has been beaten, *defeat* when every player
  is dead. The enemies and the walls are removed, the end screen stays for
  `RESTART_DELAY` seconds, then the game goes back to the lobby (see
  [Maps and lobby](#maps-and-lobby)). If the last player leaves, the world is
  reset.
- **Coins**: a monster that dies drops a coin; touching it adds to the purse
  shared by the players (`CoinCounter`, reset with each match).

## Kinds of monsters and bosses

Each kind has a definition (`MonsterDefs.hpp`, `BossDefs.hpp`): health, speed,
damage, fire cooldown, hit box and sprite. The hit box has the same proportions
as one frame of the sprite, because the client stretches the frame to the hit
box. Only the stats and the look exist so far: the special behaviours (dive,
zigzag, bombs, summoning...) are not coded yet.

| Kind | Sprite sheet | Hit box | HP | Speed | Damage |
|---|---|---|---|---|---|
| Basic | `r-typesheet5.gif` | 80 x 80 | 30 | 250 | 10 |
| Tank | `r-typesheet26.gif` (armoured block) | 169 x 130 | 120 | 120 | 20 |
| Kamikaze | `r-typesheet16.gif` (small pod) | 64 x 40 | 15 | 450 | 30 |
| Elite | `r-typesheet19.gif` (red drone) | 100 x 100 | 60 | 280 | 15 |
| Rocket | `r-typesheet11.gif` (missile launcher) | 105 x 108 | 45 | 140 | 35 |
| Bomber | `r-typesheet17.gif` (armoured giant) | 96 x 195 | 150 | 90 | 25 |
| Standard boss | `r-typesheet32.gif` | 480 x 264 | 600 | 120 | 20 |
| Dasher boss | `r-typesheet21.gif` (clawed alien) | 143 x 110 | 280 | 400 | 4 |
| Summoner boss | `r-typesheet40.gif` (dome turret) | 264 x 240 | 500 | 100 | 10 |
| Fortress boss | `r-typesheet37.gif` (battleship) | 700 x 210 | 1200 | 60 | 12 |

All the sheets are the official R-Type sprites of the `assets` folder. A boss
stops against the right edge of the map whatever its width.

Each kind also fires its own projectile (`ProjectileDef`: sprite, size, speed,
number of bullets per volley and angle between them):

| Kind | Projectile | Volley |
|---|---|---|
| Basic | orange fireball | 1 fast bullet |
| Tank | large orange shell | 1 slow bullet |
| Elite | small fast bolts | 2 bullets |
| Rocket | rocket missile (`r-typesheet10.gif`) | 1 fast rocket |
| Bomber | grey bomb (`r-typesheet3.gif`) | 1 slow bomb |

### Behaviours

`MonsterBehavior.hpp` holds one movement function per `MonsterKind`, in the
table `MONSTER_BEHAVIORS`. `monsterSystem` calls it every tick for each
monster that has entered the map (bosses are skipped). The per-monster state
is `MonsterState` (`age`, `locked`).

| Kind | Behaviour |
|---|---|
| Basic, Tank | straight line to the left |
| Kamikaze | flies straight until a player is closer than `DIVE_DISTANCE` on its left, then locks on this player and dives at full speed (the aim is not corrected after) |
| Elite | zigzags vertically (`ZIGZAG_AMPLITUDE`, `ZIGZAG_FREQUENCY`), never leaves the map |
| Rocket | stops during the last `ROCKET_CHARGE_TIME` seconds before it fires, then flies on |
| Bomber | straight line; its bomb (`ProjectileDef::drop`) falls up or down, on the side of the nearest player |

Bosses have their own table, `BOSS_BEHAVIORS` (`Boss.hpp`):

| Boss | Behaviour |
|---|---|
| Standard | vertical sway against the right edge |
| Dasher | roams the right side of the map on a Lissajous curve |
| Summoner | sways like the Standard boss and, every `SUMMON_INTERVAL` seconds, calls `SUMMON_COUNT` Kamikazes at its own position, spread vertically. Nothing is summoned while the minions alive would exceed `SUMMON_MAX_ALIVE`. Summoned monsters (`MonsterState::summoned`) drop no coin, so they cannot be farmed |
| Fortress | slow, wide sway; during the last `FORTRESS_BURST_DURATION` seconds of every `FORTRESS_BURST_PERIOD` it fires every `FORTRESS_BURST_COOLDOWN` seconds instead of its normal cooldown |
| Standard boss | orange fireballs | fan of 5 |
| Dasher boss | small blue alien shards (`r-typesheet9.gif`) | fan of 9 |
| Summoner boss | orange fireballs | fan of 3 |
| Fortress boss | orange fireballs | fan of 9 |

The Dasher does not sway like the other bosses: after its entrance it follows a
fast Lissajous curve over the whole right side of the map (`roamBoss`). Each
boss kind has its behaviour in the `BOSS_BEHAVIORS` table (`Boss.hpp`); the
Summoner and the Fortress have their own behaviour too (see
[Behaviours](#behaviours)).

## Obstacles

A level can contain obstacles (`ObstacleStep`: distance, height on the map,
size, kind, and optionally a swing). An obstacle appears at the right edge when
the level reaches its distance and then moves left at the speed of the
scrolling, so it stays still against the background. A wall that has left the
map is removed. There are five kinds (`ObstacleKind`):

| Kind | Effect on a player | Bullets | Monsters |
|---|---|---|---|
| `Rock` | **solid**: the ship cannot cross it and is pushed out (`pushOutOf`). A rock that pushes a ship against the left edge of the map crushes it, because a player whose centre leaves the map dies | absorbed | fly through |
| `Spike` | **deadly**: touching it sets the health to 0 | absorbed | fly through |
| `ReversePortal` | inverts the four keys; crossing a second one gives them back | cross | cross |
| `FastPortal` | the ship moves `FAST_SPEED_FACTOR` (1.5) times faster | cross | cross |
| `NormalPortal` | back to the normal speed | cross | cross |

A **portal** has the whole height of the map (`portal()` in `LevelBuilder.hpp`),
so it cannot be avoided. It acts once on each player that crosses it
(`PlayerEffects::lastPortal`). The effects are kept by the player (the
component `PlayerEffects`, read by `playerControlSystem`) until a `NormalPortal`
or a reverse portal changes them, and are cleared when the level changes
(`endPortalEffects`) and when a ship is created again. The speed of the ships
is independent of the scrolling: a fast portal changes how fast the ship
moves, not how fast the walls come.

An obstacle also has a `speed`: a multiplier of `OBSTACLE_SPEED`. A wall with
`speed = 2` crosses the screen twice as fast as the background, so the same
distance in the level leaves half the time to react. The portals and the
blocks of a run share the speed of its gates, so they keep their place between
them.

An obstacle can **swing**: `ObstacleStep::amplitude` and `period` move it up
and down around its height (`ObstacleMotion`, `obstacleMotionSystem`). A gate
that swings (`GateSpec::motion`) extends its two walls beyond the map by the
amplitude, so the gap stays the only hole while it moves. A **pinching** gate
(`GateSpec::pinch`) moves its two walls in opposite directions, so the gap
closes and opens; the tests keep its smallest opening above 2.5 ship heights.
`addSweepers` puts fast swinging spike blocks between two gates of a run.

In `LevelBuilder.hpp`, `wall()` writes one wall, `portal()` one portal and
`addGate(walls, GateSpec)` the top and the bottom wall around a gap.
`MapHelpers.hpp` has `addGates` and `addRun` (the gates of a death run with its
portals). The systems are in `Obstacles.hpp`, the push in `Solids.hpp`, which
the client also uses to predict its own ship against the rocks.

The client draws a wall by repeating a 64 x 64 tile (`obstacle_rock.png`: an
asteroid with craters and mineral veins; `obstacle_spike.png`: a lattice of
steel spikes with glowing red tips) every `OBSTACLE_TILE` units, and a portal
with an animated tile (`portal_*.png`).

## Maps and lobby

There are three maps (`MAPS` in `Builder.hpp`, names in `MapList.hpp`):

| Id | Name | Content |
|---|---|---|
| 0 | CAMPAIGN | the 20 levels described below |
| 1 | BOSS RUSH | 5 short levels, one to three bosses each, ending with the Fortress, the Summoner and the Dasher |
| 2 | DEATH RUNS | 4 relentless levels of spike gates, then a level with the Fortress and the Dasher: *Chain* (14 narrowing gates in a row, zigzag), *Sweepers* (fast spike blocks between the gates), *Pinch* (gates that close and open) and *Gauntlet* (16 gates 1.6 s apart, swinging, with reversed keys). Every level starts with a fast portal and the walls come 1.4 to 2 times faster than the scrolling |

`Game` has a `Lobby` phase, entered with `openLobby()` (the server does it at
startup). In the lobby nothing is simulated except the movement of the ships.
`startMap(segments, id)` is accepted only in the lobby; it resets the world
and starts the match. After a match the game goes back to the lobby, which is
also where it rests when nobody is connected. `host()` gives the slot allowed
to choose. `Game(seed)` keeps starting in `Playing`, which is what the tests
use.

## The first map: 20 levels

`map1()` (`src/maps/map1.cpp`) chains these levels. The bosses come one after
the other in the same level (the next one appears when the previous one is
dead).

| # | Level | What it is about |
|---|---|---|
| 1 | The original level | basic waves, Standard boss |
| 2 | Swarm | big swarms of Basic, first Tank |
| 3 | Kamikaze rush | fast small ships in groups |
| 4 | Asteroid field | scattered rocks to hide behind |
| 5 | Dasher | the alien boss |
| 6 | Elite squadron | diagonal formations |
| 7 | Cave | a tunnel of rocks above and below |
| 8 | Rocket battery | columns of rocket launchers |
| 9 | Summoner | Tanks, then the Summoner boss |
| 10 | **Death run I** | spike gates only, no enemy, a fast portal, two stretches with reversed keys, a few swinging gates |
| 11 | Bomber raid | slow bombers and fast Kamikazes |
| 12 | Crossfire | pillars and a mix of every enemy |
| 13 | Boss rush | Standard, then Dasher |
| 14 | Tank wall | lines of Tanks and Bombers, rocks as cover |
| 15 | **Death run II** | 16 narrower gates, four reversed stretches, five swinging gates |
| 16 | Escorted bosses | Summoner, then Standard |
| 17 | Gauntlet | rock gates to cross while enemies attack |
| 18 | Rocket storm | a narrow cave full of rockets and Kamikazes |
| 19 | Last stand | every enemy, then Standard and Summoner |
| 20 | Finale | a spike gauntlet with swinging gates and reversed keys, then Summoner, Dasher and Fortress |

Every wall pattern was checked with a reachability analysis (a ship moving at
500 units per second): the death runs can still be crossed by a ship moving at
about 60 % of that speed, so they are hard but not impossible.

## The tick: `Game::update`

`Game::update(dt)` first picks what to do from the phase:

- nobody connected: `idle()`, which resets the world if it has moved;
- phase `Playing`: `updatePlaying(dt)`;
- phase `Lobby`: `flyPlayers(dt)`, the ships move but nothing else happens;
- phase `Victory` or `Defeat`: `updateEnded(dt)`, where the players still fly
  until the end screen timer ends, then the game goes back to the lobby.

At the end, in every case, `trackDestroys()` then `trackSpawns()` turn the
changes of the world into events for the clients.

`updatePlaying` runs the systems in this order. **The order matters.**

| # | Call | Why here |
|---|---|---|
| 1 | `updateWeaponTimers(dt)` | cooldowns are up to date before anyone shoots |
| 2 | `lifetimeSystem(dt)` | short-lived entities (explosions) expire |
| 3 | `playerControlSystem` | keys become velocities, before they are used |
| 4 | `advanceLevel(dt)` | scrolls the level, queues the due waves, spawns monsters, walls and bosses |
| 5 | `monsterSystem(dt)` | the movement of each kind of monster (dive, zigzag, charge), then `obstacleMotionSystem(dt)` moves the swinging obstacles |
| 6 | `bossSystem(dt)` | moves the boss, makes it summon or fire a barrage |
| 7 | `shootingSystem` | bullets are born at the current position of the shooter |
| 8 | `movementSystem(dt)` | everybody moves: players, monsters, bullets, walls |
| 9 | `collisionSystem` | bullets hit their targets, using the positions *after* the move |
| 10 | `contactSystem` | a monster touching a player kills him |
| 11 | `obstacleSystem` | rocks push the players out, spikes kill, portals change the effects, walls absorb the bullets |
| 12 | `coinPickupSystem` | players pick up the coins they touch |
| 13 | `deleteUnitsOutOfBounds` | sets the hp of players who left the map to 0 and removes far entities and walls |
| 14 | `deathSystem` | destroys everything with `hp <= 0`, drops the coins, returns the kills |
| 15 | `skipLevel(kills)` | the kills move the scrolling forward (see [Level skip](#level-skip)) |
| 16 | `updatePhase` | every boss beaten means victory, every player dead means defeat |

If steps 13 and 14 were swapped, a player who left the map would live one more
tick. If step 3 came after step 7, the keys would take effect one tick late.

## Public API: the `Game` class

`Game` is the facade of the library. It owns the `Registry`, the level state
and the random generator; nothing else can touch them, so the order of the
systems cannot be broken from outside.

```cpp
rtype::Game game;                             // or Game(seed) for the tests

std::optional<int> slot = game.addPlayer();   // a client connects
game.setInput(*slot, input);                  // keys received from a client
game.update(rtype::config::FIXED_DT);         // one tick, 60 times per second
for (const auto &event : game.takeEvents()) { /* tell the clients */ }
game.removePlayer(*slot);                     // a client disconnects
```

| Method | Behaviour |
|---|---|
| `std::optional<int> addPlayer()` | creates a player in the first free slot and returns the slot. Returns `std::nullopt` when the game is full, so the caller can refuse the connection. |
| `void removePlayer(int slot)` | destroys the player of this slot and frees the slot. Does nothing if the slot is empty. |
| `bool respawnPlayer(int slot)` | brings a dead player back. Returns `false` if the match is over, the slot is not used or the player is alive. |
| `bool isPlayerAlive(int slot)` | is there a player entity in this slot? |
| `void setInput(int slot, const PlayerInput &input)` | stores the **current state of the keys** of a player. It stays in effect until the next call. Does nothing if the slot is empty. |
| `void openLobby()` | the game waits in the `Lobby` phase until `startMap`. The server calls it at startup. |
| `bool startMap(Segments segments, MapId map)` | starts a match on this map. Accepted only in the lobby, returns `false` otherwise. |
| `std::optional<int> host() const`, `MapId map() const` | the slot allowed to choose the map (lowest slot connected), the map being played. |
| `void update(float dt)` | advances the game by `dt` seconds. |
| `std::vector<GameEvent> takeEvents()` | the `EntitySpawned` and `EntityDestroyed` since the last call (and empties the list). |
| `std::vector<EntitySpawned> spawns() const` | the whole world, to send to a client who just connected. |
| `std::vector<EntityState> states() const` | the position of every moving entity. |
| `GameStatus status()` | phase, boss health, health of each player, coins of the shared purse, time elapsed since the start of the match, number of the level, map. |
| `Phase phase() const`, `float scroll() const` | the phase of the match, the distance scrolled. |

`Game(unsigned seed)` makes the random heights of the waves reproducible, which
is what the tests use.

`setInput` is about *state*, not events: the caller sends "right is held, fire
is held", not "right was pressed". Releasing everything stops the ship at the
next tick.

Why `std::optional` and not `-1` for "no slot"? The type tells the truth:
the compiler forces the caller to handle the empty case, and a forgotten check
cannot silently turn into an out-of-range index.

### What the server reads

The server never looks inside the `Registry`. After each `update` it reads:

- `takeEvents()`: new and destroyed entities, each with a network id (`NetId`)
  given by the game;
- `states()`: `(id, position)` for the entities that move;
- `status()`: what the HUD needs.

These types are in `Events.hpp` and `Status.hpp`, and the byte format is in
[the protocol](../../../../../docs/protocol.md).

## Components

All in `Components.hpp`, except the vectors.

| Component | Fields | Used by |
|---|---|---|
| `PlayerInput` | `up`, `down`, `left`, `right`, `fire` | `playerControlSystem`, `shootingSystem` |
| `Speed` | `value` (units per second) | `playerControlSystem` |
| `Health` | `hp` | collisions, `deathSystem` |
| `Damage` | `value` | `applyDamages`, copied into bullets |
| `Weapon` | `cooldown`, `timer` | `updateWeaponTimers`, `shoot` |
| `PlayerTag` | `slot` | queries, slot management |
| `MonsterTag` | `slot` | queries |
| `ProjectileTag` | (empty) | marks bullets |
| `Team` | `id` (`TeamId::Player` or `TeamId::Monster`) | who can hurt whom |
| `Vec2Pos` | `x`, `y` | top-left corner of the entity |
| `Vec2Velocity` | `x`, `y` | units per second |
| `Vec2Hitbox` | `x`, `y` | width and height of the box |
| `Vec2Direction` | `x`, `y` | currently unused by the systems |
| `Renderable` | `id` (`SpriteId`) | tells the client what to draw; marks the entities to replicate |
| `NetId` | `value` | identifier of the entity on the network, given by `Game` |
| `Lifetime` | `duration`, `remaining` | explosions, removed when they expire |
| `BossTag` | (empty) | marks the boss |

Tags (`PlayerTag`, `MonsterTag`, `ProjectileTag`) carry no behaviour. They only
let a system ask "give me the players" or "give me the bullets".

### Position, velocity, speed, direction

- **Velocity** is a vector: a direction and a magnitude, in units per second.
- **Speed** is the magnitude only (`Speed::value`).
- For a player: `velocity = normalize(direction from keys) * speed`.
  Normalizing keeps the diagonal as fast as a straight line.
- Movement is `pos += velocity * dt` (`calculatePos`). Since `dt` is in
  seconds, the game moves at the same real speed whatever the tick rate.

## Systems

All in `Systems.hpp` (spawning is in `Monsters.hpp`, the boss in `Boss.hpp`).

| System | What it does |
|---|---|
| `updateWeaponTimers(reg, dt)` | lowers every `Weapon::timer` by `dt`, never below 0 |
| `shootingSystem(reg)` | every entity with a `Weapon` that wants to shoot and is ready creates a bullet and restarts its timer |
| `playerControlSystem(reg)` | turns `PlayerInput` and `Speed` into `Vec2Velocity` |
| `movementSystem(reg, dt)` | applies `Vec2Velocity` to `Vec2Pos` |
| `collisionSystem(reg)` | for each bullet, finds the first enemy it overlaps, applies the damage, destroys the bullet |
| `deleteUnitsOutOfBounds(reg)` | players whose centre left the map lose all their hp, far monsters and bullets are destroyed |
| `deathSystem(reg, rng)` | destroys every entity with `hp <= 0`; monsters leave a coin whose value is drawn with the game's `rng` |
| `lifetimeSystem(reg, dt)` | removes the entities whose `Lifetime` ran out (explosions) |
| `contactSystem(reg)` | a monster that overlaps a player kills him |
| `bossSystem(reg, dt)` | enters, stops, sways and fires the fan of bullets (`Boss.hpp`) |
| `spawnMonster(reg, y)` | creates a monster on the right of the map at height `y`, moving left |

### Shooting

Shooting is split in small pieces so each rule can be read (and tested) alone:

- `isReadyToShoot(e)`: the entity has a `Weapon` whose timer is at 0.
- `wantsToShoot(e)`: players shoot when `PlayerInput::fire` is held;
  anything without a `PlayerInput` (monsters and bosses) shoots whenever it is
  ready, once it has entered the map. Their movement is in
  [Behaviours](#behaviours).
- `shoot(e)`: creates the bullet with `createBullet` and sets
  `timer = cooldown`.

`createBullet(reg, shooter, velocity)` copies the shooter's `Team` and `Damage` into the
bullet, places it just in front of the shooter and gives it the velocity it
receives (`shotVelocities` gives `BULLET_SPEED` to the right for the player
team; for the others it uses the volley of their `ProjectileDef`, or a falling
bomb).

### Collisions

Boxes are axis-aligned (AABB): two entities overlap when their boxes overlap on
both axes. Only **bullets** are tested against **targets** (entities with a
position, a hitbox, a `Team` and a `Health`), and only if their teams differ.
The work is done on `Collider` snapshots (entity + position + hitbox + team),
so the world is not modified while it is being read. Bodies colliding with
bodies (a monster ramming a player) is not part of the current rules.

## Configuration

Every number lives in `Config.hpp`, namespace `rtype::config`, as a
`constexpr`. Distances are in map units (1 unit = 1 pixel of a 1920x1080 map),
speeds in units per second, durations in seconds. Changing the difficulty or
the feel of the game never requires touching a system.

Examples: `PLAYER_SPEED`, `PLAYER_FIRE_COOLDOWN`, `MONSTER_HP`,
`MONSTER_SPAWN_INTERVAL`, `SPAWN_MARGIN`, `DESPAWN_MARGIN`,
`PLAYERS_DIE_OUT_OF_MAP`.

## How-to

### Add a component

1. Add a struct to `Components.hpp` (data only, copyable, a type of its own).
2. Add it in the relevant factory (`Factories.hpp`).
3. Read it in a system.

### Add a system

1. Write a free function `void mySystem(Engine::Registry &reg, ...)` in
   `Systems.hpp`, using `reg.each<...>` to read and modify components.
2. If it must create or destroy entities, collect them in a vector first and
   act after the `each` (see `deathSystem`).
3. Call it in `Game::update` **at the right place**, and add a line to the
   tick table above explaining why it is there.

### Add a new kind of monster

1. Add a value to `MonsterKind` in `Kinds.hpp` (before `Count`).
2. Add its `MonsterDef` (hp, speed, damage, cooldown, size, sprite,
   projectile) in `MonsterDefs.hpp` and its line in `MONSTERS`.
3. Add a `SpriteId` in `Components.hpp` and its sprite in `SpriteBank.cpp`
   (the protocol version changes, see [the protocol](../../../../../docs/protocol.md)).
4. Write its movement in `MonsterBehavior.hpp` and add it to
   `MONSTER_BEHAVIORS`, at the position of its kind (`flyStraight` if it only
   flies to the left).
5. Use it in the levels with `wave(at, MonsterKind::..., count)`.

### Add a map or a level

- A level: one more `LevelDef` in the list returned by `mapN()`. Distances are
  relative to the start of the level; everything must happen before
  `length`, and no wall may still be on screen when a boss appears.
- A map: a new file `src/maps/mapN.cpp` returning the list of levels, added to
  `rtype_maps` in `CMakeLists.txt`, declared and added to `MAPS` in
  `Builder.hpp`, with its name in `MAP_NAMES` (`MapList.hpp`). The tests in
  `MapTests.cpp` check every map of `MAPS`.

### Add a player action (a new key)

1. Add a `bool` to `PlayerInput`.
2. Read it in the system that implements the action.
3. The caller only needs to fill the new field before `setInput`.

## Coding conventions

Applied to all the game logic:

- Lines of **80 columns** at most.
- Functions of **20 lines** at most, **5 parameters** at most (lambdas
  included), at most **2 nested conditions**.
- No ternary operator: use an `if` and an early `return`.
- Components are `struct`s of pure data; behaviour lives in free functions.
- Constants go in `Config.hpp`, never as magic numbers in a system.
- Private members of classes are prefixed with `_`.
- `#pragma once` as include guard.
- One function does one thing: prefer small named predicates
  (`isReadyToShoot`, `canHit`) over long conditions.

## Level skip

Each monster killed moves the scrolling forward by `KILL_SKIP_SECONDS`
(0.5 s, i.e. `0.5 * SCROLL_SPEED` units), so the waves and the boss come
sooner. The time shown stays the real time.

- Bosses and summoned monsters do not count (`countsAsKill`), and nothing is
  gained while a boss is alive, since the scrolling is stopped then.
- The gain is capped for each level at `KILL_SKIP_MAX_FRACTION` (40 %) of its
  length (`levelSpan`, the distance to the next level; for the last level, the
  distance to its last boss or wave). `SkipBank` (`LevelSkip.hpp`) keeps the
  total of the current level and starts again on the next one.
- A wall or a wave that becomes due during a jump is placed back by the
  distance it went past (`createObstacle`'s `overshoot`), so the gaps between
  the gates of a death run never shrink.

## Current status

Done and working:

- players (4 slots), movement from keys, shooting, respawn
- 6 kinds of monsters with their own stats, projectile and movement
- 4 kinds of bosses (Standard, Dasher, Summoner, Fortress)
- walls: solid rocks, deadly spikes, swinging gates, and portals (speed, reversed keys)
- 3 maps (a 20-level campaign, a boss rush, death runs) built from
  `LevelDef`s, and the lobby to choose one
- coins, shared purse, match timer, level skip by kills
- the best times of each map
- bullets, collisions, damages, deaths, out-of-bounds rules
- the events, states and status read by the server
- the `Game` facade
- automated tests (`tests/`), see
  [Build and run](../../../../../docs/build.md#tests)

Ideas for later:

- destroyable tiles and power-ups of the original game
- spending the purse (the augments exist in `Augments.hpp`, without a shop)
- maps loaded from a data file instead of C++ tables
