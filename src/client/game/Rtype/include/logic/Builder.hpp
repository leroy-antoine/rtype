#pragma once

#include <array>
#include <cstddef>
#include <optional>
#include <vector>
#include "LevelBuilder.hpp"
#include "Levels.hpp"
#include "MapList.hpp"

namespace rtype {
    using MapFn = std::vector<LevelDef> (*)();

    std::vector<LevelDef> map1();
    std::vector<LevelDef> map2();
    std::vector<LevelDef> map3();

    inline constexpr std::array<MapFn, MAP_COUNT> MAPS = {map1, map2, map3};

    inline void appendLevel(Segments &segments, const LevelDef &level,
        const std::size_t number, const float offset)
    {
        for (const WaveStep &step : level.waves) {
            segments.waves.push_back(shifted(step, offset));
        }
        for (const BossSpawn &boss : level.bosses) {
            segments.bossSpawn.push_back(shifted(boss, offset));
        }
        for (const ObstacleStep &step : level.obstacles) {
            segments.obstacles.push_back(shifted(step, offset));
        }
        segments.levels.emplace_back(number, offset);
        if (level.deathRun) {
            segments.deathRuns.push_back(number);
        }
    }

    inline Segments buildRun(const std::vector<LevelDef> &levels)
    {
        Segments segments;
        float offset = 0.f;
        std::size_t number = 1;

        for (const LevelDef &level : levels) {
            appendLevel(segments, level, number, offset);
            offset += level.length;
            ++number;
        }
        return segments;
    }

    // id vient du reseau, en gros les joueurs choisissent leur map, et le serveur nous l'envoie
    inline std::optional<Segments> buildMap(const std::size_t id)
    {
        if (id >= MAPS.size()) {
            return std::nullopt;
        }
        return buildRun(MAPS[id]());
    }
}
