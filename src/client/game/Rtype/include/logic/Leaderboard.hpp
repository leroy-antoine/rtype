#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>
#include "MapList.hpp"
#include "Settings.hpp"

namespace rtype {
    struct RunRecord {
        std::uint32_t timeMs = 0;
        std::uint8_t players = 1;
        Settings settings;
    };

    struct RunEntry {
        MapId map = 0;
        RunRecord record;
    };

    constexpr std::size_t MAX_RECORDS = 3;

    using MapRecords = std::vector<RunRecord>;
    using DifficultyRecords = std::array<MapRecords, DIFFICULTY_COUNT>;
    using AllRecords = std::array<DifficultyRecords, MAP_COUNT>;
    using RunsByMap = std::array<MapRecords, MAP_COUNT>;

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

    // Toutes les parties gagnees sur une map, la plus rapide en premier.
    inline MapRecords runsOf(const std::vector<RunEntry> &history,
        const MapId map)
    {
        MapRecords runs;

        for (const RunEntry &entry : history) {
            if (entry.map == map) {
                runs.push_back(entry.record);
            }
        }
        std::ranges::stable_sort(runs, [](const auto &a, const auto &b) {
            return a.timeMs < b.timeMs;
        });
        return runs;
    }

    // Les trois meilleurs temps de chaque map et de chaque difficulte, et toutes les parties
    // gagnees depuis le debut.
    class Leaderboard {
    public:
        bool add(const MapId map, const RunRecord &record)
        {
            if (map >= MAP_COUNT) {
                return false;
            }
            _history.push_back({map, record});
            addRecord(_records[map][static_cast<std::size_t>(
                record.settings.difficulty)], record);
            return true;
        }

        [[nodiscard]] const AllRecords &all() const
        {
            return _records;
        }

        [[nodiscard]] const std::vector<RunEntry> &history() const
        {
            return _history;
        }

        void load(const std::filesystem::path &file)
        {
            std::ifstream in(file);
            std::string line;

            while (std::getline(in, line)) {
                if (const auto entry = parse(line)) {
                    add(entry->map, entry->record);
                }
            }
        }

        bool save(const std::filesystem::path &file) const
        {
            std::ofstream out(file, std::ios::trunc);

            for (const RunEntry &entry : _history) {
                out << static_cast<unsigned int>(entry.map) << ' '
                    << entry.record.timeMs << ' '
                    << static_cast<unsigned int>(entry.record.players) << ' '
                    << static_cast<unsigned int>(
                        entry.record.settings.difficulty) << ' '
                    << entry.record.settings.friendlyFire << '\n';
            }
            return static_cast<bool>(out);
        }

    private:
        // "map temps joueurs [difficulte tir-allie]" : les anciens fichiers
        // n'ont pas les deux derniers champs.
        static std::optional<RunEntry> parse(const std::string &line)
        {
            std::istringstream in(line);
            unsigned int map = 0;
            unsigned int time = 0;
            unsigned int players = 0;
            unsigned int difficulty = 1;
            bool fire = false;

            if (!(in >> map >> time >> players)) {
                return std::nullopt;
            }
            if (!(in >> difficulty >> fire)
                || !isValidDifficulty(static_cast<std::uint8_t>(difficulty))) {
                difficulty = 1;
                fire = false;
            }
            return RunEntry{static_cast<MapId>(map),
                {time, static_cast<std::uint8_t>(players),
                    Settings{static_cast<Difficulty>(difficulty), fire}}};
        }

        AllRecords _records{};
        std::vector<RunEntry> _history;
    };
}
