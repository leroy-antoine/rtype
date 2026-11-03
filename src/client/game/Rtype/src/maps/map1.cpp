#include "logic/Builder.hpp"
#include "logic/MapHelpers.hpp"

namespace rtype {
    namespace {
        using MK = MonsterKind;
        using BK = BossKind;

        constexpr Motion SWING{80.f, 4.f};
        constexpr Motion SLOW_SWING{60.f, 5.f};
        constexpr ObstacleKind FAST = ObstacleKind::FastPortal;
        constexpr ObstacleKind REVERSE = ObstacleKind::ReversePortal;
        constexpr ObstacleKind NORMAL = ObstacleKind::NormalPortal;

        LevelDef level1()
        {
            return {2500.f,
                {wave(240.f, MK::Basic, 4, RANDOM_Y, 0.f, 0.7f),
                 wave(840.f, MK::Basic, 3, 300.f, 250.f, 0.f),
                 wave(1320.f, MK::Basic, 5),
                 wave(1800.f, MK::Basic, 4, 200.f, 220.f, 0.f)},
                {{BK::Standard, 2400.f}}};
        }

        LevelDef level2()
        {
            return {3000.f,
                {wave(200.f, MK::Basic, 6),
                 wave(700.f, MK::Basic, 6, RANDOM_Y, 0.f, 0.4f),
                 wave(1200.f, MK::Basic, 4, 200.f, 200.f, 0.f),
                 wave(1700.f, MK::Basic, 8, RANDOM_Y, 0.f, 0.3f),
                 wave(2100.f, MK::Tank, 1, 450.f),
                 wave(2400.f, MK::Basic, 6, RANDOM_Y, 0.f, 0.35f)},
                {}};
        }

        LevelDef level3()
        {
            return {3000.f,
                {wave(200.f, MK::Kamikaze, 4, RANDOM_Y, 0.f, 0.25f),
                 wave(500.f, MK::Basic, 4),
                 wave(800.f, MK::Kamikaze, 6, RANDOM_Y, 0.f, 0.2f),
                 wave(1300.f, MK::Kamikaze, 5, 100.f, 190.f, 0.f),
                 wave(1600.f, MK::Basic, 5, RANDOM_Y, 0.f, 0.4f),
                 wave(2000.f, MK::Kamikaze, 8, RANDOM_Y, 0.f, 0.15f),
                 wave(2500.f, MK::Kamikaze, 6, RANDOM_Y, 0.f, 0.2f)},
                {}};
        }

        Walls asteroidField()
        {
            return {wall(300.f, 150.f, 128.f, 192.f),
                wall(600.f, 700.f, 192.f, 160.f),
                wall(900.f, 380.f, 128.f, 256.f),
                wall(1250.f, 100.f, 160.f, 200.f),
                wall(1500.f, 800.f, 128.f, 180.f),
                wall(1800.f, 450.f, 192.f, 192.f),
                wall(2100.f, 200.f, 128.f, 256.f),
                wall(2400.f, 650.f, 192.f, 224.f),
                wall(2700.f, 350.f, 128.f, 160.f)};
        }

        LevelDef level4()
        {
            return {3300.f,
                {wave(300.f, MK::Basic, 5),
                 wave(1000.f, MK::Basic, 6, RANDOM_Y, 0.f, 0.4f),
                 wave(1800.f, MK::Tank, 1, 500.f),
                 wave(2300.f, MK::Basic, 8, RANDOM_Y, 0.f, 0.3f)},
                {},
                asteroidField()};
        }

        LevelDef level5()
        {
            return {3200.f,
                {wave(200.f, MK::Basic, 6),
                 wave(800.f, MK::Basic, 5, 150.f, 150.f, 0.f),
                 wave(1400.f, MK::Kamikaze, 6, RANDOM_Y, 0.f, 0.25f),
                 wave(2000.f, MK::Basic, 8, RANDOM_Y, 0.f, 0.3f)},
                {{BK::Dasher, 2800.f}}};
        }

        LevelDef level6()
        {
            return {3400.f,
                {wave(200.f, MK::Elite, 4, 150.f, 220.f, 0.f),
                 wave(700.f, MK::Elite, 5),
                 wave(1200.f, MK::Basic, 8, RANDOM_Y, 0.f, 0.3f),
                 wave(1700.f, MK::Elite, 6, 100.f, 170.f, 0.f),
                 wave(2200.f, MK::Elite, 4, 900.f, -200.f, 0.f),
                 wave(2700.f, MK::Elite, 8, RANDOM_Y, 0.f, 0.3f)},
                {}};
        }

        LevelDef level7()
        {
            LevelDef level{3700.f,
                {wave(500.f, MK::Basic, 6),
                 wave(1200.f, MK::Tank, 2, 300.f, 300.f, 0.f),
                 wave(1900.f, MK::Basic, 8, RANDOM_Y, 0.f, 0.3f),
                 wave(2600.f, MK::Elite, 5)},
                {}};

            addCave(level.obstacles,
                {{120.f, 150.f}, {200.f, 240.f}, {280.f, 160.f},
                 {220.f, 300.f}, {300.f, 200.f}, {180.f, 280.f},
                 {260.f, 180.f}, {140.f, 120.f}}, 100.f, 400.f);
            return level;
        }

