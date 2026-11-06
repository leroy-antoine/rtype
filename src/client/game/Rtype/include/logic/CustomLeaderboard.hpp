#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include "Config.hpp"
#include "Leaderboard.hpp"

namespace rtype {
    // Meilleurs temps des maps de joueur, par difficulte. Une map est
    // identifiee par le hash de son contenu : si on la modifie, son classement
    // repart de zero.
    class CustomLeaderboard {
    public:
        // Pour qu'un hote ne remplisse pas le fichier de maps sans fin.
        static constexpr std::size_t MAX_MAPS = 500;

        bool add(const std::uint64_t hash, const RunRecord &record)
        {
            if (!_records.contains(hash) && _records.size() >= MAX_MAPS) {
                return false;
            }
            addRecord(_records[hash][static_cast<std::size_t>(
                record.settings.difficulty)], record);
            return true;
        }

        [[nodiscard]] const DifficultyRecords &records(
            const std::uint64_t hash) const
        {
            static const DifficultyRecords NONE{};
            const auto found = _records.find(hash);

            if (found == _records.end()) {
                return NONE;
            }
            return found->second;
        }

        void load(const std::filesystem::path &file)
        {
            std::ifstream in(file);
            std::string line;

            while (std::getline(in, line)) {
                if (const auto entry = parse(line)) {
                    add(entry->first, entry->second);
                }
            }
        }

        bool save(const std::filesystem::path &file) const
        {
            std::ofstream out(file, std::ios::trunc);

            for (const auto &[hash, byDifficulty] : _records) {
                for (const MapRecords &runs : byDifficulty) {
                    for (const RunRecord &record : runs) {
                        write(out, hash, record);
                    }
                }
            }
            return static_cast<bool>(out);
        }

    private:
        static void write(std::ostream &out, const std::uint64_t hash,
            const RunRecord &record)
        {
            out << hash << ' ' << record.timeMs << ' '
                << static_cast<unsigned int>(record.players) << ' '
                << static_cast<unsigned int>(record.settings.difficulty)
                << ' ' << record.settings.friendlyFire << '\n';
        }

        // "hash temps joueurs difficulte tir-allie". Une ligne fausse est
        // ignoree.
        static std::optional<std::pair<std::uint64_t, RunRecord>> parse(
            const std::string &line)
        {
            std::istringstream in(line);
            std::uint64_t hash = 0;
            unsigned int time = 0;
            unsigned int players = 0;
            unsigned int difficulty = 0;
            bool fire = false;

            if (!(in >> hash >> time >> players >> difficulty >> fire)
                || players < 1 || players > config::MAX_PLAYERS
                || !isValidDifficulty(static_cast<std::uint8_t>(difficulty))) {
                return std::nullopt;
            }
            return std::make_pair(hash, RunRecord{time,
                static_cast<std::uint8_t>(players),
                Settings{static_cast<Difficulty>(difficulty), fire}});
        }

        std::map<std::uint64_t, DifficultyRecords> _records;
    };
}
