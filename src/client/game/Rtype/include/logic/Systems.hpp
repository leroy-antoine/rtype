#pragma once

#include <algorithm>
#include <optional>
#include <random>
#include <vector>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Components.hpp"
#include "Colisions.hpp"
#include "Factories.hpp"
#include "Collectibles.hpp"
#include "Movements.hpp"
#include "Queries.hpp"
#include "Boss.hpp"
#include "MonsterBehavior.hpp"
#include "Obstacles.hpp"


namespace rtype {
    inline bool countsAsKill(const Engine::Registry &reg,
        const Engine::Entity e)
    {
        return reg.has<MonsterTag>(e) && !reg.has<BossTag>(e)
            && !isSummoned(reg, e);
    }

    inline int deathSystem(Engine::Registry &reg, std::mt19937 &rng)
    {
        std::vector<Engine::Entity> dead;
        int kills = 0;

        reg.each<Health>([&dead](Engine::Entity e, const Health &health) {
            if (health.hp <= 0) {
                dead.push_back(e);
            }
        });
        for (const Engine::Entity e : dead) {
            createExplosionOn(reg, e, explosionSize(reg, e));
            spawnCoin(reg, e, rng);
            if (countsAsKill(reg, e)) {
                ++kills;
            }
            reg.destroy(e);
        }
        return kills;
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
        reg.each<PlayerInput, Speed, PlayerEffects, gameLogic::Vec2Velocity>(
            [](Engine::Entity, const PlayerInput &input, const Speed &speed,
               const PlayerEffects &effects, gameLogic::Vec2Velocity &vel) {
                vel = gameLogic::Vec2Velocity{
                    inputDirection(input, effects.inverted)
                        * (speed.value * effects.speedFactor)};
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

    inline void healPlayers(Engine::Registry &reg)
    {
        for (const Engine::Entity e : collectPlayers(reg)) {
            reg.get<Health>(e).hp = config::PLAYER_HP;
        }
    }

    // A appeler apres movementSystem et avant deathSystem.
    inline void deleteUnitsOutOfBounds(Engine::Registry &reg)
    {
        killPlayersOutOfBounds(reg);
        destroyFarEntities(reg, collectMonsters(reg));
        destroyFarEntities(reg, collectProjectileEntities(reg));
        destroyFarObstacles(reg);
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

    inline void updateShieldTimers(Engine::Registry &reg, const float dt)
    {
        reg.each<Shield>([dt](const Engine::Entity, Shield &shield) {
            shield.timer = std::max(0.f, shield.timer - dt);
        });
    }

    inline void regenerationSystem(Engine::Registry &reg, const float dt)
    {
        reg.each<Regeneration, Health, MaxHealth>(
            [dt](const Engine::Entity, Regeneration &regeneration,
                Health &health, const MaxHealth &max) {
                regeneration.timer -= dt;
                if (regeneration.timer > 0.f) {
                    return;
                }
                regeneration.timer = regeneration.interval;
                health.hp = std::min(max.hp, health.hp + regeneration.hp);
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

    // La bombe tombe du cote ou se trouve le joueur le plus proche.
    inline gameLogic::Vec2 bombVelocity(Engine::Registry &reg,
        const Engine::Entity shooter, const ProjectileDef &projectile)
    {
        const gameLogic::Vec2 from = centerOf(reg, shooter);
        const auto target = nearestPlayerCenter(reg, from);
        float direction = 1.f;

        if (target && target->y < from.y) {
            direction = -1.f;
        }
        return {-config::SCROLL_SPEED, direction * projectile.speed};
    }

    // Vitesses des balles d'un tir : une balle vers la droite pour les joueurs,
    // la salve de son projectile pour les monstres et les boss.
    inline std::vector<gameLogic::Vec2> shotVelocities(
        Engine::Registry &reg, const Engine::Entity shooter)
    {
        if (reg.has<MultiShot>(shooter)) {
            return fan(reg.get<MultiShot>(shooter).bullets,
                config::MULTISHOT_SPREAD, config::BULLET_SPEED, 1.f);
        }
        if (reg.get<Team>(shooter).id == TeamId::Player) {
            return {{config::BULLET_SPEED, 0.f}};
        }
        const ProjectileDef &projectile = projectileOf(reg, shooter);

        if (projectile.drop) {
            return {bombVelocity(reg, shooter, projectile)};
        }
        return volley(projectile);
    }

    inline void shoot(Engine::Registry &reg, const Engine::Entity shooter)
    {
        if (!isReadyToShoot(reg, shooter)) {
            return;
        }
        bool fired = false;

        for (const gameLogic::Vec2 &velocity : shotVelocities(reg, shooter)) {
            if (createBullet(reg, shooter, velocity) != Engine::Entity{}) {
                fired = true;
            }
        }
        if (fired) {
            auto &[cooldown, timer] = reg.get<Weapon>(shooter);

            timer = cooldown;
        }
    }

    // Les joueurs tirent quand ils appuient sur la touche, les monstres des
    // qu'ils sont entres sur la carte.
    inline bool wantsToShoot(const Engine::Registry &reg,
        const Engine::Entity e)
    {
        if (reg.has<PlayerInput>(e)) {
            return reg.get<PlayerInput>(e).fire;
        }
        return reg.has<gameLogic::Vec2Pos>(e)
            && reg.get<gameLogic::Vec2Pos>(e).x < config::MAP_WIDTH;
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
