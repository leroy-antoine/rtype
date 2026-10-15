#pragma once

#include <vector>
#include <algorithm>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Components.hpp"
#include "Colisions.hpp"
#include "Movements.hpp"


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
            reg.destroy(e);
        }
    }

    inline void movementSystem(Engine::Registry &reg, const float dt)
    {
        reg.each<Vec2Pos, gameLogic::Vec2Velocity>([dt](Engine::Entity, Vec2Pos &pos, const gameLogic::Vec2Velocity &vel) {
                pos = Vec2Pos{calculatePos(pos, vel, dt)};
            });
    }
}
