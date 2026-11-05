# Roadmap: weapons

Goal: follow the guidelines of the subject with new kinds of weapons. The
augments bought in the shop are only personal buffs today.

## Decisions already made

- Base weapon on Space: a tap fires one shot, holding Space charges a
  stronger piercing shot released on release. The hold-to-autofire goes away
  (an "auto fire" augment could bring it back later).
- Bought weapons go in a **secondary slot** fired with an extra key. One
  secondary weapon at a time; buying another replaces it.
- Secondary weapons: laser ray, rocket launcher, chain lightning.
- Chain lightning only jumps to enemies **stuck to** the one that was hit
  (short jump radius), it is not a screen-wide strike.
- Passive weapon: **one drone**, upgradable with further purchases, flying
  beside the ship and firing on its own.
- Everything goes through `applyDamages`, so the difficulty multipliers apply.
- Code follows the clean-code rules (functions <= 20 lines, lines <= 80
  columns, <= 2 nesting levels, no ternary, <= 5 params), docs in English.

## Method

At every step: I make proposals, you give your opinion, then I code.
Build and tests pass at each step.

## Steps

### W1. Charged shot on the base weapon
- [x] Charge time tracked by the server from the `INPUT` keys.
- [x] A tap (under 0.3 s) fires one normal shot when released.
- [x] A longer hold charges up to 1.2 s; release fires a piercing shot, x2
      damage at the start of the charge up to x4 when full, bigger as it
      gets stronger.
- [x] The charge is lost when the player dies, and when the game is paused
      or the shop opens.
- [x] Gauge under the ship (client side, from the hold time of Space).
- [x] Tests (tap, partial charge, full charge, piercing, reset).

### W2. Secondary weapon slot (done)
- [x] `KEY_ALT` input bit and the X key on the client (protocol bump).
- [x] `SecondaryWeapon` component, one recharge per weapon, `secondarySystem`
      calling the fire function of the weapon (stubs until W3).
- [x] The three weapons are shop augments (`Rocket`, `Laser`, `Lightning`);
      buying one replaces the equipped weapon, the last one bought is kept
      after a respawn.
- [x] Their labels show in the augment row of the HUD.
- [ ] Cooldown or heat gauge in the HUD (needs the weapons of W3).
- [x] Tests.

### W3. Secondary weapons
- [x] Laser ray: hold X for 2 s, release to fire a flash (0.4 s) across
      the screen up to the first wall, x8 damage once to everything it
      crosses; releasing early cancels for free; 6 s cooldown; charge gauge
      under the ship.
- [x] Rocket launcher: slow rocket (450), explosion of radius 120 on an
      enemy, a wall or the edge of the map; triple damage, no damage to the
      players, 1.2 s between two rockets.
- [x] Chain lightning: aimed straight ahead by moving the ship (first
      enemy on the line of the nose, range 600), then jumps only to enemies
      stuck to the one hit (gap of 100, 4 jumps, damage x1.5 then x0.8 per
      jump); nothing fires, and no cooldown starts, without a target.
- [ ] One proposal round, one commit-sized step and tests per weapon.

### W4. Drone (done)
- [x] Three shop augments (`Drone`, `DroneBoost`, `DroneTwin`) that give
      the drone level 1, 2 or 3 (shop items are unique per team, so each
      level is its own item).
- [x] The drone flies ahead of the ship and above it, smoothed, and fires
      alone at the nearest enemy within 700, half the damage of the owner.
- [x] Level 2 fires faster, level 3 fires two bullets.
- [x] It cannot be hit, disappears with its owner and comes back with its
      level at the respawn.
- [x] Tests.

### W5. Documentation and final code review
- [ ] `docs/protocol.md`, `docs/gameplay.md`, READMEs of the game, the
      client and the server.
- [x] `/code-review` on the branch and fixes.

## Open points to settle step by step
- Key of the secondary weapon (X?), damage curve of the charge.
- Heat or ammo for the laser, area of the rocket, jump radius of the chain.
- Friendly fire with the rocket explosion and the laser.
- Prices and rarities, sprites to provide (rocket, ray, lightning, drone,
  charged shot).
