#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <random>
#include "DeathRuns.hpp"
#include "LevelBuilder.hpp"

namespace rtype {
    // Les niveaux normaux : de l'ordre d'une dizaine d'obstacles, repartis sur
    // tout le niveau (asteroides qui volent, lasers qui ne touchent que les
    // joueurs), sans jamais en poser dans les 200 dernieres unites avant un
    // boss.
    inline constexpr int SCATTER_TARGET = 10;
    inline constexpr float SCATTER_FROM = 250.f;
    inline constexpr float SCATTER_CLEARANCE = 180.f;

    inline float scatterLimit(const LevelDef &level)
    {
        float limit = level.length - 300.f;

        for (const BossSpawn &boss : level.bosses) {
            limit = std::min(limit, boss.second - 200.f);
        }
        return limit;
    }

    inline bool crowded(const std::vector<ObstacleStep> &walls, const float at)
    {
        return std::ranges::any_of(walls, [at](const ObstacleStep &step) {
            return std::abs(step.at - at) < SCATTER_CLEARANCE;
        });
    }

    inline ObstacleStep asteroid(std::mt19937 &rng, const float at)
    {
        const float size = 90.f + static_cast<float>(rng() % 90);
        const float y = 80.f + static_cast<float>(rng()
            % static_cast<unsigned>(config::MAP_HEIGHT - 160.f - size));
        ObstacleStep rock = wall(at, y, size, size);

        if (rng() % 2 == 0) {
            const float speed = 1.f + static_cast<float>(rng() % 6) / 10.f;
            const Motion drift{60.f + static_cast<float>(rng() % 90),
                4.f + static_cast<float>(rng() % 3)};

            rock = withMotion(rock, drift, speed);
        }
        return rock;
    }

    inline int scatterLasers(const int number)
    {
        if (number < 3) {
            return 0;
        }
        return std::min(3, 1 + (number - 3) / 6);
    }

    inline void scatter(LevelDef &level, const unsigned number)
    {
        std::mt19937 rng(number * 7919u);
        const float limit = scatterLimit(level);
        const int missing = SCATTER_TARGET
            - static_cast<int>(level.obstacles.size());
        const int lasers = scatterLasers(static_cast<int>(number));
        const int every = std::max(1, missing / (lasers + 1));
        int placed = 0;

        for (int i = 0; i < missing && limit > SCATTER_FROM; ++i) {
            const float at = SCATTER_FROM + (static_cast<float>(i) + 0.5f)
                * (limit - SCATTER_FROM) / static_cast<float>(missing);

            if (crowded(level.obstacles, at)) {
                continue;
            }
            if (placed < lasers && (i + 1) % every == 0) {
                level.obstacles.push_back(laser(at));
                ++placed;
            } else {
                level.obstacles.push_back(asteroid(rng, at));
            }
        }
        sortWalls(level.obstacles);
    }
}
