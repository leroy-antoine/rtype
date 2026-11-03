#include "logic/Builder.hpp"
#include "logic/DeathRuns.hpp"

namespace rtype {
    namespace {
        using BK = BossKind;

        constexpr float FIRST = 150.f;
        constexpr float TAIL = 150.f;
        constexpr Motion TREMBLE{70.f, 2.4f};
        constexpr Motion PINCH{70.f, 1.5f};

        // Une death run : un parcours, sans aucun monstre.
        LevelDef run(const Course &course)
        {
            LevelDef level{0.f, {}, {}, {}, true};
            const float last = addCourse(level.obstacles, course);

            level.length = last + TAIL;
            return level;
        }

        LevelDef rhythm()
        {
            return run({FIRST, 1.3f, 270.f,
                "FGBGMBGTGBMGRGBGMBGRGTBGMGBN", 240.f, 190.f, {}, false});
        }

        LevelDef minefield()
        {
            return run({FIRST, 1.5f, 260.f,
                "FMGBMTGBMGRBGMTGBMRGMBGTGBMN", 230.f, 190.f, {}, false});
        }

        LevelDef pinch()
        {
            return run({FIRST, 1.7f, 270.f,
                "FGBGMGBTGMGRBGMGBGRTGMGBGBN", 300.f, 265.f, PINCH, true});
        }

        LevelDef gauntlet()
        {
            return run({FIRST, 2.0f, 250.f,
                "FGBMGTBGMGBRGMBTGBGMRGBMGTBGMGBN", 220.f, 180.f, TREMBLE,
                false});
        }
    }

    std::vector<LevelDef> map3()
    {
        return {rhythm(), minefield(), pinch(), gauntlet(),
            {2500.f, {}, {{BK::Fortress, 1900.f}, {BK::Dasher, 2100.f}}, {}}};
    }
}
