#pragma once

#include "engine/core/Registry.hpp"
#include "engine/window/Renderer.hpp"
#include "logic/Components.hpp"
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

        struct Area {
            gameLogic::Vec2Pos pos;
            gameLogic::Vec2Hitbox box;
        };

        struct ScreenRect {
            Engine::Vector2f position;
            Engine::Vector2f size;
        };

        static bool isBlock(rtype::SpriteId id);
        static bool isPortalSprite(rtype::SpriteId id);
        static Aura auraOf(rtype::SpriteId id);
        static Scene sceneOf(Replica &replica);

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
        void drawMeteor(const Engine::Registry &reg, Engine::Entity e,
            const gameLogic::Vec2Pos &pos, const gameLogic::Vec2Hitbox &box);
        void drawLaserPost(Engine::Vector2f at, Engine::Vector2f size);
        void drawLaser(rtype::SpriteId id, const gameLogic::Vec2Pos &pos,
            const gameLogic::Vec2Hitbox &box, float elapsed);
        void drawTiled(const Engine::Sprite &sprite, const Engine::IntRect &frame,
            Engine::Vector2f tile, const Area &area);
        void drawTile(const Engine::Sprite &sprite, const Engine::IntRect &frame,
            Engine::Vector2f tile, const ScreenRect &target);

        Engine::Renderer &_renderer;
        const SpriteBank &_sprites;
        float _worldToScreen;
        float _screenWidth;
        float _screenHeight;
        Background _background;
        Hud _hud;
    };
}
