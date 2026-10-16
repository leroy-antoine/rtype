#pragma once
#include <vector>
#include "Components.hpp"
#include "Config.hpp"
#include "Damages.hpp"
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"

namespace rtype {
    using gameLogic::Vec2Hitbox;
    using gameLogic::Vec2Pos;

    struct Collider {
        Engine::Entity entity;
        Vec2Pos pos;
        Vec2Hitbox box;
        Team team;
    };


    inline bool isOutOfBounds(const Collider &entity)
    {
        Vec2Pos const center = {
            entity.pos.x + entity.box.x/2.f,
            entity.pos.y + entity.box.y/2.f
        };

        if (center.x >= config::MAP_WIDTH || center.x <= config::MAP_ORIGIN ||
            center.y >= config::MAP_HEIGHT || center.y <= config::MAP_ORIGIN) {
            return true;
        }
        return false;
    }

    inline bool overlaps(const Collider &a, const Collider &b)
    {
        const bool overlapX = a.pos.x < b.pos.x + b.box.x
            && b.pos.x < a.pos.x + a.box.x;
        const bool overlapY = a.pos.y < b.pos.y + b.box.y
            && b.pos.y < a.pos.y + a.box.y;

        return overlapX && overlapY;
    }

    inline bool canHit(const Collider &bullet, const Collider &target)
    {
        return bullet.team.id != target.team.id && overlaps(bullet, target);
    }

    inline std::vector<Collider> collectProjectiles(Engine::Registry &reg)
    {
        std::vector<Collider> projectiles;

        reg.each<gameLogic::Vec2Pos, gameLogic::Vec2Hitbox, Team, ProjectileTag>(
            [&](const Engine::Entity e, const gameLogic::Vec2Pos &pos, const gameLogic::Vec2Hitbox &box,
                const Team &team, const ProjectileTag &) {
                projectiles.push_back({e, pos, box, team});
            });
        return projectiles;
    }

    inline std::vector<Collider> collectTargets(Engine::Registry &reg)
    {
        std::vector<Collider> targets;

        reg.each<Vec2Pos, Vec2Hitbox, Team, Health>(
            [&](const Engine::Entity e, const Vec2Pos &pos, const Vec2Hitbox &box,
                const Team &team, const Health &) {
                targets.push_back({e, pos, box, team});
            });
        return targets;
    }

    inline bool hitFirstTarget(Engine::Registry &reg, const Collider &bullet,
                               const std::vector<Collider> &targets)
    {
        for (const Collider &target : targets) {
            if (canHit(bullet, target)) {
                applyDamages(target.entity, bullet.entity, reg);
                return true;
            }
        }
        return false;
    }

    inline bool isFarOutOfBounds(const Collider &entity)
    {
        const float margin = config::DESPAWN_MARGIN;
        const float right = entity.pos.x + entity.box.x;
        const float bottom = entity.pos.y + entity.box.y;

        return right < config::MAP_ORIGIN - margin
            || entity.pos.x > config::MAP_WIDTH + margin
            || bottom < config::MAP_ORIGIN - margin
            || entity.pos.y > config::MAP_HEIGHT + margin;
    }
}
