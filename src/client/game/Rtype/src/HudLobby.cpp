#include <algorithm>
#include <array>
#include <string>
#include "client/DifficultyStyle.hpp"
#include "client/Hud.hpp"
#include "client/TimeFormat.hpp"
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
        const std::array<const char *, 2> names = {"MAPS", "CUSTOM MAP"};

        for (std::size_t i = 0; i < names.size(); ++i) {
            Color color = GREY;

            if (i == active) {
                color = YELLOW;
            }
            _font.drawCentered(names[i],
                _width / 2.f + (static_cast<float>(i) - 0.5f) * GAP, TOP, 3.f,
                color);
        }
    }

    void Hud::drawCustomStep(const Replica &replica,
        const CustomMapList &custom)
    {
        _font.drawCentered("SELECT A GAME", _width / 2.f, 50.f, 6.f, WHITE);
        drawTabs(TAB_CUSTOM);
        drawCustomList(custom);
        drawLines(utils::wrap(custom.message(), ERROR_COLUMNS), 600.f, 2.f,
            RED);
        drawCustomFooter(replica.isHost());
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

    // The page of the custom maps in the leaderboard: the three best times of
    // the map under the cursor, for each difficulty (the hardest first).
    void Hud::drawCustomScores(const Replica &replica,
        const CustomMapList &custom)
    {
        constexpr float TOP = 200.f;
        constexpr float BLOCK = 92.f;
        const rtype::mapjson::MapEntry *entry = custom.selected();

        _font.drawCentered("CUSTOM MAPS", _width / 2.f, 120.f, 4.f, YELLOW);
        if (entry == nullptr) {
            drawNoCustomScores(custom);
        } else {
            _font.drawCentered(entry->map->name, _width / 2.f, 160.f, 3.f,
                WHITE);
        }
        for (std::size_t i = 0; entry != nullptr
            && i < rtype::DIFFICULTY_BY_RANK.size(); ++i) {
            const rtype::Difficulty level = rtype::DIFFICULTY_BY_RANK[i];

            drawCustomDifficulty(level, replica.customRecords(entry->hash)[
                static_cast<std::size_t>(level)],
                TOP + BLOCK * static_cast<float>(i));
        }
        _font.drawCentered("LEFT/RIGHT: GAME  UP/DOWN: OTHER MAP  "
            "ENTER: BACK", _width / 2.f, _height - 50.f, 2.f, WHITE);
    }

    void Hud::drawNoCustomScores(const CustomMapList &custom)
    {
        if (custom.entries().empty()) {
            drawNoCustomMap();
            return;
        }
        _font.drawCentered("NO LEADERBOARD FOR A MAP WITH A MISTAKE",
            _width / 2.f, 230.f, 3.f, RED);
    }

    void Hud::drawCustomDifficulty(const rtype::Difficulty difficulty,
        const rtype::MapRecords &runs, const float top)
    {
        constexpr float ROW = 22.f;

        _font.drawCentered(difficultyName(difficulty), _width / 2.f, top,
            3.f, difficultyColor(difficulty));
        if (runs.empty()) {
            _font.drawCentered("NO RECORD YET", _width / 2.f, top + ROW + 4.f,
                2.f, GREY);
        }
        for (std::size_t i = 0; i < runs.size(); ++i) {
            const std::string line = std::to_string(i + 1) + ". "
                + formatTime(runs[i].timeMs) + "  "
                + std::to_string(runs[i].players) + "P";

            _font.drawCentered(line, _width / 2.f,
                top + ROW + 4.f + ROW * static_cast<float>(i), 2.f, GREEN);
        }
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

    void Hud::drawCustomFooter(const bool host)
    {
        std::string footer = "WAITING FOR P1 TO CHOOSE   TAB: MAPS";

        if (host) {
            footer = "UP/DOWN: CHOOSE  ENTER: SETTINGS  RIGHT: LEADERBOARD  "
                "R: REFRESH  TAB: MAPS";
        }
        _font.drawCentered(footer, _width / 2.f, _height - 50.f, 2.f, WHITE);
    }
}
