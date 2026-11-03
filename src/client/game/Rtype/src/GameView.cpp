#include <algorithm>
#include <cmath>
#include <cstdint>
#include "logic/Config.hpp"
#include "client/GameView.hpp"

namespace client {
    GameView::GameView(Engine::Renderer &renderer, const SpriteBank &sprites, const float worldToScreen,
        const float screenWidth, const float screenHeight)
        : _renderer(renderer), _sprites(sprites), _worldToScreen(worldToScreen),
        _screenWidth(screenWidth), _screenHeight(screenHeight),
        _background(sprites, screenWidth, screenHeight, worldToScreen),
        _hud(renderer, sprites, screenWidth, screenHeight)
    {
    }

    void GameView::draw(Replica &replica, const float elapsed)
    {
        _background.draw(_renderer, sceneOf(replica), elapsed);
        drawWorld(replica.registry(), elapsed);
        _hud.draw(replica, elapsed);
    }

    bool GameView::isBlock(const rtype::SpriteId id)
    {
        return id == rtype::SpriteId::RockWall
            || id == rtype::SpriteId::SpikeWall;
    }

    bool GameView::isPortalSprite(const rtype::SpriteId id)
    {
        return id == rtype::SpriteId::ReversePortal
            || id == rtype::SpriteId::FastPortal
            || id == rtype::SpriteId::NormalPortal;
    }

    Aura GameView::auraOf(const rtype::SpriteId id)
    {
        switch (id) {
            case rtype::SpriteId::Boss: return Aura::Standard;
            case rtype::SpriteId::DasherBoss: return Aura::Dasher;
            case rtype::SpriteId::SummonerBoss: return Aura::Summoner;
            case rtype::SpriteId::FortressBoss: return Aura::Fortress;
            default: return Aura::None;
        }
    }

    Scene GameView::sceneOf(Replica &replica)
    {
        Scene scene;
        const auto &status = replica.status();

        if (status && status->phase != rtype::Phase::Lobby) {
            scene.zone = rtype::zoneOf(status->map, status->level);
        }
        replica.registry().each<rtype::Renderable>(
            [&scene](const Engine::Entity, const rtype::Renderable &sprite) {
                if (auraOf(sprite.id) != Aura::None) {
                    scene.aura = auraOf(sprite.id);
                }
            });
        scene.boosted = replica.prediction().speedFactor() > 1.f;
        return scene;
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
        if (id == rtype::SpriteId::Meteor) {
            drawMeteor(reg, e, pos, box);
            return;
        }
        if (id == rtype::SpriteId::LaserPost || id == rtype::SpriteId::BeamOn
            || id == rtype::SpriteId::BeamWarn) {
            drawLaser(id, pos, box, elapsed);
            return;
        }
        const Engine::Sprite &sprite = _sprites.get(id);

        if (isBlock(id)) {
            drawTiled(sprite, sprite.animation.firstFrame,
                {rtype::config::OBSTACLE_TILE, rtype::config::OBSTACLE_TILE},
                {pos, box});
            return;
        }
        if (isPortalSprite(id)) {
            drawTiled(sprite, sprite.animation.frameAt(elapsed),
                {rtype::config::PORTAL_WIDTH, rtype::config::OBSTACLE_TILE},
                {pos, box});
            return;
        }
        _renderer.drawResized(sprite.texture,
            {pos.x * _worldToScreen, pos.y * _worldToScreen},
            frameFor(reg, e, sprite.animation, elapsed),
            {box.x * _worldToScreen, box.y * _worldToScreen});
    }

    void GameView::drawLaser(const rtype::SpriteId id,
        const gameLogic::Vec2Pos &pos, const gameLogic::Vec2Hitbox &box,
        const float elapsed)
    {
        const float k = _worldToScreen;
        const Engine::Vector2f at{pos.x * k, pos.y * k};
        const Engine::Vector2f size{box.x * k, box.y * k};

        if (id == rtype::SpriteId::LaserPost) {
            drawLaserPost(at, size);
        } else if (id == rtype::SpriteId::BeamWarn) {
            const bool blink = static_cast<int>(elapsed * 16.f) % 2 == 0;

            if (blink) {
                _renderer.drawRect(at, size, {255, 70, 70, 70});
            }
        } else {
            _renderer.drawRect(at, size, {255, 40, 90, 140});
            _renderer.drawRect({at.x + size.x * 0.25f, at.y},
                {size.x * 0.5f, size.y}, {255, 255, 255, 235});
        }
    }

