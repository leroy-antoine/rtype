#pragma once

#include <optional>
#include "engine/window/Renderer.hpp"
#include "Font.hpp"
#include "Replica.hpp"
#include "SpriteBank.hpp"
#include "logic/Leaderboard.hpp"
#include "logic/MapList.hpp"
#include "logic/Status.hpp"

namespace client {
    // Interface minimale dessinee par-dessus le jeu : une barre de vie par
    // joueur, la barre du boss et les messages de fin de partie. Les
    // coordonnees sont en pixels ecran.
    class Hud {
    public:
        Hud(Engine::Renderer &renderer, const SpriteBank &sprites,
            float screenWidth, float screenHeight);

        void draw(const Replica &replica, float elapsed);

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

        static float textTop();

        void drawPlayers(const rtype::GameStatus &status);
        void drawTimer(const rtype::GameStatus &status, float elapsed);
        void drawLevel(const rtype::GameStatus &status);
        void drawEffects(const Replica &replica);
        void drawIcon(const Engine::Sprite &sprite,
            Engine::Vector2f position, float elapsed);
        void drawCoins(const rtype::GameStatus &status, float elapsed);
        void drawBar(Engine::Vector2f position, Engine::Vector2f size,
            float ratio, Engine::Color fill);
        void drawBossBar(const rtype::GameStatus &status);
        void drawRespawnHint(const rtype::GameStatus &status,
            std::optional<int> self, float elapsed);
        void drawEndScreen(float elapsed);
        void drawLobby(const Replica &replica);
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
        void drawRecords(const rtype::MapRecords &records);
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
