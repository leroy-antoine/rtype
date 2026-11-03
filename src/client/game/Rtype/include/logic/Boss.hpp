#pragma once

#include <cmath>
#include <vector>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Bullet.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Queries.hpp"

namespace rtype {
    // Le boss entre par la droite, s'arrete a BOSS_STOP_X, puis monte et
    // descend (sinus) en continuant a tirer.
    inline void bossSystem(Engine::Registry &reg, const float dt)
    {
        for (const Engine::Entity e : collectWith<BossTag>(reg)) {
            if (reg.get<gameLogic::Vec2Pos>(e).x > config::BOSS_STOP_X) {
                continue;
            }
            BossTag &boss = reg.get<BossTag>(e);
            auto &velocity = reg.get<gameLogic::Vec2Velocity>(e);
            constexpr float amplitude = config::BOSS_SWAY_AMPLITUDE;
            constexpr float frequency = config::BOSS_SWAY_FREQUENCY;

            boss.age += dt;
            velocity.x = 0.f;
            velocity.y = amplitude * frequency
                * std::cos(frequency * boss.age);
        }
    }

    // Les vitesses des balles d'un tir du boss : un eventail vers la gauche.
    inline std::vector<gameLogic::Vec2> bossFan()
    {
        return fan(config::BOSS_FAN_BULLETS, config::BOSS_FAN_ANGLE,
            config::BOSS_BULLET_SPEED, -1.f);
    }
}
