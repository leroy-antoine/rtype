#pragma once

#include <algorithm>
#include <optional>
#include <vector>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Components.hpp"
#include "Colisions.hpp"
#include "Factories.hpp"
#include "Movements.hpp"
#include "Queries.hpp"


namespace rtype {
    inline void deathSystem(Engine::Registry &reg)
    {
        std::vector<Engine::Entity> dead;

        reg.each<Health>([&dead](Engine::Entity e, const Health &health) {
            if (health.hp <= 0) {
                dead.push_back(e);
            }
        });
        for (const Engine::Entity e : dead) {
            createExplosionOn(reg, e, explosionSize(reg, e));
            reg.destroy(e);
        }
    }

    inline void collisionSystem(Engine::Registry &reg)
    {
        const std::vector<Collider> targets = collectTargets(reg);
        std::vector<Engine::Entity> spent;

        for (const Collider &bullet : collectProjectiles(reg)) {
            if (hitFirstTarget(reg, bullet, targets)) {
                spent.push_back(bullet.entity);
            }
        }
        for (const Engine::Entity e : spent) {
            createExplosionOn(reg, e, config::HIT_EXPLOSION_SIZE);
            reg.destroy(e);
        }
    }

    inline void lifetimeSystem(Engine::Registry &reg, const float dt)
    {
        std::vector<Engine::Entity> expired;

        reg.each<Lifetime>([&expired, dt](const Engine::Entity e, Lifetime &lifetime) {
            lifetime.remaining -= dt;
            if (lifetime.remaining <= 0.f) {
                expired.push_back(e);
            }
        });
        for (const Engine::Entity e : expired) {
            reg.destroy(e);
        }
    }

    inline void movementSystem(Engine::Registry &reg, const float dt)
    {
        reg.each<gameLogic::Vec2Pos, gameLogic::Vec2Velocity>(
            [dt](Engine::Entity, gameLogic::Vec2Pos &pos, const gameLogic::Vec2Velocity &vel) {
                pos = gameLogic::Vec2Pos{calculatePos(pos, vel, dt)};
            });
    }

    inline void contactSystem(Engine::Registry &reg)
    {
        const std::vector<Collider> monsters = collectColliders(reg, collectMonsters(reg));

        for (const Collider &player : collectColliders(reg, collectPlayers(reg))) {
            for (const Collider &monster : monsters) {
                applyContact(reg, player, monster);
            }
        }
    }

    // Transforme les touches de chaque joueur en vitesse.
    inline void playerControlSystem(Engine::Registry &reg)
    {
        reg.each<PlayerInput, Speed, gameLogic::Vec2Velocity>(
            [](Engine::Entity, const PlayerInput &input, const Speed &speed,
               gameLogic::Vec2Velocity &vel) {
                vel = gameLogic::Vec2Velocity{
                    inputDirection(input) * speed.value};
            });
    }

    // Les joueurs dont le centre est hors de la carte perdent toute leur vie.
    inline void killPlayersOutOfBounds(Engine::Registry &reg)
    {
        if (!config::PLAYERS_DIE_OUT_OF_MAP) {
            return;
        }
        for (const Engine::Entity e : collectPlayers(reg)) {
            const std::optional<Collider> collider = getCollider(reg, e);

            if (collider && isOutOfBounds(*collider) && reg.has<Health>(e)) {
                reg.get<Health>(e).hp = 0;
            }
        }
    }

    // Detruit les entites de la liste qui sont loin au-dela de la carte.
    inline void destroyFarEntities(Engine::Registry &reg,
                                   const std::vector<Engine::Entity> &entities)
    {
        for (const Engine::Entity e : entities) {
            const std::optional<Collider> collider = getCollider(reg, e);

            if (collider && isFarOutOfBounds(*collider)) {
                reg.destroy(e);
            }
        }
    }

    // A appeler apres movementSystem et avant deathSystem.
    inline void deleteUnitsOutOfBounds(Engine::Registry &reg)
    {
        killPlayersOutOfBounds(reg);
        destroyFarEntities(reg, collectMonsters(reg));
        destroyFarEntities(reg, collectProjectileEntities(reg));
    }

    inline void updateWeaponTimers(Engine::Registry &reg, const float dt)
    {
        reg.each<Weapon>([dt](const Engine::Entity, Weapon &weapon) {
            weapon.timer -= dt;
            if (weapon.timer < 0.f) {
                weapon.timer = 0.f;
            }
        });
    }

    inline bool isReadyToShoot(const Engine::Registry &reg,
        const Engine::Entity e)
    {
        if (!reg.has<Weapon>(e)) {
            return false;
        }
        return reg.get<Weapon>(e).timer <= 0.f;
    }

    inline void shoot(Engine::Registry &reg, const Engine::Entity shooter)
    {
        if (!isReadyToShoot(reg, shooter)) {
            return;
        }
        if (createBullet(reg, shooter) == Engine::Entity{}) {
            return;
        }
        auto &[cooldown, timer] = reg.get<Weapon>(shooter);

        timer = cooldown;
    }

    inline bool wantsToShoot(const Engine::Registry &reg,
        const Engine::Entity e)
    {
        if (reg.has<PlayerInput>(e)) {
            return reg.get<PlayerInput>(e).fire;
        }
        return true;
    }

    // Fait tirer toutes les entites armees qui le veulent.
    inline void shootingSystem(Engine::Registry &reg)
    {
        for (const Engine::Entity e : collectWith<Weapon>(reg)) {
            if (wantsToShoot(reg, e)) {
                shoot(reg, e);
            }
        }
    }
}
