#include "logic/Builder.hpp"

namespace rtype {
    std::vector<LevelDef> map1()
    {
        return {
            {2400.f,
                {wave(240.f, MonsterKind::Basic, 4, RANDOM_Y, 0.f, 0.7f),
                 wave(840.f, MonsterKind::Basic, 3, 300.f, 250.f, 0.f),
                 wave(1320.f, MonsterKind::Basic, 5),
                 wave(1800.f, MonsterKind::Basic, 4, 200.f, 220.f, 0.f)},
                {{BossKind::Standard, 2400.f}}},
            {2600.f,
                {wave(200.f, MonsterKind::Tank, 3),
                 wave(900.f, MonsterKind::Basic, 6, RANDOM_Y, 0.f, 0.4f),
                 wave(1600.f, MonsterKind::Tank, 4, 250.f, 200.f, 0.f)},
                {{BossKind::Dasher, 2600.f}}},
        };
    }
}
