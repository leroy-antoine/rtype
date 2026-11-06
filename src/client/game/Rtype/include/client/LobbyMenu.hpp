#pragma once

#include <algorithm>
#include <cstddef>
#include "logic/MapList.hpp"
#include "logic/Settings.hpp"

namespace client {
    enum class LobbyStep { Map, Settings, Scores };

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

        void openSettings(const rtype::Settings &current)
        {
            _step = LobbyStep::Settings;
            _row = SettingsRow::Difficulty;
            _draft = current;
        }

        void openScores()
        {
            _step = LobbyStep::Scores;
            _scroll = 0;
        }

        void back()
        {
            _step = LobbyStep::Map;
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

        void moveScoresMap(const int delta)
        {
            const int count = static_cast<int>(rtype::MAP_COUNT);
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
    };
}
