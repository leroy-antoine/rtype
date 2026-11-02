#pragma once

#include <utility>
#include <vector>
#include "Kinds.hpp"
#include "Levels.hpp"
#include "Waves.hpp"

namespace rtype {
    using BossSpawn = std::pair<BossKind, float>;

    struct LevelDef {
        float length;
        std::vector<WaveStep> waves;
        std::vector<BossSpawn> bosses;
    };

    inline WaveStep wave(const float at, const MonsterKind kind,
        const int count, const float y = RANDOM_Y, const float dy = 0.f,
        const float interval = 0.5f)
    {
        return WaveStep{at, count, y, dy, interval, kind};
    }

    inline WaveStep shifted(const WaveStep &step, const float offset)
    {
        WaveStep moved = step;

        moved.at += offset;
        return moved;
    }

    inline BossSpawn shifted(const BossSpawn &boss, const float offset)
    {
        return {boss.first, boss.second + offset};
    }
}
