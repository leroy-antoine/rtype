#include "logic/Builder.hpp"

namespace rtype {
    namespace {
        using MK = MonsterKind;
        using BK = BossKind;
    }

    std::vector<LevelDef> map2()
    {
        return {
            {1500.f, {wave(200.f, MK::Basic, 6, RANDOM_Y, 0.f, 0.4f)},
                {{BK::Standard, 1400.f}}, {}},
            {1800.f, {wave(200.f, MK::Elite, 5, RANDOM_Y, 0.f, 0.4f)},
                {{BK::Dasher, 1700.f}}, {}},
            {2000.f, {wave(200.f, MK::Rocket, 4, 150.f, 250.f, 0.f),
                wave(900.f, MK::Kamikaze, 8, RANDOM_Y, 0.f, 0.2f)},
                {{BK::Summoner, 1900.f}}, {}},
            {2200.f, {wave(200.f, MK::Tank, 2, 250.f, 400.f, 0.f),
                wave(800.f, MK::Bomber, 3, 150.f, 300.f, 0.f),
                wave(1400.f, MK::MiniTank, 1, 300.f, 0.f, 0.f),
                wave(1700.f, MK::MiniElite, 1, 600.f, 0.f, 0.f),
                wave(2000.f, MK::MiniBomber, 1, 400.f, 0.f, 0.f)}, {}, {}},
            {2500.f, {wave(200.f, MK::Elite, 8, RANDOM_Y, 0.f, 0.3f)},
                {{BK::Fortress, 2000.f}}, {}}};
    }
}
