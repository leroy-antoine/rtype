#pragma once

#include <algorithm>
#include <cstddef>
#include "logic/MapList.hpp"
#include "logic/Settings.hpp"

namespace client {
    // Map lists the maps of the game, Custom the maps written by the players.
    // The settings and the scores screens serve both: the scores have one page
    // for each map of the game, then one page for the custom maps.
    enum class LobbyStep { Map, Settings, Scores, Custom };

    enum class SettingsRow { Difficulty, FriendlyFire };

    constexpr std::size_t SETTINGS_ROW_COUNT = 2;

    // La liste du lobby : les maps, puis l'entree du leaderboard.
    constexpr std::size_t LEADERBOARD_ENTRY = rtype::MAP_COUNT;
    constexpr std::size_t ENTRY_COUNT = rtype::MAP_COUNT + 1;

    // Ce que l'hote voit dans le lobby : d'abord la map, puis les reglages.
    class LobbyMenu {
    public:
        [[nodiscard]] LobbyStep step() const
        {
            return _step;
        }

        [[nodiscard]] SettingsRow row() const
        {
            return _row;
        }

        [[nodiscard]] const rtype::Settings &settings() const
        {
            return _draft;
        }

        void openSettings(const rtype::Settings &current,
            const bool forCustom = false)
        {
            _step = LobbyStep::Settings;
            _row = SettingsRow::Difficulty;
            _draft = current;
            _forCustom = forCustom;
        }

        // True when the settings are those of a map written by a player.
        [[nodiscard]] bool settingsForCustom() const
        {
            return _forCustom;
        }

        void openCustom()
        {
            _step = LobbyStep::Custom;
        }

        // `fromCustom`: opened from the list of custom maps, so it shows the
        // custom page and the way back leads to that list.
        void openScores(const bool fromCustom = false)
        {
            _step = LobbyStep::Scores;
            _scroll = 0;
            _scoresFromCustom = fromCustom;
            if (fromCustom) {
                _scoresMap = rtype::CUSTOM_MAP;
            }
        }

        // True when the scores page shown is the one of the custom maps.
        [[nodiscard]] bool onCustomScores() const
        {
            return _scoresMap == rtype::CUSTOM_MAP;
        }

        // One screen back: to the list of custom maps from the settings of a
        // custom map or from the scores opened from that list, to the maps of
        // the game from anything else.
        void back()
        {
            const bool fromCustomList =
                (_step == LobbyStep::Settings && _forCustom)
                || (_step == LobbyStep::Scores && _scoresFromCustom);

            if (fromCustomList) {
                _step = LobbyStep::Custom;
                return;
            }
            _step = LobbyStep::Map;
        }

        // Back to the first screen, whatever the current one.
        void reset()
        {
            _step = LobbyStep::Map;
            _forCustom = false;
            _scoresFromCustom = false;
        }

        [[nodiscard]] rtype::Difficulty shownDifficulty() const
        {
            return rtype::DIFFICULTY_BY_RANK[_shownRank];
        }

        // Passe au classement de la difficulte suivante (+1) ou precedente.
        void moveShown(const int delta)
        {
            const int count = static_cast<int>(rtype::DIFFICULTY_COUNT);
            const int next = (static_cast<int>(_shownRank) + delta + count)
                % count;

            _shownRank = static_cast<std::size_t>(next);
        }

        [[nodiscard]] rtype::MapId scoresMap() const
        {
            return _scoresMap;
        }

        // One page per map of the game, then the page of the custom maps.
        void moveScoresMap(const int delta)
        {
            const int count = static_cast<int>(rtype::MAP_COUNT) + 1;
            const int next = (static_cast<int>(_scoresMap) + delta + count)
                % count;

            _scoresMap = static_cast<rtype::MapId>(next);
            _scroll = 0;
        }

        [[nodiscard]] std::size_t scroll() const
        {
            return _scroll;
        }

        // Fait defiler la liste des scores, sans sortir de ses `count` lignes.
        void scrollBy(const int delta, const std::size_t count)
        {
            const int last = std::max(0, static_cast<int>(count) - 1);
            const int next = static_cast<int>(_scroll) + delta;

            _scroll = static_cast<std::size_t>(std::clamp(next, 0, last));
        }

        void moveRow(const int delta)
        {
            const int last = static_cast<int>(SETTINGS_ROW_COUNT) - 1;
            const int next = static_cast<int>(_row) + delta;

            _row = static_cast<SettingsRow>(std::clamp(next, 0, last));
        }

        // Applique un appui sur gauche (-1) ou droite (+1) a la ligne
        // courante et rend les reglages a envoyer.
        const rtype::Settings &change(const int side)
        {
            if (_row == SettingsRow::Difficulty) {
                _draft.difficulty = rtype::shiftedDifficulty(
                    _draft.difficulty, side);
                return _draft;
            }
            _draft.friendlyFire = side > 0;
            return _draft;
        }

    private:
        LobbyStep _step = LobbyStep::Map;
        SettingsRow _row = SettingsRow::Difficulty;
        rtype::Settings _draft;
        std::size_t _scroll = 0;
        rtype::MapId _scoresMap = 0;
        std::size_t _shownRank = 0;
        bool _forCustom = false;
        bool _scoresFromCustom = false;
    };
}
