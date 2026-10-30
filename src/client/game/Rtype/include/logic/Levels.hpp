//
// Created by antoine on 10/30/26.
//

#pragma once
#include <cstddef>
#include <utility>
#include <vector>

#include "Config.hpp"
#include "Waves.hpp"

namespace rtype {
    struct Segments {
        std::vector<WaveStep> waves;
        std::vector<std::pair<std::size_t, float>> bossSpawn; //where size_t is the id of a boss inside an array of bosses
        std::vector<std::pair<std::size_t, float>> levels; //where size_t is the id of the level
    };

    inline Segments firstLevel()
    {
        return Segments{
            std::vector<WaveStep>(WAVES.begin(), WAVES.end()),
            {{0, config::BOSS_SPAWN_SCROLL}},
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
}
