#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <vector>
#include "Config.hpp"
#include "Kinds.hpp"
#include "Settings.hpp"

namespace rtype {
    struct EscortSpawn {
        float delay;
        float y;
        MonsterKind kind;
    };

    inline constexpr std::array<MonsterKind, 3> ESCORT_KINDS = {
        MonsterKind::MiniTank, MonsterKind::MiniElite,
        MonsterKind::MiniBomber};

    // Le premier boss a un mini-boss, puis un de plus a chaque boss.
    inline std::size_t escortSize(const Difficulty difficulty,
        const std::size_t bossIndex)
    {
        if (difficulty != Difficulty::Brutal) {
            return 0;
        }
        return std::min(bossIndex + 1, config::ESCORT_MAX);
    }

    inline std::vector<EscortSpawn> escortFor(const Difficulty difficulty,
        const std::size_t bossIndex)
    {
        const std::size_t size = escortSize(difficulty, bossIndex);
        const float span = config::WAVE_MAX_Y - config::WAVE_MIN_Y;
        std::vector<EscortSpawn> escort;

        for (std::size_t i = 0; i < size; ++i) {
            const float slot = static_cast<float>(i + 1)
                / static_cast<float>(size + 1);

            escort.push_back({config::ESCORT_DELAY * static_cast<float>(i + 1),
                config::WAVE_MIN_Y + span * slot,
                ESCORT_KINDS[i % ESCORT_KINDS.size()]});
        }
        return escort;
    }
}
