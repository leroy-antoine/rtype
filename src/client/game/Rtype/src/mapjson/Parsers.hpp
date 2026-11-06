#pragma once

#include <string_view>
#include <vector>
#include "logic/LevelBuilder.hpp"
#include "logic/Zones.hpp"
#include "Reader.hpp"

// One parser per element of a map. `length` is the length of the level that
// holds the element: everything is placed between 0 and `length`.
namespace rtype::mapjson {
    struct ParsedLevel {
        LevelDef level;
        Zone zone = Zone::DeepSpace;
    };

    WaveStep parseWave(const Reader &wave, float length);
    BossSpawn parseBoss(const Reader &boss, float length);
    void parseObstacle(const Reader &obstacle, float length,
        std::vector<ObstacleStep> &out);
    ParsedLevel parseLevel(const Reader &level);

    // Checks that concern the whole map (totals, at least one boss).
    void checkTotals(const std::vector<LevelDef> &levels);

    // Parses the text, with a cap on its size and on its nesting.
    Json parseJson(std::string_view text);
}
