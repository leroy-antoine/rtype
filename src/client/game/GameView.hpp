#pragma once

#include "engine/core/Registry.hpp"
#include "engine/window/Renderer.hpp"
#include "Components.hpp"
#include "Background.hpp"
#include "Hud.hpp"
#include "Replica.hpp"
#include "SpriteBank.hpp"
#include "utils/Vectors.hpp"

namespace client {
    class GameView {
    public:
        GameView(Engine::Renderer &renderer, const SpriteBank &sprites, float worldToScreen,
            float screenWidth, float screenHeight);

        // Fond, entites du monde puis interface.
        void draw(Replica &replica, float elapsed);

    private:
        static constexpr int PLAYER_FRAME_DOWN = 0;
        static constexpr int PLAYER_FRAME_IDLE = 2;
        static constexpr int PLAYER_FRAME_UP = 4;
        static constexpr float ANIMATION_PHASE = 0.13f;

        static int tiltFrame(const Engine::Registry &reg, Engine::Entity e);
        static Engine::IntRect playerFrame(const Engine::Registry &reg, Engine::Entity e,
            const Engine::Animation &animation);
        static Engine::IntRect effectFrame(const rtype::Lifetime &lifetime, const Engine::Animation &animation);
        static Engine::IntRect frameFor(const Engine::Registry &reg, Engine::Entity e,
            const Engine::Animation &animation, float elapsed);

        void drawWorld(Engine::Registry &reg, float elapsed);
        void drawLayer(Engine::Registry &reg, float elapsed, bool effects);
        void drawEntity(const Engine::Registry &reg, Engine::Entity e, const gameLogic::Vec2Pos &pos,
            const gameLogic::Vec2Hitbox &box, rtype::SpriteId id, float elapsed);

        Engine::Renderer &_renderer;
        const SpriteBank &_sprites;
        float _worldToScreen;
        Background _background;
        Hud _hud;
    };
}
