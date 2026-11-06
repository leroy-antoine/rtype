#pragma once

#include <algorithm>
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
    inline bool overlapsVertically(const Collider &a, const Collider &b)
    {
        return a.pos.y < b.pos.y + b.box.y && b.pos.y < a.pos.y + a.box.y;
    }

    // Le rayon s'arrete au premier mur qu'il rencontre sur sa ligne.
    inline float laserEnd(Engine::Registry &reg, const Collider &lane)
    {
        float end = config::MAP_WIDTH;

        for (const Collider &wall : obstacleColliders(reg, isWall)) {
            const bool ahead = wall.pos.x >= lane.pos.x;

            if (ahead && overlapsVertically(lane, wall)) {
                end = std::min(end, wall.pos.x);
            }
        }
        return end;
    }

    // Tout ce que le rayon traverse subit la frappe, une seule fois.
    inline void burn(Engine::Registry &reg, const Engine::Entity beam)
    {
        const Collider body{beam, reg.get<gameLogic::Vec2Pos>(beam),
            reg.get<gameLogic::Vec2Hitbox>(beam), Team{}};

        for (const Collider &target : collectTargets(reg)) {
            if (target.team.id == TeamId::Monster
                && isAlive(reg, target.entity) && overlaps(body, target)) {
                applyDamages(target.entity, beam, reg);
            }
        }
    }

    inline Engine::Entity createLaser(Engine::Registry &reg,
        const gameLogic::Vec2Pos &start, const float length, const int damage)
    {
        const Engine::Entity beam = reg.create();

        reg.add(beam, start);
        reg.add(beam, gameLogic::Vec2Hitbox{{length, config::LASER_HEIGHT}});
        reg.add(beam, Damage{damage});
        reg.add(beam, Team{TeamId::Player});
        reg.add(beam, Renderable{SpriteId::PlayerLaser});
        reg.add(beam, Lifetime{config::LASER_DURATION,
            config::LASER_DURATION});
        return beam;
    }

    // Le rayon part du nez du vaisseau jusqu'au premier mur ou au bord.
    inline bool fireLaser(Engine::Registry &reg, const Engine::Entity shooter)
    {
        if (!isShooter(reg, shooter)) {
            return false;
        }
        const gameLogic::Vec2Pos pos = reg.get<gameLogic::Vec2Pos>(shooter);
        const gameLogic::Vec2Hitbox box =
            reg.get<gameLogic::Vec2Hitbox>(shooter);
        const gameLogic::Vec2Pos start{{pos.x + box.x,
            pos.y + box.y / 2.f - config::LASER_HEIGHT / 2.f}};
        const Collider lane{shooter, start,
            {{config::MAP_WIDTH, config::LASER_HEIGHT}}, Team{}};
        const int damage = reg.get<Damage>(shooter).value
            * config::LASER_DAMAGE_FACTOR;
        const Engine::Entity beam = createLaser(reg, start,
            laserEnd(reg, lane) - start.x, damage);

        burn(reg, beam);
        return true;
    }
}
