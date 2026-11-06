#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <vector>
#include "engine/core/Registry.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Kinds.hpp"
#include "Laser.hpp"
#include "Lightning.hpp"
#include "Rocket.hpp"

namespace rtype {
    using SecondaryFire = bool (*)(Engine::Registry &, Engine::Entity);

    // `fire` renvoie false quand il n'y a rien a tirer : la recharge ne
    // demarre pas. `chargeTime` vaut 0 pour une arme qui tire tant que X
    // est maintenu, sinon il faut maintenir X ce temps puis relacher pour
    // tirer.
    struct SecondaryDef {
        WeaponKind kind;
        float cooldown;
        float chargeTime;
        SecondaryFire fire;
    };

    inline constexpr std::array<SecondaryDef,
        static_cast<std::size_t>(WeaponKind::Count)> SECONDARY_DEFS = {{
        {WeaponKind::Rocket, config::ROCKET_COOLDOWN, 0.f, fireRocket},
        {WeaponKind::Laser, config::LASER_COOLDOWN, config::LASER_CHARGE_TIME,
            fireLaser},
        {WeaponKind::Lightning, config::LIGHTNING_COOLDOWN, 0.f,
            fireLightning}}};

    inline const SecondaryDef &secondaryDef(const WeaponKind kind)
    {
        return SECONDARY_DEFS[static_cast<std::size_t>(kind)];
    }

    // Une arme achetee prend la place de l'arme secondaire actuelle.
    inline void equipSecondary(Engine::Registry &reg, const Engine::Entity e,
        const WeaponKind kind)
    {
        const SecondaryWeapon weapon{kind, secondaryDef(kind).cooldown};

        if (reg.has<SecondaryWeapon>(e)) {
            reg.get<SecondaryWeapon>(e) = weapon;
            return;
        }
        reg.add(e, weapon);
    }

    inline void updateInstant(Engine::Registry &reg, const Engine::Entity e,
        SecondaryWeapon &weapon)
    {
        if (weapon.timer > 0.f || !reg.get<PlayerInput>(e).alt) {
            return;
        }
        if (secondaryDef(weapon.kind).fire(reg, e)) {
            weapon.timer = weapon.cooldown;
        }
    }

    // On maintient X pour charger, on relache pour tirer : relacher avant
    // la fin de la charge l'annule sans rien coute.
    inline void updateCharged(Engine::Registry &reg, const Engine::Entity e,
        SecondaryWeapon &weapon, const float dt)
    {
        const SecondaryDef &def = secondaryDef(weapon.kind);

        if (reg.get<PlayerInput>(e).alt && weapon.timer <= 0.f) {
            weapon.charge = std::min(weapon.charge + dt, def.chargeTime);
            return;
        }
        const bool ready = weapon.charge >= def.chargeTime;

        weapon.charge = 0.f;
        if (ready && def.fire(reg, e)) {
            weapon.timer = weapon.cooldown;
        }
    }

    inline void updateSecondary(Engine::Registry &reg, const Engine::Entity e,
        const float dt)
    {
        SecondaryWeapon &weapon = reg.get<SecondaryWeapon>(e);

        weapon.timer = std::max(0.f, weapon.timer - dt);
        if (secondaryDef(weapon.kind).chargeTime > 0.f) {
            updateCharged(reg, e, weapon, dt);
            return;
        }
        updateInstant(reg, e, weapon);
    }

    inline void secondarySystem(Engine::Registry &reg, const float dt)
    {
        std::vector<Engine::Entity> armed;

        reg.each<SecondaryWeapon, PlayerInput>(
            [&armed](const Engine::Entity e, const SecondaryWeapon &,
                const PlayerInput &) {
                armed.push_back(e);
            });
        for (const Engine::Entity e : armed) {
            updateSecondary(reg, e, dt);
        }
    }
}
