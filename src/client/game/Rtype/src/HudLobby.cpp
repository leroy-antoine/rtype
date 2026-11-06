#include <algorithm>
#include <array>
#include <string>
#include "client/Hud.hpp"
#include "client/TimeFormat.hpp"
#include "utils/Wrap.hpp"

namespace client {
    namespace {
        using Engine::Color;

        constexpr Color WHITE{255, 255, 255, 255};
        constexpr Color GREEN{90, 220, 90, 255};
        constexpr Color RED{235, 70, 60, 255};
        constexpr Color YELLOW{255, 220, 40, 255};
        constexpr Color GREY{150, 150, 150, 255};
        constexpr Color SELECTED_STRIP{40, 70, 140, 255};

        constexpr std::size_t VISIBLE_MAPS = 6;
        constexpr std::size_t ERROR_COLUMNS = 90;
        constexpr float CUSTOM_ROW_TOP = 170.f;
        constexpr float CUSTOM_ROW_STEP = 44.f;
        constexpr float STRIP_WIDTH = 760.f;
        constexpr float DETAIL_TOP = 460.f;

        // First map shown, so that the cursor is always on screen.
        std::size_t firstVisible(const std::size_t cursor)
        {
            if (cursor + 1 > VISIBLE_MAPS) {
                return cursor + 1 - VISIBLE_MAPS;
            }
            return 0;
        }

        std::string labelOf(const rtype::mapjson::MapEntry &entry)
        {
            if (entry.map) {
                return entry.map->name;
            }
            return "! " + entry.file;
        }
    }

    void Hud::drawLobby(const Replica &replica, const LobbyMenu &menu)
    {
        _renderer.drawRect({0.f, 0.f}, {_width, _height}, {0, 0, 0, 210});
        _font.drawCentered("SELECT A MAP", _width / 2.f, 40.f, 6.f, WHITE);
        drawTabs(menu.tab());
        if (menu.tab() == Tab::Maps) {
            drawMapList(replica.mapCursor());
            drawRecords(replica.records()[replica.mapCursor()]);
        } else if (menu.tab() == Tab::Custom) {
            drawCustomList(menu);
        } else {
            drawCustomRecords(replica, menu);
        }
        drawLines(utils::wrap(menu.message(), ERROR_COLUMNS), 600.f, 2.f,
            RED);
        drawLobbyFooter(menu, replica.isHost());
    }

    void Hud::drawTabs(const Tab tab)
    {
        constexpr float TOP = 104.f;
        constexpr float GAP = 330.f;
        const std::array<const char *, 3> names = {"MAPS", "CUSTOM MAP",
            "LEADERBOARD"};

        for (std::size_t i = 0; i < names.size(); ++i) {
            Color color = GREY;

            if (i == static_cast<std::size_t>(tab)) {
                color = YELLOW;
            }
            _font.drawCentered(names[i],
                _width / 2.f + (static_cast<float>(i) - 1.f) * GAP, TOP, 3.f,
                color);
        }
    }

    void Hud::drawMapList(const rtype::MapId cursor)
    {
        constexpr float TOP = 150.f;
        constexpr float STEP = 52.f;
        constexpr float STRIP = 520.f;

        for (std::size_t i = 0; i < rtype::MAP_COUNT; ++i) {
            const float y = TOP + static_cast<float>(i) * STEP;
            Color color = WHITE;

            if (i == cursor) {
                _renderer.drawRect({(_width - STRIP) / 2.f, y - 8.f},
                    {STRIP, STEP - 6.f}, SELECTED_STRIP);
                color = YELLOW;
            }
            _font.drawCentered(rtype::MAP_NAMES[i], _width / 2.f, y, 4.f,
                color);
        }
    }

    void Hud::drawRecords(const rtype::MapRecords &records)
    {
        constexpr float TOP = 360.f;
        constexpr float STEP = 36.f;

        _font.drawCentered("BEST TIMES", _width / 2.f, TOP, 3.f, WHITE);
        if (records.empty()) {
            _font.drawCentered("NO RECORD YET", _width / 2.f, TOP + STEP,
                3.f, GREEN);
        }
        for (std::size_t i = 0; i < records.size(); ++i) {
            const std::string line = std::to_string(i + 1) + ". "
                + formatTime(records[i].timeMs) + "  "
                + std::to_string(records[i].players) + "P";

            _font.drawCentered(line, _width / 2.f,
                TOP + STEP * static_cast<float>(i + 1), 3.f, GREEN);
        }
    }

