#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>
#include "mapjson/CustomMap.hpp"

namespace rtype::mapjson {
    // One file of the CustomMaps folder: a valid map, or the reason why not.
    struct MapEntry {
        std::string file;
        std::uint64_t hash = 0;
        std::optional<CustomMap> map;
        std::string error;
    };

    // Reads a whole map file into `text`. Returns an empty string on success,
    // the reason otherwise (file too big, unreadable). Never throws.
    std::string readMapFile(const std::filesystem::path &file,
        std::string &text);

    // Reads every .json file of the folder, sorted by file name. A missing
    // folder is created. Never throws.
    std::vector<MapEntry> scanMaps(const std::filesystem::path &folder);
}