    void GameView::drawMeteor(const Engine::Registry &reg,
        const Engine::Entity e, const gameLogic::Vec2Pos &pos,
        const gameLogic::Vec2Hitbox &box)
    {
        constexpr int TRAIL = 9;
        const gameLogic::Vec2Velocity velocity =
            reg.get<gameLogic::Vec2Velocity>(e);
        const float speed = std::hypot(velocity.x, velocity.y);
        const float k = _worldToScreen;
        gameLogic::Vec2 back{0.8f, -0.6f};

        if (speed > 1.f) {
            back = {-velocity.x / speed, -velocity.y / speed};
        }
        for (int i = TRAIL; i >= 1; --i) {
            const float t = static_cast<float>(i) / TRAIL;
            const float size = box.x * (1.f - 0.6f * t) * k;
            const float reach = t * box.x * 5.f;
            const auto alpha = static_cast<std::uint8_t>(190.f * (1.f - t));
            const float cx = (pos.x + box.x / 2.f + back.x * reach) * k;
            const float cy = (pos.y + box.y / 2.f + back.y * reach) * k;

            _renderer.drawRect({cx - size / 2.f, cy - size / 2.f},
                {size, size}, {255, 50, 30, alpha});
        }
        _renderer.drawResized(_sprites.get(rtype::SpriteId::Meteor).texture,
            {pos.x * k, pos.y * k}, {0, 0, 32, 32}, {box.x * k, box.y * k});
    }

    void GameView::drawLaserPost(const Engine::Vector2f at,
        const Engine::Vector2f size)
    {
        const float cap = 48.f * _worldToScreen;

        _renderer.drawRect({at.x + size.x * 0.45f, at.y},
            {size.x * 0.1f, size.y}, {255, 90, 90, 45});
        _renderer.drawRect(at, {size.x, cap}, {70, 76, 92, 255});
        _renderer.drawRect({at.x, at.y + size.y - cap}, {size.x, cap},
            {70, 76, 92, 255});
        _renderer.drawRect({at.x + size.x * 0.3f, at.y + cap * 0.6f},
            {size.x * 0.4f, cap * 0.4f}, {255, 90, 70, 255});
        _renderer.drawRect({at.x + size.x * 0.3f, at.y + size.y - cap},
            {size.x * 0.4f, cap * 0.4f}, {255, 90, 70, 255});
    }

    void GameView::drawTiled(const Engine::Sprite &sprite,
        const Engine::IntRect &frame, const Engine::Vector2f tile,
        const Area &area)
    {
        for (float y = 0.f; y < area.box.y; y += tile.y) {
            for (float x = 0.f; x < area.box.x; x += tile.x) {
                drawTile(sprite, frame, tile,
                    {{(area.pos.x + x) * _worldToScreen,
                        (area.pos.y + y) * _worldToScreen},
                    {std::min(tile.x, area.box.x - x),
                        std::min(tile.y, area.box.y - y)}});
            }
        }
    }

    void GameView::drawTile(const Engine::Sprite &sprite,
        const Engine::IntRect &frame, const Engine::Vector2f tile,
        const ScreenRect &target)
    {
        const float width = target.size.x * _worldToScreen;
        const float height = target.size.y * _worldToScreen;

        if (target.position.x > _screenWidth || target.position.x + width < 0.f
            || target.position.y > _screenHeight
            || target.position.y + height < 0.f) {
            return;
        }
        const Engine::IntRect source{frame.left, frame.top,
            static_cast<int>(static_cast<float>(frame.width)
                * target.size.x / tile.x),
            static_cast<int>(static_cast<float>(frame.height)
                * target.size.y / tile.y)};

        _renderer.drawResized(sprite.texture, target.position, source,
            {width, height});
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
