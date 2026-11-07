#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>
#include "mapjson/Folder.hpp"

namespace client {
    // The maps written by the players that this client found in its
    // CustomMaps folder, and the cursor on them. The lobby (LobbyMenu) says
    // which screen is shown; this class only holds the list.
    class CustomMapList {
    public:
        explicit CustomMapList(std::filesystem::path folder);

        // Reads the folder again.
        void rescan();
        void moveCursor(int delta);

        // Text of the chosen map, or the reason why it cannot be played.
        [[nodiscard]] std::string readSelected(std::string &text) const;

        void setMessage(std::string message);

        [[nodiscard]] std::size_t cursor() const;
        [[nodiscard]] const std::string &message() const;
        [[nodiscard]] const std::vector<rtype::mapjson::MapEntry> &
            entries() const;
        // The map under the cursor, or null when there is none or when it has
        // a mistake.
        [[nodiscard]] const rtype::mapjson::MapEntry *selected() const;

    private:
        std::filesystem::path _folder;
        std::vector<rtype::mapjson::MapEntry> _entries;
        std::size_t _cursor = 0;
        std::string _message;
    };
}
