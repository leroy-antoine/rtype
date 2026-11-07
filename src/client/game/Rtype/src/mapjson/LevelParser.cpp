#include <algorithm>
#include "Names.hpp"
#include "Parsers.hpp"

namespace rtype::mapjson {
    namespace {
        // The game spawns the elements in the order of the lists.
        std::vector<WaveStep> readWaves(const Reader &level,
            const float length)
        {
            std::vector<WaveStep> waves;

            for (const Reader &item : level.itemsOr("waves", MAX_WAVES)) {
                waves.push_back(parseWave(item, length));
            }
            std::ranges::stable_sort(waves, {}, &WaveStep::at);
            return waves;
        }

        std::vector<BossSpawn> readBosses(const Reader &level,
            const float length)
        {
            std::vector<BossSpawn> bosses;

            for (const Reader &item : level.itemsOr("bosses", MAX_BOSSES)) {
                bosses.push_back(parseBoss(item, length));
            }
            std::ranges::stable_sort(bosses, {}, &BossSpawn::second);
            return bosses;
        }

        std::vector<ObstacleStep> readObstacles(const Reader &level,
            const float length)
        {
            const std::vector<Reader> items =
                level.itemsOr("obstacles", MAX_OBSTACLES);
            std::vector<ObstacleStep> obstacles;

            for (const Reader &item : items) {
                parseObstacle(item, length, obstacles);
            }
            std::ranges::stable_sort(obstacles, {}, &ObstacleStep::at);
            return obstacles;
        }
    }

    ParsedLevel parseLevel(const Reader &level)
    {
        ParsedLevel parsed{};

        level.allow({"length", "zone", "deathRun", "waves", "bosses",
            "obstacles"});
        parsed.level.length = level.number("length", LENGTH);
        parsed.zone = level.choiceOr("zone", ZONE_NAMES, Zone::DeepSpace);
        parsed.level.deathRun = level.flagOr("deathRun", false);
        parsed.level.waves = readWaves(level, parsed.level.length);
        parsed.level.bosses = readBosses(level, parsed.level.length);
        parsed.level.obstacles = readObstacles(level, parsed.level.length);
        return parsed;
    }
}
