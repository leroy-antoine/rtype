#pragma once

#include <array>
#include <cstddef>
#include <vector>
#include "MapList.hpp"

namespace rtype {
    enum class Zone {
        DeepSpace, Nebula, Asteroids, Wreckage, Core, Warp, Arena, Count
    };

    // Quatre ambiances de cinq niveaux, une par boss ; les death runs
    // (10, 15) ont la leur.
    inline constexpr std::array<Zone, 20> CAMPAIGN_ZONES = {
        Zone::DeepSpace, Zone::DeepSpace, Zone::DeepSpace, Zone::DeepSpace,
        Zone::DeepSpace, Zone::Nebula, Zone::Nebula, Zone::Nebula,
        Zone::Nebula, Zone::Warp, Zone::Wreckage, Zone::Wreckage,
        Zone::Wreckage, Zone::Wreckage, Zone::Warp, Zone::Core,
        Zone::Core, Zone::Core, Zone::Core, Zone::Core};

    // Dans la nebuleuse, des meteorites tombent entre les vagues.
    inline float stormInterval(const Zone zone)
    {
        if (zone == Zone::Nebula) {
            return 2.5f;
        }
        return 0.f;
    }

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

    // Une map de joueur donne la zone de chaque niveau, dans l'ordre.
    inline Zone customZone(const std::vector<Zone> &zones,
        const std::size_t level)
    {
        if (level >= 1 && level <= zones.size()) {
            return zones[level - 1];
        }
        return Zone::DeepSpace;
    }
}