        LevelDef level8()
        {
            return {3400.f,
                {wave(300.f, MK::Rocket, 3, 150.f, 300.f, 0.f),
                 wave(900.f, MK::Basic, 6, RANDOM_Y, 0.f, 0.4f),
                 wave(1300.f, MK::Rocket, 4, 100.f, 240.f, 0.f),
                 wave(1700.f, MK::Tank, 2, 300.f, 300.f, 0.f),
                 wave(2100.f, MK::Rocket, 5, RANDOM_Y, 0.f, 0.8f),
                 wave(2600.f, MK::Elite, 4)},
                {}};
        }

        LevelDef level9()
        {
            return {3100.f,
                {wave(300.f, MK::Tank, 2, 250.f, 400.f, 0.f),
                 wave(800.f, MK::Basic, 8, RANDOM_Y, 0.f, 0.3f),
                 wave(1400.f, MK::Tank, 3, 150.f, 300.f, 0.f),
                 wave(1900.f, MK::Elite, 4)},
                {{BK::Summoner, 2700.f}}};
        }

        LevelDef level10()
        {
            LevelDef level{4900.f, {}, {}};

            addRun(level.obstacles,
                {{540.f, 300.f}, {340.f, 280.f}, {640.f, 280.f},
                 {300.f, 260.f}, {680.f, 260.f}, {420.f, 240.f},
                 {260.f, 240.f}, {700.f, 220.f, SLOW_SWING},
                 {380.f, 200.f, SLOW_SWING}, {620.f, 200.f, SLOW_SWING},
                 {260.f, 180.f}},
                {600.f, 380.f, 96.f, ObstacleKind::Spike},
                {{-1, FAST}, {2, REVERSE}, {5, REVERSE}, {10, NORMAL}});
            return level;
        }

        LevelDef level11()
        {
            return {3500.f,
                {wave(300.f, MK::Bomber, 2, 250.f, 450.f, 0.f),
                 wave(800.f, MK::Elite, 6, RANDOM_Y, 0.f, 0.4f),
                 wave(1300.f, MK::Bomber, 3, 150.f, 300.f, 0.f),
                 wave(1900.f, MK::Kamikaze, 8, RANDOM_Y, 0.f, 0.2f),
                 wave(2300.f, MK::Bomber, 4, 100.f, 280.f, 0.f),
                 wave(2800.f, MK::Elite, 5)},
                {}};
        }

        Walls pillars()
        {
            return {wall(400.f, 0.f, 96.f, 320.f),
                wall(800.f, 760.f, 96.f, 320.f),
                wall(1200.f, 0.f, 96.f, 420.f),
                wall(1600.f, 660.f, 96.f, 420.f),
                wall(2000.f, 300.f, 128.f, 300.f),
                wall(2400.f, 0.f, 96.f, 350.f),
                wall(2800.f, 730.f, 96.f, 350.f)};
        }

        LevelDef level12()
        {
            return {3600.f,
                {wave(500.f, MK::Rocket, 3),
                 wave(900.f, MK::Kamikaze, 6, RANDOM_Y, 0.f, 0.25f),
                 wave(1300.f, MK::Elite, 5),
                 wave(1800.f, MK::Rocket, 4, RANDOM_Y, 0.f, 0.7f),
                 wave(2200.f, MK::Tank, 2, 300.f, 300.f, 0.f),
                 wave(2600.f, MK::Kamikaze, 8, RANDOM_Y, 0.f, 0.2f),
                 wave(3000.f, MK::Elite, 6)},
                {},
                pillars()};
        }

        LevelDef level13()
        {
            return {3800.f,
                {wave(200.f, MK::Basic, 8, RANDOM_Y, 0.f, 0.3f),
                 wave(700.f, MK::Elite, 5),
                 wave(1200.f, MK::Kamikaze, 8, RANDOM_Y, 0.f, 0.2f),
                 wave(1800.f, MK::Tank, 2, 300.f, 300.f, 0.f)},
                {{BK::Standard, 2700.f}, {BK::Dasher, 3300.f}}};
        }

        Walls tankCover()
        {
            return {wall(500.f, 250.f, 160.f, 160.f),
                wall(1100.f, 680.f, 160.f, 160.f),
                wall(1700.f, 420.f, 160.f, 200.f),
                wall(2300.f, 150.f, 160.f, 160.f),
                wall(2900.f, 700.f, 160.f, 160.f)};
        }

        LevelDef level14()
        {
            return {3600.f,
                {wave(300.f, MK::Tank, 3, 200.f, 300.f, 0.f),
                 wave(900.f, MK::Bomber, 2, 250.f, 450.f, 0.f),
                 wave(1500.f, MK::Tank, 4, 100.f, 250.f, 0.f),
                 wave(2100.f, MK::Bomber, 3, 150.f, 300.f, 0.f),
                 wave(2700.f, MK::Tank, 5, 100.f, 200.f, 0.f)},
                {},
                tankCover()};
        }

