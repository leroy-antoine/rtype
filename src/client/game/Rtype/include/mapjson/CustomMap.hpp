#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include "logic/LevelBuilder.hpp"
#include "logic/Zones.hpp"

namespace rtype::mapjson {
    // A map written by a player. levels[i] is played with zones[i] as scenery.
    struct CustomMap {
        std::string name;
        std::string author;
        std::vector<LevelDef> levels;
        std::vector<Zone> zones;
    };

    // Either a map, or the reason why the text is not a valid map.
    struct ParseResult {
        std::optional<CustomMap> map;
        std::string error;
    };

    // Never throws: any problem is reported in `error`.
    ParseResult parseMap(std::string_view text);

    // FNV-1a of the text: identifies the exact content of a map.
    std::uint64_t contentHash(std::string_view text);
}
