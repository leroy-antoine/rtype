#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace rtype {
    using MapId = std::uint8_t;

    inline constexpr std::array<const char *, 3> MAP_NAMES = {
        "CAMPAIGN", "BOSS RUSH", "DEATH RUNS"};

    inline constexpr std::size_t MAP_COUNT = MAP_NAMES.size();

    // Identifiant des maps ecrites par les joueurs : il vient
    // apres les maps du jeu et n'a pas de nom dans MAP_NAMES.
    inline constexpr MapId CUSTOM_MAP = static_cast<MapId>(MAP_COUNT);
}
