#pragma once

#include <optional>
#include <vector>
#include "engine/window/Renderer.hpp"
#include "CustomMapList.hpp"
#include "Font.hpp"
#include "Replica.hpp"
#include "SpriteBank.hpp"
#include "logic/Leaderboard.hpp"
#include "logic/MapList.hpp"
#include "logic/Augments.hpp"
#include "logic/Settings.hpp"
#include "logic/Status.hpp"
#include <string>

namespace client {
    // Interface minimale dessinee par-dessus le jeu : une barre de vie par
    // joueur, la barre du boss et les messages de fin de partie. Les
    // coordonnees sont en pixels ecran.
    class Hud {
    public:
        Hud(Engine::Renderer &renderer, const SpriteBank &sprites,
            float screenWidth, float screenHeight);

        void draw(const Replica &replica, const CustomMapList &custom,
            float elapsed);

    private:
        static constexpr float BAR_X = 70.f;
        static constexpr float BAR_WIDTH = 160.f;
        static constexpr float BAR_HEIGHT = 14.f;
        static constexpr float ROW_STEP = 30.f;
        static constexpr float BOSS_BAR_WIDTH = 600.f;
        static constexpr float PLAYERS_TOP = 60.f;
        static constexpr float MARGIN = 16.f;
        static constexpr float ICON_SIZE = 32.f;
        static constexpr float ICON_GAP = 10.f;
        static constexpr float TEXT_SCALE = 4.f;
        static constexpr float RUN_ROW_WIDTH = 540.f;

        static float textTop();

        void drawPlayers(const rtype::GameStatus &status,
            const rtype::protocol::OwnedAugment &augments);
        void drawAugments(const std::vector<rtype::AugmentId> &ids, float x,
            float top);
        void drawTimer(const rtype::GameStatus &status, float elapsed);
        void drawLevel(const rtype::GameStatus &status);
        void drawEffects(const Replica &replica);
        void drawIcon(const Engine::Sprite &sprite,
            Engine::Vector2f position, float elapsed);
        static constexpr float AUGMENT_GAP = 12.f;
        static constexpr float AUGMENT_TEXT_SCALE = 1.5f;
        static constexpr float AUGMENT_PADDING = 3.f;
        static constexpr float SHOP_TEXT_MARGIN = 60.f;
        void drawCoins(const rtype::GameStatus &status, float elapsed);
        void drawBar(Engine::Vector2f position, Engine::Vector2f size,
            float ratio, Engine::Color fill);
        void drawBossBar(const rtype::GameStatus &status);
        void drawRespawnHint(const rtype::GameStatus &status,
            std::optional<int> self, float elapsed);
        void drawEndScreen(float elapsed);
        void drawLobby(const Replica &replica, const CustomMapList &custom);
        // The maps written by the players (see HudLobby.cpp).
        void drawTabs(std::size_t active);
        void drawCustomStep(const Replica &replica,
            const CustomMapList &custom);
        void drawCustomList(const CustomMapList &custom);
        void drawCustomRow(const rtype::mapjson::MapEntry &entry, float y,
            bool selected);
        void drawCustomDetail(const rtype::mapjson::MapEntry &entry);
        void drawNoCustomMap();
        void drawCustomScores(const Replica &replica,
            const CustomMapList &custom);
        void drawNoCustomScores(const CustomMapList &custom);
        void drawCustomDifficulty(rtype::Difficulty difficulty,
            const rtype::MapRecords &runs, float top);
        void drawLines(const std::vector<std::string> &lines, float top,
            float scale, Engine::Color color);
        void drawCustomFooter(bool host);
        [[nodiscard]] static std::string settingsMapName(
            const Replica &replica, const CustomMapList &custom);
        void drawMapStep(const Replica &replica);
        void drawSettingsStep(const Replica &replica,
            const std::string &mapName);
        void drawSettingRow(const std::string &label,
            const std::string &value, Engine::Color color, bool selected,
            float y);
        void drawScaleSummary(rtype::Difficulty difficulty);
        void drawScoresStep(const Replica &replica,
            const CustomMapList &custom);
        void drawRunRow(std::size_t rank, const rtype::RunRecord &record,
            float y);
        float drawLives(int lives, float x, float top);
        void drawVictoryScreen(const rtype::GameStatus &status,
            const Replica &replica, float elapsed);
        void drawConfetti(float age);
        void drawVictoryTitle(float age);
        void drawVictoryStats(const rtype::GameStatus &status,
            const Replica &replica, float age);
        [[nodiscard]] Engine::Color timeColor(const rtype::GameStatus &status,
            const Replica &replica) const;
        void drawMenuHint(const Replica &replica);
        void drawPauseMenu(const Replica &replica);
        void drawMapList(rtype::MapId cursor);
        void drawRecords(const rtype::MapRecords &records,
            rtype::Difficulty difficulty);
        float drawReady(const rtype::PlayerStatus &player, float x, float top);
        void drawShopScreen(const rtype::GameStatus &status,
            const Replica &replica, float elapsed);
        void trackPhase(rtype::Phase phase, float elapsed);

        Engine::Renderer &_renderer;
        const SpriteBank &_sprites;
        Font _font;
        float _width;
        float _height;
        rtype::Phase _phase = rtype::Phase::Playing;
        float _phaseSince = 0.f;
    };
}
