#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <map>
#include "Config.hpp"
#include "Leaderboard.hpp"

namespace rtype {
    // Meilleurs temps des maps de joueur. Une map est identifiee par le hash
    // de son contenu : si on la modifie, son classement repart de zero.
    class CustomLeaderboard {
    public:
        // Pour qu'un hote ne remplisse pas le fichier de maps sans fin.
        static constexpr std::size_t MAX_MAPS = 500;

        bool add(const std::uint64_t hash, const RunRecord &record)
        {
            if (!_records.contains(hash) && _records.size() >= MAX_MAPS) {
                return false;
            }
            addRecord(_records[hash], record);
            return true;
        }

        [[nodiscard]] const MapRecords &records(const std::uint64_t hash) const
        {
            static const MapRecords NONE;
            const auto found = _records.find(hash);

            if (found == _records.end()) {
                return NONE;
            }
            return found->second;
        }

        void load(const std::filesystem::path &file)
        {
            std::ifstream in(file);
            std::uint64_t hash = 0;
            unsigned int time = 0;
            unsigned int players = 0;

            while (in >> hash >> time >> players) {
                if (players >= 1 && players <= config::MAX_PLAYERS) {
                    add(hash, {time, static_cast<std::uint8_t>(players)});
                }
            }
        }

        bool save(const std::filesystem::path &file) const
        {
            std::ofstream out(file, std::ios::trunc);

            for (const auto &[hash, records] : _records) {
                for (const RunRecord &record : records) {
                    out << hash << ' ' << record.timeMs << ' '
                        << static_cast<unsigned int>(record.players) << '\n';
                }
            }
            return static_cast<bool>(out);
        }

    private:
        std::map<std::uint64_t, MapRecords> _records;
    };
}
