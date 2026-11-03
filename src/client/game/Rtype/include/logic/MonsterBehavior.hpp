#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <optional>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Queries.hpp"

namespace rtype {
    inline gameLogic::Vec2 centerOf(const Engine::Registry &reg,
        const Engine::Entity e)
    {
        const gameLogic::Vec2Pos pos = reg.get<gameLogic::Vec2Pos>(e);
        const gameLogic::Vec2Hitbox box = reg.get<gameLogic::Vec2Hitbox>(e);

        return {pos.x + box.x / 2.f, pos.y + box.y / 2.f};
    }

    inline void flyStraight(Engine::Registry &, const Engine::Entity,
        const float)
    {
    }

    inline float confinedVy(const Engine::Registry &reg,
        const Engine::Entity e, const float vy, const float dt)
    {
        if (dt <= 0.f) {
            return 0.f;
        }
        const float y = reg.get<gameLogic::Vec2Pos>(e).y;
        const float maxY = config::MAP_HEIGHT
            - reg.get<gameLogic::Vec2Hitbox>(e).y;
        const float next = std::clamp(y + vy * dt, config::MAP_ORIGIN, maxY);

        return (next - y) / dt;
    }

    inline void zigzag(Engine::Registry &reg, const Engine::Entity e,
        const float dt)
    {
        const float age = reg.get<MonsterState>(e).age;
        const float vy = config::ZIGZAG_AMPLITUDE * config::ZIGZAG_FREQUENCY
            * std::cos(config::ZIGZAG_FREQUENCY * age);

        reg.get<gameLogic::Vec2Velocity>(e).y = confinedVy(reg, e, vy, dt);
    }

    inline void lockOn(Engine::Registry &reg, const Engine::Entity e,
        const gameLogic::Vec2 &target)
    {
        const gameLogic::Vec2 from = centerOf(reg, e);
        const float speed = reg.get<Speed>(e).value;
        const float distance = std::hypot(target.x - from.x, target.y - from.y);

        reg.get<gameLogic::Vec2Velocity>(e) = gameLogic::Vec2Velocity{{
            speed * (target.x - from.x) / distance,
            speed * (target.y - from.y) / distance}};
        reg.get<MonsterState>(e).locked = true;
    }

    inline void dive(Engine::Registry &reg, const Engine::Entity e,
        const float)
    {
        const gameLogic::Vec2 from = centerOf(reg, e);
        const auto target = nearestPlayerCenter(reg, from);

        if (reg.get<MonsterState>(e).locked || !target) {
            return;
        }
        if (target->x < from.x && from.x - target->x < config::DIVE_DISTANCE) {
            lockOn(reg, e, *target);
        }
    }

    inline void chargeRocket(Engine::Registry &reg, const Engine::Entity e,
        const float)
    {
        const bool charging = reg.get<Weapon>(e).timer
            < config::ROCKET_CHARGE_TIME;
        float vx = -reg.get<Speed>(e).value;

        if (charging) {
            vx = 0.f;
        }
        reg.get<gameLogic::Vec2Velocity>(e).x = vx;
    }

    using MonsterBehavior = void (*)(Engine::Registry &, Engine::Entity,
        float);

    inline constexpr std::array<MonsterBehavior,
        static_cast<std::size_t>(MonsterKind::Count)> MONSTER_BEHAVIORS = {
        flyStraight, flyStraight, dive, zigzag, chargeRocket, flyStraight,
        flyStraight, zigzag, flyStraight};

    inline bool hasEntered(const Engine::Registry &reg, const Engine::Entity e)
    {
        return reg.get<gameLogic::Vec2Pos>(e).x < config::MAP_WIDTH;
    }

    inline void monsterSystem(Engine::Registry &reg, const float dt)
    {
        for (const Engine::Entity e : collectWith<MonsterState>(reg)) {
            if (reg.has<BossTag>(e) || !hasEntered(reg, e)) {
                continue;
            }
            const auto kind = reg.get<MonsterType>(e).kind;

            reg.get<MonsterState>(e).age += dt;
            MONSTER_BEHAVIORS[static_cast<std::size_t>(kind)](reg, e, dt);
        }
    }
}
