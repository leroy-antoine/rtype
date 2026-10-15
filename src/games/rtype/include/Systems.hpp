#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "engine/core/Registry.hpp"
#include "utils/vectors.hpp"
#include "Components.hpp"


namespace rtype {

    using gameLogic::Vec2Hitbox;
    using gameLogic::Vec2Pos;

    struct Collider {
        Engine::Entity entity;
        Vec2Pos pos;
        Vec2Hitbox box;
        Team team;
    };

    inline void deathSystem(Engine::Registry &reg)
    {
        std::vector<Engine::Entity> dead;

        reg.each<Health>([&dead](Engine::Entity e, const Health &health) {
            if (health.hp <= 0) {
                dead.push_back(e);
            }
        });
        for (const Engine::Entity e : dead) {
            reg.destroy(e);
        }
    }

    inline void applyDamages(const Engine::Entity victim, const Engine::Entity attacker,
        Engine::Registry &reg)
    {
        if (!reg.has<Health>(victim) || !reg.has<Damage>(attacker)) {
            std::cerr << "ERROR : one of the two entities is missing a component (see Systmes.hpp line 27)" << std::endl;
            return;
        }
        reg.get<Health>(victim).hp -= reg.get<Damage>(attacker).value;
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

        reg.each<Vec2Pos, Vec2Hitbox, Team, Projectile>(
            [&](const Engine::Entity e, const Vec2Pos &pos, const Vec2Hitbox &box,
                const Team &team, const Projectile &) {
                projectiles.push_back({e, pos, box, team});
            });
        return projectiles;
    }

    inline std::vector<Collider> collectTargets(Engine::Registry &reg)
    {
        std::vector<Collider> targets;

        reg.each<Vec2Pos, Vec2Hitbox, Team, Health>(
            [&](Engine::Entity e, const Vec2Pos &pos, const Vec2Hitbox &box,
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
            reg.destroy(e);
        }
    }
}
