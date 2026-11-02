#pragma once

#include <utility>
#include <vector>
#include "Config.hpp"
#include "Kinds.hpp"
#include "Levels.hpp"
#include "Waves.hpp"

namespace rtype {
    using BossSpawn = std::pair<BossKind, float>;

    struct LevelDef {
        float length;
        std::vector<WaveStep> waves;
        std::vector<BossSpawn> bosses;
        std::vector<ObstacleStep> obstacles;
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

    inline ObstacleStep wall(const float at, const float y, const float width,
        const float height, const ObstacleKind kind = ObstacleKind::Rock)
    {
        return ObstacleStep{at, y, width, height, kind};
    }

    inline void addGate(std::vector<ObstacleStep> &walls, const float at,
        const float gapY, const float gapHeight, const float width,
        const ObstacleKind kind = ObstacleKind::Rock)
    {
        const float gapEnd = gapY + gapHeight;

        if (gapY > 0.f) {
            walls.push_back(wall(at, 0.f, width, gapY, kind));
        }
        if (gapEnd < config::MAP_HEIGHT) {
            walls.push_back(wall(at, gapEnd, width,
                config::MAP_HEIGHT - gapEnd, kind));
        }
    }

    inline ObstacleStep shifted(const ObstacleStep &step, const float offset)
    {
        ObstacleStep moved = step;

        moved.at += offset;
        return moved;
    }

    inline BossSpawn shifted(const BossSpawn &boss, const float offset)
    {
        return {boss.first, boss.second + offset};
    }
}
