#include <algorithm>
#include <array>
#include <string>
#include "client/DifficultyStyle.hpp"
#include "client/Hud.hpp"
#include "utils/Wrap.hpp"

// The part of the lobby that shows the maps written by the players. The maps
// of the game, the settings and the history are drawn in Hud.cpp.
namespace client {
    namespace {
        using Engine::Color;

        constexpr Color WHITE{255, 255, 255, 255};
        constexpr Color GREEN{90, 220, 90, 255};
        constexpr Color RED{235, 70, 60, 255};
        constexpr Color YELLOW{255, 220, 40, 255};
        constexpr Color GREY{150, 150, 150, 255};
        constexpr Color SELECTED_STRIP{40, 70, 140, 255};

        constexpr std::size_t TAB_MAPS = 0;
        constexpr std::size_t TAB_CUSTOM = 1;
        constexpr std::size_t TAB_SCORES = 2;
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

    std::string Hud::settingsMapName(const Replica &replica,
        const CustomMapList &custom)
    {
        const rtype::mapjson::MapEntry *entry = custom.selected();

        if (replica.menu().settingsForCustom() && entry != nullptr) {
            return entry->map->name;
        }
        return rtype::MAP_NAMES[replica.mapCursor() % rtype::MAP_COUNT];
    }

    void Hud::drawTabs(const std::size_t active)
    {
        constexpr float TOP = 104.f;
        constexpr float GAP = 330.f;
        const std::array<const char *, 3> names = {"MAPS", "CUSTOM MAP",
            "LEADERBOARD"};

        for (std::size_t i = 0; i < names.size(); ++i) {
            Color color = GREY;

            if (i == active) {
                color = YELLOW;
            }
            _font.drawCentered(names[i],
                _width / 2.f + (static_cast<float>(i) - 1.f) * GAP, TOP, 3.f,
                color);
        }
    }

    void Hud::drawCustomStep(const Replica &replica,
        const CustomMapList &custom)
    {
        const LobbyStep step = replica.menu().step();

        _font.drawCentered("SELECT A GAME", _width / 2.f, 50.f, 6.f, WHITE);
        if (step == LobbyStep::CustomScores) {
            drawTabs(TAB_SCORES);
            drawCustomScores(replica, custom);
        } else {
            drawTabs(TAB_CUSTOM);
            drawCustomList(custom);
        }
        drawLines(utils::wrap(custom.message(), ERROR_COLUMNS), 600.f, 2.f,
            RED);
        drawCustomFooter(step, replica.isHost());
    }

    void Hud::drawCustomList(const CustomMapList &custom)
    {
        const auto &entries = custom.entries();
        const std::size_t first = firstVisible(custom.cursor());
        const std::size_t last = std::min(entries.size(),
            first + VISIBLE_MAPS);

        if (entries.empty()) {
            drawNoCustomMap();
            return;
        }
        for (std::size_t i = first; i < last; ++i) {
            const auto row = static_cast<float>(i - first);

            drawCustomRow(entries[i], CUSTOM_ROW_TOP + row * CUSTOM_ROW_STEP,
                i == custom.cursor());
        }
        drawCustomDetail(entries[custom.cursor()]);
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

    // The best times of the selected map, for the difficulty being shown.
    void Hud::drawCustomScores(const Replica &replica,
        const CustomMapList &custom)
    {
        const rtype::mapjson::MapEntry *entry = custom.selected();
        const rtype::Difficulty shown = replica.menu().shownDifficulty();

        if (entry == nullptr) {
            _font.drawCentered("NO LEADERBOARD FOR A MAP WITH A MISTAKE",
                _width / 2.f, 230.f, 3.f, RED);
            return;
        }
        _font.drawCentered(entry->map->name, _width / 2.f, 200.f, 4.f,
            YELLOW);
        drawRecords(replica.customRecords(entry->hash)[
            static_cast<std::size_t>(shown)], shown);
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

    void Hud::drawCustomFooter(const LobbyStep step, const bool host)
    {
        std::string footer = "WAITING FOR P1 TO CHOOSE   TAB: MAPS";

        if (step == LobbyStep::CustomScores) {
            footer = "LEFT/RIGHT: DIFFICULTY  UP/DOWN: OTHER MAP  TAB: MAPS";
        } else if (host) {
            footer = "UP/DOWN: CHOOSE  ENTER: SETTINGS  TAB/RIGHT: "
                "LEADERBOARD  R: REFRESH";
        }
        _font.drawCentered(footer, _width / 2.f, _height - 50.f, 2.f, WHITE);
    }
}
