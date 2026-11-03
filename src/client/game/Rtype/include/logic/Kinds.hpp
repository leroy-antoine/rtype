#pragma once

namespace rtype {
    enum class MonsterKind {
        Basic, Tank, Kamikaze, Elite, Rocket, Bomber, Count
    };

    enum class ObstacleKind {
        Rock, Spike, ReversePortal, FastPortal, NormalPortal, Count
    };

    constexpr bool isPortal(const ObstacleKind kind)
    {
        return kind == ObstacleKind::ReversePortal
            || kind == ObstacleKind::FastPortal
            || kind == ObstacleKind::NormalPortal;
    }

    enum class BossKind {
        Standard, Dasher, Summoner, Fortress, Count
    };
}