    void Hud::drawCustomList(const LobbyMenu &menu)
    {
        const auto &entries = menu.entries();
        const std::size_t first = firstVisible(menu.cursor());
        const std::size_t last = std::min(entries.size(),
            first + VISIBLE_MAPS);

        if (entries.empty()) {
            drawNoCustomMap();
            return;
        }
        for (std::size_t i = first; i < last; ++i) {
            const auto row = static_cast<float>(i - first);

            drawCustomRow(entries[i], CUSTOM_ROW_TOP + row * CUSTOM_ROW_STEP,
                i == menu.cursor());
        }
        drawCustomDetail(entries[menu.cursor()]);
    }

    void Hud::drawCustomRow(const rtype::mapjson::MapEntry &entry,
        const float y, const bool selected)
    {
        Color color = WHITE;

        if (!entry.map) {
            color = RED;
        }
        if (selected) {
            _renderer.drawRect({(_width - STRIP_WIDTH) / 2.f, y - 8.f},
                {STRIP_WIDTH, CUSTOM_ROW_STEP - 6.f}, SELECTED_STRIP);
            if (entry.map) {
                color = YELLOW;
            }
        }
        _font.drawCentered(labelOf(entry), _width / 2.f, y, 3.f, color);
    }

    // Who wrote the map, or what is wrong with it.
    void Hud::drawCustomDetail(const rtype::mapjson::MapEntry &entry)
    {
        if (!entry.map) {
            drawLines(utils::wrap(entry.error, ERROR_COLUMNS), DETAIL_TOP,
                2.f, RED);
            return;
        }
        const std::string author = "BY " + entry.map->author;

        _font.drawCentered(author + "   "
            + std::to_string(entry.map->levels.size()) + " LEVELS",
            _width / 2.f, DETAIL_TOP, 2.f, GREEN);
    }

    void Hud::drawNoCustomMap()
    {
        _font.drawCentered("NO MAP FOUND", _width / 2.f, 230.f, 4.f, WHITE);
        _font.drawCentered("PUT .JSON FILES IN THE CUSTOMMAPS FOLDER",
            _width / 2.f, 300.f, 2.f, GREY);
        _font.drawCentered("THEN PRESS R", _width / 2.f, 330.f, 2.f, GREY);
    }

    void Hud::drawCustomRecords(const Replica &replica, const LobbyMenu &menu)
    {
        const rtype::mapjson::MapEntry *entry = menu.selected();

        if (entry == nullptr) {
            return;
        }
        _font.drawCentered(entry->map->name, _width / 2.f, 200.f, 4.f,
            YELLOW);
        drawRecords(replica.customRecords(entry->hash));
    }

    void Hud::drawLines(const std::vector<std::string> &lines,
        const float top, const float scale, const Color color)
    {
        const float step = Font::height(scale) * 1.7f;

        for (std::size_t i = 0; i < lines.size(); ++i) {
            _font.drawCentered(lines[i], _width / 2.f,
                top + step * static_cast<float>(i), scale, color);
        }
    }

    void Hud::drawLobbyFooter(const LobbyMenu &menu, const bool host)
    {
        std::string footer = "WAITING FOR P1 TO CHOOSE   TAB: SWITCH";

        if (menu.tab() == Tab::Records) {
            footer = "LEFT: BACK   UP/DOWN: OTHER MAP   TAB: MAPS";
        } else if (host && menu.tab() == Tab::Custom) {
            footer = "UP/DOWN: CHOOSE  ENTER: START  TAB/RIGHT: LEADERBOARD  "
                "R: REFRESH";
        } else if (host) {
            footer = "UP/DOWN: CHOOSE   ENTER: START   TAB: CUSTOM MAPS";
        }
        _font.drawCentered(footer, _width / 2.f, _height - 50.f, 2.f, WHITE);
    }
}
