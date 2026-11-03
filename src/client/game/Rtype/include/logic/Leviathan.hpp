#pragma once

#include <algorithm>
#include <cmath>
#include <vector>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "BossDefs.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Factories.hpp"
#include "MonsterBehavior.hpp"
#include "Queries.hpp"

namespace rtype {
    inline Engine::Entity spawnStrikeBody(Engine::Registry &reg,
        const StrikeSpec &spec, const ObstacleKind kind, const float vy)
    {
        const Engine::Entity body = createObstacle(reg,
            {0.f, spec.y, spec.width, spec.height, kind});

        reg.get<gameLogic::Vec2Pos>(body).x = spec.x;
        reg.get<gameLogic::Vec2Velocity>(body) =
            gameLogic::Vec2Velocity{{0.f, vy}};
        return body;
    }

    inline void spawnStrike(Engine::Registry &reg, const StrikeSpec &spec)
    {
        const Engine::Entity body =
            spawnStrikeBody(reg, spec, ObstacleKind::BeamWarn, 0.f);

        reg.add(body, Strike{spec.warn, spec, false});
    }

    inline void armStrike(Engine::Registry &reg, const StrikeSpec &spec)
    {
        const Engine::Entity body =
            spawnStrikeBody(reg, spec, ObstacleKind::Beam, spec.vy);

        reg.add(body, Strike{spec.active, spec, true});
    }

    inline void strikeSystem(Engine::Registry &reg, const float dt)
    {
        std::vector<Engine::Entity> done;

        reg.each<Strike>([&done, dt](const Engine::Entity e, Strike &strike) {
            strike.time -= dt;
            if (strike.time <= 0.f) {
                done.push_back(e);
            }
        });
        for (const Engine::Entity e : done) {
            const Strike strike = reg.get<Strike>(e);

            reg.destroy(e);
            if (!strike.deadly && strike.spec.active > 0.f) {
                armStrike(reg, strike.spec);
            }
        }
    }

    // 0 au debut du combat, 2 quand il lui reste moins d'un tiers de vie.
    inline std::size_t leviathanPhase(Engine::Registry &reg,
        const Engine::Entity e)
    {
        const float max = static_cast<float>(bossDef(BossKind::Fortress).hp);
        const float ratio = static_cast<float>(reg.get<Health>(e).hp) / max;

        if (ratio > 2.f / 3.f) {
            return 0;
        }
        if (ratio > 1.f / 3.f) {
            return 1;
        }
        return 2;
    }

    // Des colonnes annoncees par une zone rouge ; elles changent de place a
    // chaque salve pour toujours laisser des couloirs larges.
    inline void spawnColumns(Engine::Registry &reg, const std::size_t phase,
        const int step)
    {
        const int count = 4 + static_cast<int>(phase);
        const float spacing = config::COLUMN_RANGE
            / static_cast<float>(count);
        float shift = 0.f;

        if (step % 8 >= 4) {
            shift = spacing / 2.f;
        }
        for (int i = 0; i < count; ++i) {
            spawnStrike(reg, {config::COLUMN_FIRST + shift
                + static_cast<float>(i) * spacing, 0.f, config::COLUMN_WIDTH,
                config::MAP_HEIGHT, config::LEVIATHAN_WARN[phase],
                config::COLUMN_ACTIVE, 0.f});
        }
    }

    // Deux murs de rayons qui laissent un passage ; le passage descend ou
    // monte, il faut le suivre.
    inline void spawnSweep(Engine::Registry &reg, const std::size_t phase,
        const int step)
    {
        const float half = config::SWEEP_GAP / 2.f;
        float from = half + config::SWEEP_MARGIN;
        float to = config::MAP_HEIGHT - from;

        if (step % 8 >= 4) {
            std::swap(from, to);
        }
        const float time = config::LEVIATHAN_SWEEP_TIME[phase];
        const float vy = (to - from) / time;
        const StrikeSpec top{0.f, from - half - config::MAP_HEIGHT,
            config::MAP_WIDTH, config::MAP_HEIGHT,
            config::LEVIATHAN_WARN[phase], time, vy};
        StrikeSpec bottom = top;

        bottom.y = from + half;
        spawnStrike(reg, top);
        spawnStrike(reg, bottom);
    }

    // La zone rouge montre le couloir exact de la charge.
    inline void spawnChargeLane(Engine::Registry &reg, const Engine::Entity e)
    {
        const gameLogic::Vec2Pos pos = reg.get<gameLogic::Vec2Pos>(e);
        const gameLogic::Vec2Hitbox box = reg.get<gameLogic::Vec2Hitbox>(e);

        spawnStrike(reg, {0.f, pos.y, pos.x, box.y,
            config::LEVIATHAN_WINDUP, 0.f, 0.f});
    }

    inline float bossStopX(const gameLogic::Vec2Hitbox &box)
    {
        return config::MAP_WIDTH - box.x - config::BOSS_RIGHT_MARGIN;
    }

    inline float swayTarget(const gameLogic::Vec2Hitbox &box,
        const float swing)
    {
        const float middle = (config::MAP_HEIGHT - box.y) / 2.f;

        return middle + (middle - config::LEVIATHAN_MARGIN_Y)
            * std::sin(swing);
    }

    inline float swayVelocity(const gameLogic::Vec2Hitbox &box,
        const float swing, const float frequency, const float y)
    {
        const float middle = (config::MAP_HEIGHT - box.y) / 2.f
            - config::LEVIATHAN_MARGIN_Y;

        return middle * frequency * std::cos(swing)
            + config::LEVIATHAN_CORRECTION * (swayTarget(box, swing) - y);
    }

