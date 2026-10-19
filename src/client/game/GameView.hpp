#pragma once

#include "engine/core/Registry.hpp"
#include "engine/window/Renderer.hpp"
#include "Components.hpp"
#include "SpriteBank.hpp"
#include "utils/Vectors.hpp"

namespace client {
    class GameView {
    public:
        GameView(Engine::Renderer &renderer, const SpriteBank &sprites, float worldToScreen);

        void draw(Engine::Registry &reg, float elapsed);

    private:
        void drawEntity(Engine::Registry &reg, Engine::Entity e, const gameLogic::Vec2Pos &pos,
            const gameLogic::Vec2Hitbox &box, rtype::SpriteId id, float elapsed);
        [[nodiscard]] static Engine::IntRect frameFor(Engine::Registry &reg, Engine::Entity e,
            const Engine::Animation &animation, float elapsed);

        Engine::Renderer &_renderer;
        const SpriteBank &_sprites;
        float _worldToScreen;
    };
}
