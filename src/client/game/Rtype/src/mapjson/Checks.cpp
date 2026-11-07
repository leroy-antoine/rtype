#include <string>
#include "Parsers.hpp"

namespace rtype::mapjson {
    namespace {
        struct Totals {
            int monsters = 0;
            std::size_t bosses = 0;
            std::size_t obstacles = 0;
        };

        Totals countAll(const std::vector<LevelDef> &levels)
        {
            Totals totals;

            for (const LevelDef &level : levels) {
                for (const WaveStep &wave : level.waves) {
                    totals.monsters += wave.count;
                }
                totals.bosses += level.bosses.size();
                totals.obstacles += level.obstacles.size();
            }
            return totals;
        }

        [[noreturn]] void tooMany(const std::string &what,
            const std::size_t limit)
        {
            throw MapError("map: more than " + std::to_string(limit) + " "
                + what + " in total");
        }
    }

    // The run ends when the last boss is dead, so a map without boss would
    // be won at once.
    void checkTotals(const std::vector<LevelDef> &levels)
    {
        const Totals totals = countAll(levels);

        if (totals.bosses == 0) {
            throw MapError("map: needs at least one boss, the run ends "
                "when the last boss dies");
        }
        if (totals.monsters > MAX_MONSTERS_TOTAL) {
            tooMany("monsters", MAX_MONSTERS_TOTAL);
        }
        if (totals.bosses > MAX_BOSSES_TOTAL) {
            tooMany("bosses", MAX_BOSSES_TOTAL);
        }
        if (totals.obstacles > MAX_OBSTACLES_TOTAL) {
            tooMany("obstacles", MAX_OBSTACLES_TOTAL);
        }
    }
}