    inline float aimVelocity(Engine::Registry &reg, const Engine::Entity e)
    {
        const gameLogic::Vec2 center = centerOf(reg, e);
        const auto target = nearestPlayerCenter(reg, center);

        if (!target) {
            return 0.f;
        }
        return std::clamp(config::LEVIATHAN_CORRECTION
            * (target->y - center.y), -config::LEVIATHAN_AIM_SPEED,
            config::LEVIATHAN_AIM_SPEED);
    }

    // Vitesse horizontale : il reste contre le bord, sauf en charge.
    inline float horizontalVelocity(const LeviathanState &state, const float x,
        const float stopX)
    {
        if (state.move == LeviathanMove::Dash) {
            return -config::LEVIATHAN_DASH_SPEED;
        }
        if (state.move == LeviathanMove::Return) {
            return config::LEVIATHAN_RETURN_SPEED;
        }
        return config::LEVIATHAN_CORRECTION * (stopX - x);
    }

    inline float verticalVelocity(Engine::Registry &reg,
        const Engine::Entity e, const LeviathanState &state,
        const std::size_t phase)
    {
        const auto box = reg.get<gameLogic::Vec2Hitbox>(e);
        const float y = reg.get<gameLogic::Vec2Pos>(e).y;

        if (state.move == LeviathanMove::Aim) {
            return aimVelocity(reg, e);
        }
        if (state.move == LeviathanMove::Windup
            || state.move == LeviathanMove::Dash
            || state.move == LeviathanMove::Return) {
            return 0.f;
        }
        return swayVelocity(box, state.swing, config::LEVIATHAN_SWAY[phase],
            y);
    }

    inline void leviathanMotion(Engine::Registry &reg, const Engine::Entity e,
        LeviathanState &state, const std::size_t phase)
    {
        const auto box = reg.get<gameLogic::Vec2Hitbox>(e);
        const float x = reg.get<gameLogic::Vec2Pos>(e).x;

        reg.get<gameLogic::Vec2Velocity>(e) = gameLogic::Vec2Velocity{{
            horizontalVelocity(state, x, bossStopX(box)),
            verticalVelocity(reg, e, state, phase)}};
        if (state.move == LeviathanMove::Dash
            && x <= config::LEVIATHAN_DASH_END_X) {
            state.move = LeviathanMove::Return;
            state.timer = config::LEVIATHAN_RETURN_MAX;
        }
        if (state.move == LeviathanMove::Return && x >= bossStopX(box)) {
            state.move = LeviathanMove::Rest;
            state.timer = config::LEVIATHAN_REST[phase];
        }
    }

    inline void startMove(Engine::Registry &reg, LeviathanState &state,
        const std::size_t phase)
    {
        const int pick = state.step % 4;
        const float warn = config::LEVIATHAN_WARN[phase];

        if (pick == 0) {
            state = {LeviathanMove::Columns,
                warn + config::COLUMN_ACTIVE + 0.3f, state.swing, state.step};
            spawnColumns(reg, phase, state.step);
        } else if (pick == 2) {
            state = {LeviathanMove::Sweep, warn
                + config::LEVIATHAN_SWEEP_TIME[phase] + 0.3f, state.swing,
                state.step};
            spawnSweep(reg, phase, state.step);
        } else {
            state = {LeviathanMove::Aim, config::LEVIATHAN_AIM_TIME,
                state.swing, state.step};
        }
        ++state.step;
    }

    inline void nextMove(Engine::Registry &reg, const Engine::Entity e,
        LeviathanState &state, const std::size_t phase)
    {
        if (state.move == LeviathanMove::Rest) {
            startMove(reg, state, phase);
        } else if (state.move == LeviathanMove::Aim) {
            state.move = LeviathanMove::Windup;
            state.timer = config::LEVIATHAN_WINDUP;
            spawnChargeLane(reg, e);
        } else if (state.move == LeviathanMove::Windup) {
            state.move = LeviathanMove::Dash;
            state.timer = config::LEVIATHAN_DASH_MAX;
        } else if (state.move == LeviathanMove::Dash) {
            state.move = LeviathanMove::Return;
            state.timer = config::LEVIATHAN_RETURN_MAX;
        } else {
            state.move = LeviathanMove::Rest;
            state.timer = config::LEVIATHAN_REST[phase];
        }
    }

    // Les tirs en rafale du boss restent ceux de la forteresse.
    inline void leviathanFire(Engine::Registry &reg, const Engine::Entity e)
    {
        const float phase = std::fmod(reg.get<BossTag>(e).age,
            config::LEVIATHAN_BURST_PERIOD);

        if (phase >= config::LEVIATHAN_BURST_PERIOD
            - config::LEVIATHAN_BURST_DURATION) {
            Weapon &weapon = reg.get<Weapon>(e);

            weapon.timer = std::min(weapon.timer,
                config::LEVIATHAN_BURST_COOLDOWN);
        }
    }

    inline void leviathanBoss(Engine::Registry &reg, const Engine::Entity e,
        const float dt)
    {
        LeviathanState state = reg.get<LeviathanState>(e);
        const std::size_t phase = leviathanPhase(reg, e);

        reg.get<BossTag>(e).age += dt;
        state.swing += config::LEVIATHAN_SWAY[phase] * dt;
        state.timer -= dt;
        leviathanMotion(reg, e, state, phase);
        if (state.timer <= 0.f) {
            nextMove(reg, e, state, phase);
        }
        reg.get<LeviathanState>(e) = state;
        leviathanFire(reg, e);
    }
}