        LevelDef level15()
        {
            LevelDef level{5900.f, {}, {}};

            addRun(level.obstacles,
                {{540.f, 240.f}, {360.f, 240.f}, {620.f, 230.f},
                 {280.f, 230.f}, {700.f, 220.f}, {420.f, 220.f},
                 {300.f, 240.f, SWING}, {560.f, 240.f, SWING},
                 {700.f, 240.f, SWING}, {400.f, 230.f, SWING},
                 {640.f, 230.f, SWING}, {300.f, 220.f},
                 {520.f, 200.f}, {760.f, 190.f}, {420.f, 180.f},
                 {300.f, 180.f}},
                {500.f, 320.f, 80.f, ObstacleKind::Spike},
                {{-1, FAST}, {2, REVERSE}, {5, REVERSE}, {8, REVERSE},
                 {11, REVERSE}, {15, NORMAL}});
            return level;
        }

        LevelDef level16()
        {
            return {3800.f,
                {wave(200.f, MK::Elite, 6),
                 wave(700.f, MK::Rocket, 4, 100.f, 240.f, 0.f),
                 wave(1200.f, MK::Kamikaze, 8, RANDOM_Y, 0.f, 0.2f),
                 wave(1800.f, MK::Bomber, 3, 150.f, 300.f, 0.f),
                 wave(2300.f, MK::Elite, 8, RANDOM_Y, 0.f, 0.3f)},
                {{BK::Summoner, 3000.f}, {BK::Standard, 3500.f}}};
        }

        LevelDef level17()
        {
            LevelDef level{3500.f,
                {wave(400.f, MK::Elite, 5),
                 wave(900.f, MK::Kamikaze, 6, RANDOM_Y, 0.f, 0.2f),
                 wave(1500.f, MK::Rocket, 3),
                 wave(2100.f, MK::Tank, 2, 300.f, 300.f, 0.f),
                 wave(2700.f, MK::Elite, 6)},
                {}};

            addGates(level.obstacles,
                {{540.f, 360.f}, {300.f, 360.f}, {700.f, 360.f},
                 {380.f, 340.f}, {640.f, 340.f}, {480.f, 320.f}},
                {300.f, 520.f, 120.f, ObstacleKind::Rock});
            return level;
        }

        LevelDef level18()
        {
            LevelDef level{3600.f,
                {wave(500.f, MK::Rocket, 4, RANDOM_Y, 0.f, 0.7f),
                 wave(1100.f, MK::Kamikaze, 10, RANDOM_Y, 0.f, 0.15f),
                 wave(1800.f, MK::Bomber, 2, 250.f, 450.f, 0.f),
                 wave(2500.f, MK::Rocket, 5, RANDOM_Y, 0.f, 0.6f)},
                {}};

            addCave(level.obstacles,
                {{200.f, 180.f}, {260.f, 220.f}, {320.f, 160.f},
                 {240.f, 300.f}, {300.f, 200.f}, {220.f, 280.f},
                 {340.f, 140.f}, {260.f, 240.f}}, 100.f, 400.f);
            return level;
        }

        LevelDef level19()
        {
            return {4300.f,
                {wave(200.f, MK::Elite, 6),
                 wave(600.f, MK::Tank, 3, 150.f, 300.f, 0.f),
                 wave(1000.f, MK::Kamikaze, 10, RANDOM_Y, 0.f, 0.15f),
                 wave(1500.f, MK::Bomber, 3, 150.f, 300.f, 0.f),
                 wave(2000.f, MK::Rocket, 5, RANDOM_Y, 0.f, 0.6f),
                 wave(2600.f, MK::Elite, 8, RANDOM_Y, 0.f, 0.25f)},
                {{BK::Standard, 3300.f}, {BK::Summoner, 3700.f}}};
        }

        LevelDef level20()
        {
            LevelDef level{5800.f,
                {wave(3100.f, MK::Elite, 8, RANDOM_Y, 0.f, 0.3f),
                 wave(3500.f, MK::Bomber, 3, 150.f, 300.f, 0.f),
                 wave(3800.f, MK::Kamikaze, 8, RANDOM_Y, 0.f, 0.2f)},
                {{BK::Summoner, 5000.f}, {BK::Dasher, 5200.f},
                 {BK::Fortress, 5400.f}}};

            addRun(level.obstacles,
                {{540.f, 240.f}, {300.f, 240.f}, {720.f, 230.f},
                 {360.f, 240.f, SWING}, {640.f, 240.f, SWING},
                 {320.f, 230.f, SWING}, {700.f, 220.f},
                 {500.f, 200.f}},
                {400.f, 340.f, 88.f, ObstacleKind::Spike},
                {{-1, FAST}, {1, REVERSE}, {3, REVERSE}, {7, NORMAL}});
            return level;
        }
    }

    std::vector<LevelDef> map1()
    {
        return {level1(), level2(), level3(), level4(), level5(),
            level6(), level7(), level8(), level9(), level10(),
            level11(), level12(), level13(), level14(), level15(),
            level16(), level17(), level18(), level19(), level20()};
    }
}
