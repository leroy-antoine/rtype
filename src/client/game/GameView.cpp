#include "GameView.hpp"

namespace client {
    namespace {
        constexpr int PLAYER_FRAME_DOWN = 0;
        constexpr int PLAYER_FRAME_IDLE = 2;
        constexpr int PLAYER_FRAME_UP = 4;
        constexpr float ANIMATION_PHASE = 0.13f;
    }

    GameView::GameView(Engine::Renderer &renderer, const SpriteBank &sprites, const float worldToScreen)
        : _renderer(renderer), _sprites(sprites), _worldToScreen(worldToScreen)
    {
    }

    void GameView::draw(Engine::Registry &reg, const float elapsed)
    {
        for (const bool effects : {false, true}) {
            reg.each<gameLogic::Vec2Pos, gameLogic::Vec2Hitbox, rtype::Renderable>(
                [&](const Engine::Entity e, const gameLogic::Vec2Pos &pos,
                    const gameLogic::Vec2Hitbox &box, const rtype::Renderable &renderable) {
                    if (reg.has<rtype::Lifetime>(e) == effects) {
                        drawEntity(reg, e, pos, box, renderable.id, elapsed);
                    }
                });
        }
    }

    void GameView::drawEntity(Engine::Registry &reg, const Engine::Entity e,
        const gameLogic::Vec2Pos &pos, const gameLogic::Vec2Hitbox &box,
        const rtype::SpriteId id, const float elapsed)
    {
        const Engine::Sprite &sprite = _sprites.get(id);

        _renderer.drawResized(sprite.texture,
            {pos.x * _worldToScreen, pos.y * _worldToScreen},
            frameFor(reg, e, sprite.animation, elapsed),
            {box.x * _worldToScreen, box.y * _worldToScreen});
    }

    Engine::IntRect GameView::frameFor(Engine::Registry &reg, const Engine::Entity e,
        const Engine::Animation &animation, const float elapsed)
    {
        if (reg.has<rtype::PlayerTag>(e)) {
            int frame = PLAYER_FRAME_IDLE;

            if (reg.has<gameLogic::Vec2Velocity>(e)) {
                const float vy = reg.get<gameLogic::Vec2Velocity>(e).y;

                frame = vy < 0.f ? PLAYER_FRAME_UP : vy > 0.f ? PLAYER_FRAME_DOWN : PLAYER_FRAME_IDLE;
            }
            Engine::IntRect rect = animation.frame(frame);

            rect.top += reg.get<rtype::PlayerTag>(e).slot * rect.height;
            return rect;
        }
        if (reg.has<rtype::Lifetime>(e)) {
            const rtype::Lifetime &lifetime = reg.get<rtype::Lifetime>(e);

            return animation.frameAtProgress(1.f - lifetime.remaining / lifetime.duration);
        }
        return animation.frameAt(elapsed + static_cast<float>(e.getIndex()) * ANIMATION_PHASE);
    }
}
