#pragma once

#include <array>
#include <cstddef>
#include "MapList.hpp"

namespace rtype {
    enum class Zone {
        DeepSpace, Nebula, Asteroids, Wreckage, Core, Warp, Arena, Count
    };

    inline constexpr std::array<Zone, 20> CAMPAIGN_ZONES = {
        Zone::DeepSpace, Zone::DeepSpace, Zone::DeepSpace, Zone::DeepSpace,
        Zone::Nebula, Zone::Nebula, Zone::Nebula, Zone::Nebula,
        Zone::Asteroids, Zone::Warp, Zone::Asteroids, Zone::Asteroids,
        Zone::Wreckage, Zone::Wreckage, Zone::Warp, Zone::Wreckage,
        Zone::Core, Zone::Core, Zone::Core, Zone::Core};

    constexpr std::size_t DEATH_RUN_LEVELS = 4;

    // La zone dont le decor accompagne ce niveau de cette map.
    inline Zone zoneOf(const MapId map, const std::size_t level)
    {
        if (map == 1) {
            return Zone::Arena;
        }
        if (map == 2) {
            if (level <= DEATH_RUN_LEVELS) {
                return Zone::Warp;
            }
            return Zone::Core;
        }
        if (level >= 1 && level <= CAMPAIGN_ZONES.size()) {
            return CAMPAIGN_ZONES[level - 1];
        }
        return Zone::Core;
    }
}
