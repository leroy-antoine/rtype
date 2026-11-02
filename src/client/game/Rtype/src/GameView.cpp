#include <algorithm>
#include "logic/Config.hpp"
#include "client/GameView.hpp"

namespace client {
    GameView::GameView(Engine::Renderer &renderer, const SpriteBank &sprites, const float worldToScreen,
        const float screenWidth, const float screenHeight)
        : _renderer(renderer), _sprites(sprites), _worldToScreen(worldToScreen),
        _screenWidth(screenWidth),
        _background(screenWidth, screenHeight, worldToScreen),
        _hud(renderer, sprites, screenWidth, screenHeight)
    {
    }

    void GameView::draw(Replica &replica, const float elapsed)
    {
        _background.draw(_renderer, elapsed);
        drawWorld(replica.registry(), elapsed);
        _hud.draw(replica, elapsed);
    }

    void GameView::drawWorld(Engine::Registry &reg, const float elapsed)
    {
        drawLayer(reg, elapsed, false);
        drawLayer(reg, elapsed, true);
    }

    void GameView::drawLayer(Engine::Registry &reg, const float elapsed, const bool effects)
    {
        reg.each<gameLogic::Vec2Pos, gameLogic::Vec2Hitbox, rtype::Renderable>(
            [&](const Engine::Entity e, const gameLogic::Vec2Pos &pos,
                const gameLogic::Vec2Hitbox &box, const rtype::Renderable &renderable) {
                if (reg.has<rtype::Lifetime>(e) == effects) {
                    drawEntity(reg, e, pos, box, renderable.id, elapsed);
                }
            });
    }

    void GameView::drawEntity(const Engine::Registry &reg, const Engine::Entity e,
        const gameLogic::Vec2Pos &pos, const gameLogic::Vec2Hitbox &box,
        const rtype::SpriteId id, const float elapsed)
    {
        const Engine::Sprite &sprite = _sprites.get(id);

        if (id == rtype::SpriteId::RockWall
            || id == rtype::SpriteId::HazardWall) {
            drawTiled(sprite, pos, box);
            return;
        }
        _renderer.drawResized(sprite.texture,
            {pos.x * _worldToScreen, pos.y * _worldToScreen},
            frameFor(reg, e, sprite.animation, elapsed),
            {box.x * _worldToScreen, box.y * _worldToScreen});
    }

    void GameView::drawTiled(const Engine::Sprite &sprite,
        const gameLogic::Vec2Pos &pos, const gameLogic::Vec2Hitbox &box)
    {
        const float tile = rtype::config::OBSTACLE_TILE;

        for (float y = 0.f; y < box.y; y += tile) {
            for (float x = 0.f; x < box.x; x += tile) {
                drawTile(sprite, {(pos.x + x) * _worldToScreen,
                    (pos.y + y) * _worldToScreen},
                    {std::min(tile, box.x - x), std::min(tile, box.y - y)});
            }
        }
    }

    void GameView::drawTile(const Engine::Sprite &sprite,
        const Engine::Vector2f position, const Engine::Vector2f size)
    {
        const float tile = rtype::config::OBSTACLE_TILE;
        const float width = size.x * _worldToScreen;

        if (position.x > _screenWidth || position.x + width < 0.f) {
            return;
        }
        const Engine::IntRect frame = sprite.animation.firstFrame;
        const Engine::IntRect source{frame.left, frame.top,
            static_cast<int>(static_cast<float>(frame.width) * size.x / tile),
            static_cast<int>(static_cast<float>(frame.height) * size.y / tile)};

        _renderer.drawResized(sprite.texture, position, source,
            {width, size.y * _worldToScreen});
    }

    int GameView::tiltFrame(const Engine::Registry &reg, const Engine::Entity e)
    {
        if (!reg.has<gameLogic::Vec2Velocity>(e)) {
            return PLAYER_FRAME_IDLE;
        }
        const float vy = reg.get<gameLogic::Vec2Velocity>(e).y;

        if (vy < 0.f) {
            return PLAYER_FRAME_UP;
        }
        return vy > 0.f ? PLAYER_FRAME_DOWN : PLAYER_FRAME_IDLE;
    }

    Engine::IntRect GameView::playerFrame(const Engine::Registry &reg, const Engine::Entity e,
        const Engine::Animation &animation)
    {
        Engine::IntRect rect = animation.frame(tiltFrame(reg, e));

        rect.top += reg.get<rtype::PlayerTag>(e).slot * rect.height;
        return rect;
    }

    Engine::IntRect GameView::effectFrame(const rtype::Lifetime &lifetime, const Engine::Animation &animation)
    {
        if (lifetime.duration <= 0.f) {
            return animation.firstFrame;
        }
        return animation.frameAtProgress(1.f - lifetime.remaining / lifetime.duration);
    }

    Engine::IntRect GameView::frameFor(const Engine::Registry &reg, const Engine::Entity e,
        const Engine::Animation &animation, const float elapsed)
    {
        if (reg.has<rtype::PlayerTag>(e)) {
            return playerFrame(reg, e, animation);
        }
        if (reg.has<rtype::Lifetime>(e)) {
            return effectFrame(reg.get<rtype::Lifetime>(e), animation);
        }
        return animation.frameAt(elapsed + static_cast<float>(e.getIndex()) * ANIMATION_PHASE);
    }
}
