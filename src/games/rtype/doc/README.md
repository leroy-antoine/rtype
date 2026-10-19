# R-Type game logic

Technical documentation of the **game logic** (`src/games/rtype`).

This part decides *what happens in the game*: where entities are, who shoots,
who gets hit, who dies. It knows nothing about the network, the window, the
sprites or the sound. It is a plain C++ library built on top of the ECS of
`engine/core`.

- [Overview](#overview)
- [Architecture](#architecture)
- [Game rules](#game-rules)
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
| `Bullet.hpp` | helpers for bullets: `isShooter`, `bulletDirection`, `bulletStartPos` |
| `Factories.hpp` | `createPlayer`, `createMonster`, `createBullet` |
| `Damages.hpp` | `applyDamages` |
| `Colisions.hpp` | `Collider`, `overlaps`, `canHit`, bounds tests |
| `Queries.hpp` | read-only searches in the world (`collectWith<Tag>`, `findFreeSlot`, ...) |
| `Systems.hpp` | the systems (see below) |
| `Monsters.hpp` | monster spawning |
| `Game.hpp` | the `Game` class, entry point of the library |

The library is header-only: CMake target `rtype::game`
(`src/games/rtype/CMakeLists.txt`), which links `engine::core`.

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
- The level scrolls from left to right. The scrolling is handled by the
  display: the logic only moves entities in the fixed map.
- **Players** move with the keys, shoot to the right, and die when their
  centre leaves the map (`PLAYERS_DIE_OUT_OF_MAP`).
- **Monsters** appear on the right of the map every
  `MONSTER_SPAWN_INTERVAL` seconds, fly to the left and shoot to the left
  continuously. They are removed when they are far outside the map.
- **Bullets** belong to a team. A bullet only hurts the other team, damages the
  first target it overlaps, then disappears.
- An entity whose `hp` falls to 0 or below is destroyed.

## The tick: `Game::update`

`Game::update(dt)` runs the systems in this order. **The order matters.**

| # | Call | Why here |
|---|---|---|
| 1 | `updateWeaponTimers(dt)` | cooldowns are up to date before anyone shoots |
| 2 | `shootingSystem` | bullets are born at the current position of the shooter |
| 3 | `updateSpawn(dt)` | new monsters enter the world |
| 4 | `playerControlSystem` | keys become velocities, before they are used |
| 5 | `movementSystem(dt)` | everybody moves: players, monsters, bullets |
| 6 | `collisionSystem` | uses the positions *after* the move |
| 7 | `deleteUnitsOutOfBounds` | sets the hp of players who left the map to 0 and removes far entities |
| 8 | `deathSystem` | last: destroys everything with `hp <= 0` (hits of step 6, exits of step 7) |

If steps 7 and 8 were swapped, a player who left the map would live one more
tick. If step 4 came after step 5, the keys would take effect one tick late.

## Public API: the `Game` class

`Game` is the facade of the library. It owns the `Registry` and the spawn
timer; nothing else can touch them, so the order of the systems cannot be
broken from outside.

```cpp
rtype::Game game;

std::optional<int> slot = game.addPlayer();   // a client connects
game.setInput(*slot, input);                  // keys received from a client
game.update(rtype::config::FIXED_DT);         // one tick, 60 times per second
game.removePlayer(*slot);                     // a client disconnects
```

| Method | Behaviour |
|---|---|
| `std::optional<int> addPlayer()` | creates a player in the first free slot and returns the slot. Returns `std::nullopt` when the game is full, so the caller can refuse the connection. |
| `void removePlayer(int slot)` | destroys the player of this slot. Does nothing if the slot is empty. |
| `void setInput(int slot, const PlayerInput &input)` | stores the **current state of the keys** of a player. It stays in effect until the next call. Does nothing if the slot is empty. |
| `void update(float dt)` | advances the game by `dt` seconds. |

`setInput` is about *state*, not events: the caller sends "right is held, fire
is held", not "right was pressed". Releasing everything stops the ship at the
next tick.

Why `std::optional` and not `-1` for "no slot"? The type tells the truth:
the compiler forces the caller to handle the empty case, and a forgotten check
cannot silently turn into an out-of-range index.

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

All in `Systems.hpp` (spawning is in `Monsters.hpp`).

| System | What it does |
|---|---|
| `updateWeaponTimers(reg, dt)` | lowers every `Weapon::timer` by `dt`, never below 0 |
| `shootingSystem(reg)` | every entity with a `Weapon` that wants to shoot and is ready creates a bullet and restarts its timer |
| `playerControlSystem(reg)` | turns `PlayerInput` and `Speed` into `Vec2Velocity` |
| `movementSystem(reg, dt)` | applies `Vec2Velocity` to `Vec2Pos` |
| `collisionSystem(reg)` | for each bullet, finds the first enemy it overlaps, applies the damage, destroys the bullet |
| `deleteUnitsOutOfBounds(reg)` | players whose centre left the map lose all their hp, far monsters and bullets are destroyed |
| `deathSystem(reg)` | destroys every entity with `hp <= 0` |
| `spawnMonster(reg, y)` | creates a monster on the right of the map at height `y`, moving left |

### Shooting

Shooting is split in small pieces so each rule can be read (and tested) alone:

- `isReadyToShoot(e)`: the entity has a `Weapon` whose timer is at 0.
- `wantsToShoot(e)`: players shoot when `PlayerInput::fire` is held;
  anything without a `PlayerInput` (monsters, later the boss) shoots whenever
  it is ready. This is the whole "AI" of the monsters for now.
- `shoot(e)`: creates the bullet with `createBullet` and sets
  `timer = cooldown`.

`createBullet(reg, shooter)` copies the shooter's `Team` and `Damage` into the
bullet, places it just in front of the shooter and gives it a velocity
(`BULLET_SPEED`) to the right for the player team, to the left otherwise.

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

1. Add its constants to `Config.hpp` (hp, speed, size, damage, cooldown).
2. Write its factory in `Factories.hpp`, building on `createMonster` or
   starting from a new tag if the systems must tell it apart.
3. Spawn it from `Monsters.hpp`, with its own position and velocity.
4. If it needs a specific behaviour (sine wave, aimed shots), add a component
   that describes the behaviour and a system that reads it. Do not add
   `if (isThisMonster)` branches to the existing systems.

### Add a player action (a new key)

1. Add a `bool` to `PlayerInput`.
2. Read it in the system that implements the action.
3. The caller only needs to fill the new field before `setInput`.

## Coding conventions

Applied to all the game logic:

- Lines of **80 columns** at most.
- Functions of **25 lines** at most, **5 parameters** at most (lambdas
  included), at most **2 nested conditions**.
- No ternary operator: use an `if` and an early `return`.
- Components are `struct`s of pure data; behaviour lives in free functions.
- Constants go in `Config.hpp`, never as magic numbers in a system.
- Private members of classes are prefixed with `_`.
- `#pragma once` as include guard.
- One function does one thing: prefer small named predicates
  (`isReadyToShoot`, `canHit`) over long conditions.

## Current status

Done and working:

- players (4 slots), movement from keys, shooting
- monsters (spawn, movement, shooting)
- bullets, collisions, damages, deaths
- out-of-bounds rules
- the `Game` facade

Not done yet:

- **Boss** at the end of the level and the **end of the game** (victory when
  the boss dies, defeat when no player is alive). The constants already exist
  in `Config.hpp` (`BOSS_*`).
- **Events** (a player died, a monster appeared, game over) so that the server
  can tell the clients.
- **Reading the state of the world** to send it to the clients.
- **Random spawn height**: monsters currently all appear at `y = 0`.
- **Input source**: `setInput` is ready, but nothing calls it yet; how the
  inputs travel from the network to the `main` is still to be decided.
- **Automated tests** in the repository.
