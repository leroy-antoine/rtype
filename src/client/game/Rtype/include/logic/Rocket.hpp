#pragma once

#include <algorithm>
#include <cmath>
#include <vector>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Colisions.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Factories.hpp"
#include "Obstacles.hpp"
#include "Queries.hpp"

namespace rtype {
    // La roquette part du nez du vaisseau, centree sur sa hauteur.
    inline bool fireRocket(Engine::Registry &reg, const Engine::Entity shooter)
    {
        if (!isShooter(reg, shooter)) {
            return false;
        }
        const gameLogic::Vec2Pos pos = reg.get<gameLogic::Vec2Pos>(shooter);
        const gameLogic::Vec2Hitbox box =
            reg.get<gameLogic::Vec2Hitbox>(shooter);
        const Engine::Entity rocket = reg.create();

        reg.add(rocket, gameLogic::Vec2Pos{{pos.x + box.x,
            pos.y + box.y / 2.f - config::ROCKET_HEIGHT / 2.f}});
        reg.add(rocket, gameLogic::Vec2Hitbox{{config::ROCKET_WIDTH,
            config::ROCKET_HEIGHT}});
        reg.add(rocket, gameLogic::Vec2Velocity{{config::ROCKET_SPEED, 0.f}});
        reg.add(rocket, Damage{reg.get<Damage>(shooter).value
            * config::ROCKET_DAMAGE_FACTOR});
        reg.add(rocket, Team{TeamId::Player});
        reg.add(rocket, Missile{config::ROCKET_RADIUS});
        reg.add(rocket, Renderable{SpriteId::PlayerRocket});
        return true;
    }

    // Le disque de rayon `radius` touche-t-il la boite ?
    inline bool blastReaches(const gameLogic::Vec2 &center, const float radius,
        const Collider &target)
    {
        const float x = std::clamp(center.x, target.pos.x,
            target.pos.x + target.box.x);
        const float y = std::clamp(center.y, target.pos.y,
            target.pos.y + target.box.y);

        return std::hypot(center.x - x, center.y - y) <= radius;
    }

    // Tous les ennemis dans le rayon subissent les degats entiers.
    inline void detonate(Engine::Registry &reg, const Engine::Entity rocket)
    {
        const Collider body{rocket, reg.get<gameLogic::Vec2Pos>(rocket),
            reg.get<gameLogic::Vec2Hitbox>(rocket), Team{}};
        const gameLogic::Vec2 center = centerOfBox(body);
        const float radius = reg.get<Missile>(rocket).radius;

        for (const Collider &target : collectTargets(reg)) {
            if (target.team.id == TeamId::Monster
                && isAlive(reg, target.entity)
                && blastReaches(center, radius, target)) {
                applyDamages(target.entity, rocket, reg);
            }
        }
        createExplosion(reg, center, radius * 2.f);
        reg.destroy(rocket);
    }

    // Un ennemi a moins de ROCKET_FUSE de la roquette la fait exploser.
    inline bool nearAnything(const Collider &rocket,
        const std::vector<Collider> &enemies)
    {
        return std::ranges::any_of(enemies, [&rocket](const Collider &enemy) {
            return gapBetween(rocket, enemy) <= config::ROCKET_FUSE;
        });
    }

    inline bool shouldDetonate(const Collider &rocket,
        const std::vector<Collider> &enemies,
        const std::vector<Collider> &walls)
    {
        return nearAnything(rocket, enemies)
            || std::ranges::any_of(walls, [&rocket](const Collider &wall) {
                return overlaps(rocket, wall);
            })
            || rocket.pos.x >= config::MAP_WIDTH;
    }

    // Une roquette explose pres d'un ennemi, sur un mur ou au bord.
    inline void rocketSystem(Engine::Registry &reg)
    {
        const std::vector<Collider> enemies = livingEnemies(reg);
        const std::vector<Collider> walls = obstacleColliders(reg, isWall);
        std::vector<Engine::Entity> blown;

        reg.each<Missile, gameLogic::Vec2Pos, gameLogic::Vec2Hitbox>(
            [&](const Engine::Entity e, const Missile &,
                const gameLogic::Vec2Pos &pos,
                const gameLogic::Vec2Hitbox &box) {
                if (shouldDetonate({e, pos, box, Team{}}, enemies, walls)) {
                    blown.push_back(e);
                }
            });
        for (const Engine::Entity e : blown) {
            detonate(reg, e);
        }
    }
}
