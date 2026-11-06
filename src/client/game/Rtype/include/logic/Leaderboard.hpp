#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <vector>
#include "MapList.hpp"

namespace rtype {
    struct RunRecord {
        std::uint32_t timeMs = 0;
        std::uint8_t players = 1;
    };

    constexpr std::size_t MAX_RECORDS = 5;

    using MapRecords = std::vector<RunRecord>;
    using AllRecords = std::array<MapRecords, MAP_COUNT>;

    // Ajoute un temps : le plus rapide en premier, MAX_RECORDS au plus.
    inline void addRecord(MapRecords &records, const RunRecord &record)
    {
        records.push_back(record);
        std::ranges::stable_sort(records, [](const auto &a, const auto &b) {
            return a.timeMs < b.timeMs;
        });
        if (records.size() > MAX_RECORDS) {
            records.resize(MAX_RECORDS);
        }
    }

    // Meilleurs temps de victoire de chaque map, le plus rapide en premier.
    class Leaderboard {
    public:
        bool add(const MapId map, const RunRecord &record)
        {
            if (map >= MAP_COUNT) {
                return false;
            }
            addRecord(_records[map], record);
            return true;
        }

        [[nodiscard]] const AllRecords &all() const
        {
            return _records;
        }

        void load(const std::filesystem::path &file)
        {
            std::ifstream in(file);
            unsigned int map = 0;
            unsigned int time = 0;
            unsigned int players = 0;

            while (in >> map >> time >> players) {
                add(static_cast<MapId>(map), {time,
                    static_cast<std::uint8_t>(players)});
            }
        }

        bool save(const std::filesystem::path &file) const
        {
            std::ofstream out(file, std::ios::trunc);

            for (std::size_t map = 0; map < MAP_COUNT; ++map) {
                for (const RunRecord &record : _records[map]) {
                    out << map << ' ' << record.timeMs << ' '
                        << static_cast<unsigned int>(record.players) << '\n';
                }
            }
            return static_cast<bool>(out);
        }

    private:
        AllRecords _records{};
    };
}
