#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>
#include "engine/window/Event.hpp"
#include "mapjson/Folder.hpp"

namespace client {
    // The tabs of the lobby: the maps of the game, the maps written by the
    // players, and the best times of one of those maps.
    enum class Tab { Maps, Custom, Records };

    // The part of the lobby that belongs to this player: the maps found in its
    // CustomMaps folder, the tab on screen and the cursor.
    class LobbyMenu {
    public:
        explicit LobbyMenu(std::filesystem::path folder);

        // Reads the folder again.
        void rescan();

        // Tab (Maps, Custom map, Leaderboard, Maps...), Right and Left (the
        // leaderboard of the chosen map) and R, from the keys pressed in this
        // frame.
        void navigate(const std::vector<Engine::Key> &pressed);
        void moveCursor(int delta);

        // Text of the chosen map, or the reason why it cannot be played.
        [[nodiscard]] std::string readSelected(std::string &text) const;

        // The hash of the map whose records must be asked to the server.
        std::optional<std::uint64_t> takeRecordsRequest();

        void setMessage(std::string message);

        [[nodiscard]] Tab tab() const;
        [[nodiscard]] std::size_t cursor() const;
        [[nodiscard]] const std::string &message() const;
        [[nodiscard]] const std::vector<rtype::mapjson::MapEntry> &
            entries() const;
        [[nodiscard]] const rtype::mapjson::MapEntry *selected() const;

    private:
        void switchTab();
        void openRecords();

        std::filesystem::path _folder;
        std::vector<rtype::mapjson::MapEntry> _entries;
        Tab _tab = Tab::Maps;
        std::size_t _cursor = 0;
        bool _recordsWanted = false;
        std::string _message;
    };
}
