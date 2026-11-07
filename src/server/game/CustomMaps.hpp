#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <variant>
#include "logic/CustomLeaderboard.hpp"
#include "mapjson/CustomMap.hpp"
#include "rtype/CustomMapProtocol.hpp"
#include "rtype/MapUpload.hpp"

namespace server {
    // A map that was received and checked, ready to be played.
    struct LoadedMap {
        rtype::mapjson::CustomMap map;
        std::uint64_t hash = 0;
    };

    // The maps written by the players: receives their JSON, checks it again
    // (a client is never trusted) and keeps the records of each map.
    class CustomMaps {
    public:
        CustomMaps(rtype::CustomLeaderboard &board, std::filesystem::path recordsFile);

        bool begin(int slot, const rtype::protocol::Payload &payload);
        bool add(int slot, const rtype::protocol::Payload &chunk);
        void cancel(int slot);

        // The map, or the reason why the received text is not a valid map.
        std::variant<LoadedMap, std::string> finish(int slot);

        // The map that is being played: what the clients must be told.
        void select(const LoadedMap &loaded);
        [[nodiscard]] rtype::protocol::CustomInfo info() const;
        [[nodiscard]] bool selected() const;

        // Records the time of the map that is being played.
        std::uint64_t recordVictory(const rtype::RunRecord &record);
        [[nodiscard]] rtype::protocol::CustomRecords recordsOf(
            std::uint64_t hash) const;

    private:
        std::filesystem::path _recordsFile;
        rtype::MapUpload _upload;
        rtype::CustomLeaderboard &_board;
        std::optional<LoadedMap> _current;
    };
}
