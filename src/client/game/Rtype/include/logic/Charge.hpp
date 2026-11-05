#pragma once

#include <algorithm>
#include <cmath>
#include "engine/core/Registry.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Factories.hpp"
#include "Queries.hpp"
#include "Systems.hpp"

namespace rtype {
    // 0 au debut de la charge, 1 quand la jauge est pleine.
    inline float chargeRatio(const float time)
    {
        const float span = config::CHARGE_FULL_TIME - config::CHARGE_TAP_LIMIT;

        return std::clamp((time - config::CHARGE_TAP_LIMIT) / span, 0.f, 1.f);
    }

    inline bool isCharging(const float time)
    {
        return time >= config::CHARGE_TAP_LIMIT;
    }

    // Multiplicateur des degats du tir charge.
    inline float chargeFactor(const float time)
    {
        return config::CHARGE_MIN_FACTOR
            + (config::CHARGE_MAX_FACTOR - config::CHARGE_MIN_FACTOR)
            * chargeRatio(time);
    }

    // Multiplicateur de la taille du tir charge.
    inline float chargeSize(const float time)
    {
        return config::CHARGE_MIN_SIZE
            + (config::CHARGE_MAX_SIZE - config::CHARGE_MIN_SIZE)
            * chargeRatio(time);
    }

    inline void growBullet(Engine::Registry &reg, const Engine::Entity bullet,
        const float factor)
    {
        gameLogic::Vec2Hitbox &box = reg.get<gameLogic::Vec2Hitbox>(bullet);
        const float grown = box.y * factor;

        reg.get<gameLogic::Vec2Pos>(bullet).y -= (grown - box.y) / 2.f;
        box = gameLogic::Vec2Hitbox{{box.x * factor, grown}};
    }

    // Une grosse balle qui traverse les ennemis.
    inline Engine::Entity createChargedBullet(Engine::Registry &reg,
        const Engine::Entity shooter, const float time)
    {
        const Engine::Entity bullet = createBullet(reg, shooter,
            {config::BULLET_SPEED, 0.f});

        if (bullet == Engine::Entity{}) {
            return bullet;
        }
        growBullet(reg, bullet, chargeSize(time));
        Damage &damage = reg.get<Damage>(bullet);

        damage.value = static_cast<int>(std::lround(
            static_cast<float>(damage.value) * chargeFactor(time)));
        reg.add(bullet, Pierce{});
        return bullet;
    }

    // Le joueur relache la touche : un coup normal apres un appui court,
    // un tir charge apres un maintien.
    inline void releaseShot(Engine::Registry &reg, const Engine::Entity player,
        const float time)
    {
        if (!isCharging(time)) {
            shoot(reg, player);
            return;
        }
        if (createChargedBullet(reg, player, time)
            != Engine::Entity{}) {
            reg.get<Weapon>(player).timer = reg.get<Weapon>(player).cooldown;
        }
    }

    inline void updateCharge(Engine::Registry &reg, const Engine::Entity player,
        const float dt)
    {
        Charge &charge = reg.get<Charge>(player);

        if (reg.get<PlayerInput>(player).fire) {
            charge.time = std::min(charge.time + dt, config::CHARGE_FULL_TIME);
            return;
        }
        if (charge.time > 0.f) {
            const float held = charge.time;

            charge.time = 0.f;
            releaseShot(reg, player, held);
        }
    }

    inline void chargeSystem(Engine::Registry &reg, const float dt)
    {
        for (const Engine::Entity player : collectWith<Charge>(reg)) {
            updateCharge(reg, player, dt);
        }
    }

    // La charge est perdue : pause, boutique.
    inline void resetCharges(Engine::Registry &reg)
    {
        reg.each<Charge>([](const Engine::Entity, Charge &charge) {
            charge.time = 0.f;
        });
        reg.each<SecondaryWeapon>(
            [](const Engine::Entity, SecondaryWeapon &weapon) {
                weapon.charge = 0.f;
            });
    }
}
