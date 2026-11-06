#pragma once
#include <vector>
#include "Components.hpp"
#include "Config.hpp"
#include "Damages.hpp"
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"

namespace rtype {
    struct Collider {
        Engine::Entity entity;
        gameLogic::Vec2Pos pos;
        gameLogic::Vec2Hitbox box;
        Team team;
    };


    inline bool isOutOfBounds(const Collider &entity)
    {
        gameLogic::Vec2Pos const center = {
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

        reg.each<gameLogic::Vec2Pos, gameLogic::Vec2Hitbox, Team, Health>(
            [&](const Engine::Entity e, const gameLogic::Vec2Pos &pos, const gameLogic::Vec2Hitbox &box,
                const Team &team, const Health &) {
                targets.push_back({e, pos, box, team});
            });
        return targets;
    }

    inline bool isAlive(const Engine::Registry &reg, const Engine::Entity e)
    {
        return reg.has<Health>(e) && reg.get<Health>(e).hp > 0;
    }

    inline bool isFriendlyHit(Engine::Registry &reg, const Collider &bullet,
        const Collider &target)
    {
        if (!reg.has<Shooter>(bullet.entity)
            || reg.get<Shooter>(bullet.entity).owner == target.entity) {
            return false;
        }
        return bullet.team.id == TeamId::Player
            && target.team.id == TeamId::Player && overlaps(bullet, target);
    }

    inline bool canStrike(Engine::Registry &reg, const Collider &bullet,
        const Collider &target, const bool friendlyFire)
    {
        if (canHit(bullet, target)) {
            return true;
        }
        return friendlyFire && isFriendlyHit(reg, bullet, target);
    }

    inline bool hitFirstTarget(Engine::Registry &reg, const Collider &bullet,
        const std::vector<Collider> &targets, const bool friendlyFire = false)
    {
        for (const Collider &target : targets) {
            if (isAlive(reg, target.entity)
                && canStrike(reg, bullet, target, friendlyFire)) {
                applyDamages(target.entity, bullet.entity, reg);
                return true;
            }
        }
        return false;
    }

    inline void applyContact(Engine::Registry &reg, const Collider &player,
                             const Collider &monster)
    {
        if (!isAlive(reg, player.entity) || !isAlive(reg, monster.entity)
            || !overlaps(player, monster)) {
            return;
        }
        reg.get<Health>(player.entity).hp = 0;
        if (!reg.has<BossTag>(monster.entity)) {
            applyDamages(monster.entity, player.entity, reg);
        }
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
