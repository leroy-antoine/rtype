#include "logic/Builder.hpp"
#include "logic/MapHelpers.hpp"

namespace rtype {
    namespace {
        using BK = BossKind;

        constexpr ObstacleKind SPIKE = ObstacleKind::Spike;
        constexpr ObstacleKind FAST = ObstacleKind::FastPortal;
        constexpr ObstacleKind REVERSE = ObstacleKind::ReversePortal;
        constexpr ObstacleKind NORMAL = ObstacleKind::NormalPortal;
        constexpr Motion TREMBLE{70.f, 2.4f};
        constexpr Motion SWAY{110.f, 2.2f};
        constexpr Motion SWEEP{240.f, 2.0f};
        constexpr Motion QUICK_SWEEP{260.f, 1.7f};
        constexpr Motion PINCH{70.f, 1.5f};

        struct Run {
            float length;
            GateStyle style;
            std::vector<Gap> gaps;
            std::vector<PortalMark> portals;
            std::vector<Sweeper> sweepers;
        };

        LevelDef build(const Run &run)
        {
            LevelDef level{run.length, {}, {}, {}};

            addRun(level.obstacles, run.gaps, run.style, run.portals);
            addSweepers(level.obstacles, run.style, run.sweepers);
            return level;
        }

        LevelDef chain()
        {
            return build({4100.f, {500.f, 210.f, 80.f, SPIKE, 1.4f},
                {{540.f, 240.f}, {300.f, 240.f}, {760.f, 230.f},
                 {340.f, 230.f}, {720.f, 220.f}, {260.f, 220.f},
                 {800.f, 210.f}, {380.f, 210.f}, {680.f, 200.f},
                 {300.f, 200.f}, {760.f, 190.f}, {420.f, 190.f},
                 {640.f, 180.f}, {540.f, 180.f}},
                {{-1, FAST}, {5, REVERSE}, {8, REVERSE}, {13, NORMAL}},
                {}});
        }

        LevelDef sweepers()
        {
            return build({3900.f, {500.f, 380.f, 80.f, SPIKE, 1.5f},
                {{540.f, 260.f}, {340.f, 250.f}, {700.f, 240.f},
                 {300.f, 230.f}, {760.f, 220.f}, {420.f, 220.f},
                 {660.f, 210.f}, {540.f, 210.f}},
                {{-1, FAST}, {3, REVERSE}, {5, REVERSE}, {7, NORMAL}},
                {{0, 540.f, 300.f, SWEEP}, {1, 300.f, 280.f, SWEEP},
                 {2, 600.f, 300.f, QUICK_SWEEP}, {4, 400.f, 280.f, SWEEP},
                 {5, 540.f, 300.f, QUICK_SWEEP},
                 {6, 460.f, 280.f, QUICK_SWEEP}}});
        }

        LevelDef pinch()
        {
            return build({3500.f, {500.f, 190.f, 80.f, SPIKE, 1.9f, true},
                {{540.f, 290.f, PINCH}, {260.f, 290.f, PINCH},
                 {820.f, 290.f, PINCH}, {300.f, 280.f, PINCH},
                 {780.f, 280.f, PINCH}, {240.f, 280.f, PINCH},
                 {840.f, 270.f, PINCH}, {320.f, 270.f, PINCH},
                 {760.f, 270.f, PINCH}, {260.f, 260.f, PINCH},
                 {820.f, 260.f, PINCH}, {540.f, 260.f, PINCH}},
                {{-1, FAST}, {2, REVERSE}, {5, REVERSE}, {8, REVERSE},
                 {10, REVERSE}, {11, NORMAL}},
                {}});
        }

        LevelDef gauntlet()
        {
            return build({3800.f, {500.f, 165.f, 80.f, SPIKE, 2.0f},
                {{540.f, 240.f, TREMBLE}, {330.f, 230.f, TREMBLE},
                 {740.f, 230.f, TREMBLE}, {300.f, 220.f, SWAY},
                 {760.f, 220.f, SWAY}, {320.f, 210.f, SWAY},
                 {740.f, 210.f, SWAY}, {340.f, 200.f, TREMBLE},
                 {720.f, 200.f, TREMBLE}, {360.f, 200.f, SWAY},
                 {700.f, 190.f, SWAY}, {380.f, 190.f, TREMBLE},
                 {680.f, 190.f, TREMBLE}, {420.f, 180.f, SWAY},
                 {640.f, 180.f, SWAY}, {540.f, 180.f}},
                {{-1, FAST}, {3, REVERSE}, {7, REVERSE}, {10, REVERSE},
                 {13, REVERSE}, {15, NORMAL}},
                {}});
        }
    }

    std::vector<LevelDef> map3()
    {
        return {chain(), sweepers(), pinch(), gauntlet(),
            {2000.f, {}, {{BK::Fortress, 1500.f}, {BK::Dasher, 1700.f}}, {}}};
    }
}
