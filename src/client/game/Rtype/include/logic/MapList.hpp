#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace rtype {
    using MapId = std::uint8_t;

    inline constexpr std::array<const char *, 3> MAP_NAMES = {
        "CAMPAIGN", "BOSS RUSH", "DEATH RUNS"};

    inline constexpr std::size_t MAP_COUNT = MAP_NAMES.size();
}
