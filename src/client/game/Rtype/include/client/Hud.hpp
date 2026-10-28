#pragma once

#include <optional>
#include "engine/window/Renderer.hpp"
#include "Font.hpp"
#include "Replica.hpp"
#include "SpriteBank.hpp"
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
        static constexpr float COIN_MARGIN = 16.f;
        static constexpr float COIN_ICON_SIZE = 32.f;
        static constexpr float COIN_ICON_GAP = 10.f;
        static constexpr float COIN_TEXT_SCALE = 4.f;

        void drawPlayers(const rtype::GameStatus &status);
        void drawCoins(const rtype::GameStatus &status, float elapsed);
        void drawBar(Engine::Vector2f position, Engine::Vector2f size,
            float ratio, Engine::Color fill);
        void drawBossBar(const rtype::GameStatus &status);
        void drawRespawnHint(const rtype::GameStatus &status,
            std::optional<int> self, float elapsed);
        void drawEndScreen(const rtype::GameStatus &status, float elapsed);
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
