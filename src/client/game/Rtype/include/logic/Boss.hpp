#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <vector>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Bullet.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Monsters.hpp"
#include "MonsterBehavior.hpp"
#include "Queries.hpp"
#include "Leviathan.hpp"

namespace rtype {
    struct DashTarget {
        gameLogic::Vec2 pos;
        gameLogic::Vec2 velocity;
    };

    inline void swayAround(Engine::Registry &reg, const Engine::Entity e,
        const float dt, const float amplitude, const float frequency)
    {
        BossTag &boss = reg.get<BossTag>(e);
        auto &velocity = reg.get<gameLogic::Vec2Velocity>(e);

        boss.age += dt;
        velocity.x = 0.f;
        velocity.y = amplitude * frequency * std::cos(frequency * boss.age);
    }

    inline void swayBoss(Engine::Registry &reg, const Engine::Entity e,
        const float dt)
    {
        swayAround(reg, e, dt, config::BOSS_SWAY_AMPLITUDE,
            config::BOSS_SWAY_FREQUENCY);
    }

    inline int countMinions(Engine::Registry &reg)
    {
        int minions = 0;

        for (const Engine::Entity e : collectMonsters(reg)) {
            if (reg.get<MonsterState>(e).summoned) {
                ++minions;
            }
        }
        return minions;
    }

    inline void summonMinions(Engine::Registry &reg, const Engine::Entity boss)
    {
        const gameLogic::Vec2 center = centerOf(reg, boss);
        const float x = reg.get<gameLogic::Vec2Pos>(boss).x;
        const float first = -(config::SUMMON_COUNT - 1) / 2.f;

        for (int i = 0; i < config::SUMMON_COUNT; ++i) {
            const float y = center.y
                + (first + static_cast<float>(i)) * config::SUMMON_SPACING;
            const Engine::Entity minion =
                spawnMonsterAt(reg, x, y, MonsterKind::Kamikaze);

            reg.get<MonsterState>(minion).summoned = true;
        }
    }

    inline void summonerBoss(Engine::Registry &reg, const Engine::Entity e,
        const float dt)
    {
        BossTag &boss = reg.get<BossTag>(e);

        swayBoss(reg, e, dt);
        boss.summonTimer += dt;
        if (boss.summonTimer < config::SUMMON_INTERVAL
            || countMinions(reg) + config::SUMMON_COUNT
                > config::SUMMON_MAX_ALIVE) {
            return;
        }
        boss.summonTimer = 0.f;
        summonMinions(reg, e);
    }

    inline DashTarget dasherTarget(const gameLogic::Vec2Hitbox &box,
        const float age)
    {
        const float stopX = config::MAP_WIDTH - box.x
            - config::BOSS_RIGHT_MARGIN;
        const float centerX = (config::DASHER_MIN_X + stopX) / 2.f;
        const float amplitudeX = (stopX - config::DASHER_MIN_X) / 2.f;
        const float centerY = (config::MAP_HEIGHT - box.y) / 2.f;
        const float amplitudeY = centerY - config::DASHER_MARGIN_Y;
        const float phaseX = config::DASHER_FREQUENCY_X * age;
        const float phaseY = config::DASHER_FREQUENCY_Y * age;

        return {{centerX + amplitudeX * std::cos(phaseX),
                centerY + amplitudeY * std::sin(phaseY)},
            {-amplitudeX * config::DASHER_FREQUENCY_X * std::sin(phaseX),
                amplitudeY * config::DASHER_FREQUENCY_Y * std::cos(phaseY)}};
    }

    // Trajectoire de Lissajous qui parcourt tout le cote droit de la carte ;
    // la correction ramene le boss sur la courbe malgre l'integration.
    inline void roamBoss(Engine::Registry &reg, const Engine::Entity e,
        const float dt)
    {
        BossTag &boss = reg.get<BossTag>(e);
        const gameLogic::Vec2Pos pos = reg.get<gameLogic::Vec2Pos>(e);

        boss.age += dt;
        const DashTarget target =
            dasherTarget(reg.get<gameLogic::Vec2Hitbox>(e), boss.age);

        reg.get<gameLogic::Vec2Velocity>(e) = gameLogic::Vec2Velocity{{
            target.velocity.x
                + config::DASHER_CORRECTION * (target.pos.x - pos.x),
            target.velocity.y
                + config::DASHER_CORRECTION * (target.pos.y - pos.y)}};
    }

    using BossBehavior = void (*)(Engine::Registry &, Engine::Entity, float);

    inline constexpr std::array<BossBehavior,
        static_cast<std::size_t>(BossKind::Count)> BOSS_BEHAVIORS = {
        swayBoss, roamBoss, summonerBoss, leviathanBoss};

    // Le boss entre par la droite, s'arrete contre le bord droit, puis suit le
    // comportement de son type en continuant a tirer.
    inline void bossSystem(Engine::Registry &reg, const float dt)
    {
        for (const Engine::Entity e : collectWith<BossTag>(reg)) {
            const float stopX = config::MAP_WIDTH
                - reg.get<gameLogic::Vec2Hitbox>(e).x
                - config::BOSS_RIGHT_MARGIN;
            const auto kind = reg.get<BossType>(e).kind;
            const bool entered = reg.get<BossTag>(e).age > 0.f
                || reg.get<gameLogic::Vec2Pos>(e).x <= stopX;

            if (entered) {
                BOSS_BEHAVIORS[static_cast<std::size_t>(kind)](reg, e, dt);
            }
        }
    }
}
