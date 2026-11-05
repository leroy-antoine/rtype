//
// Created by antoine on 10/30/26.
//

#pragma once
#include <algorithm>
#include <limits>
#include <cstddef>
#include <utility>
#include <vector>

#include "Config.hpp"
#include "Kinds.hpp"
#include "Waves.hpp"

namespace rtype {
    struct Segments {
        std::vector<WaveStep> waves;
        std::vector<std::pair<BossKind, float>> bossSpawn;
        std::vector<std::pair<std::size_t, float>> levels; //where size_t is the id of the level
        std::vector<ObstacleStep> obstacles;
        std::vector<std::size_t> deathRuns;
    };

    inline Segments firstLevel()
    {
        return Segments{
            std::vector<WaveStep>(WAVES.begin(), WAVES.end()),
            {{BossKind::Standard, config::BOSS_SPAWN_SCROLL}},
            {{1, 0.f}}};
    }

    inline std::size_t levelAt(const Segments &segments, const float scroll)
    {
        std::size_t level = 0;

        for (const auto &[number, start] : segments.levels) {
            if (start <= scroll) {
                level = number;
            }
        }
        return level;
    }

    inline bool isDeathRun(const Segments &segments, const float scroll)
    {
        const std::size_t number = levelAt(segments, scroll);

        return std::ranges::find(segments.deathRuns, number)
            != segments.deathRuns.end();
    }

    inline float levelStart(const Segments &segments, const float scroll)
    {
        float start = 0.f;

        for (const auto &[number, from] : segments.levels) {
            if (from <= scroll) {
                start = from;
            }
        }
        return start;
    }

    inline float levelStartOf(const Segments &segments,
        const std::size_t number)
    {
        for (const auto &[id, from] : segments.levels) {
            if (id == number) {
                return from;
            }
        }
        return 0.f;
    }

    inline float levelEnd(const Segments &segments, const float start)
    {
        float end = std::numeric_limits<float>::max();

        for (const auto &[number, from] : segments.levels) {
            if (from > start) {
                end = std::min(end, from);
            }
        }
        return end;
    }

    inline float lastSpawnPoint(const Segments &segments)
    {
        float last = 0.f;

        for (const WaveStep &wave : segments.waves) {
            last = std::max(last, wave.at);
        }
        for (const auto &boss : segments.bossSpawn) {
            last = std::max(last, boss.second);
        }
        return last;
    }

    inline float levelSpan(const Segments &segments, const float scroll)
    {
        const float start = levelStart(segments, scroll);

        for (const auto &[number, from] : segments.levels) {
            if (from > start) {
                return from - start;
            }
        }
        return lastSpawnPoint(segments) - start;
    }
}
