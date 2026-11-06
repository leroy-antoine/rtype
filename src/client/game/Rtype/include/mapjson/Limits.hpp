#pragma once

#include <cstddef>
#include "logic/Config.hpp"

namespace rtype::mapjson {
    struct Range {
        float min;
        float max;
    };

    // Technical caps: they protect the server from huge or hostile files.
    inline constexpr std::size_t MAX_FILE_BYTES = 256 * 1024;
    inline constexpr std::size_t MAX_FILES = 200;
    inline constexpr int MAX_DEPTH = 16;
    inline constexpr std::size_t MAX_TEXT = 40;
    inline constexpr std::size_t MAX_LEVELS = 100;
    inline constexpr std::size_t MAX_WAVES = 300;
    inline constexpr std::size_t MAX_BOSSES = 20;
    inline constexpr std::size_t MAX_OBSTACLES = 2000;
    inline constexpr int MAX_MONSTERS_TOTAL = 20000;
    inline constexpr std::size_t MAX_BOSSES_TOTAL = 200;
    inline constexpr std::size_t MAX_OBSTACLES_TOTAL = 20000;

    // Distances are in map units, durations in seconds.
    inline constexpr float MIN_GAP_HEIGHT = 2.f * rtype::config::PLAYER_HEIGHT;
    inline constexpr Range LENGTH{100.f, 100000.f};
    inline constexpr Range WAVE_COUNT{1.f, 50.f};
    inline constexpr Range WAVE_Y{0.f, rtype::config::MONSTER_MAX_SPAWN_Y};
    inline constexpr Range WAVE_DY{-500.f, 500.f};
    inline constexpr Range WAVE_INTERVAL{0.f, 10.f};
    inline constexpr Range OBSTACLE_Y{-1080.f, 2160.f};
    inline constexpr Range OBSTACLE_WIDTH{1.f, 2000.f};
    inline constexpr Range OBSTACLE_HEIGHT{1.f, 2160.f};
    inline constexpr Range GAP_Y{0.f, rtype::config::MAP_HEIGHT};
    inline constexpr Range GAP_HEIGHT{MIN_GAP_HEIGHT,
        rtype::config::MAP_HEIGHT};
    inline constexpr Range AMPLITUDE{-1080.f, 1080.f};
    inline constexpr Range PERIOD{0.f, 60.f};
    inline constexpr Range SPEED{0.1f, 5.f};
}
