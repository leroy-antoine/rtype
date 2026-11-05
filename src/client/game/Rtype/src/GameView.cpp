#include <algorithm>
#include <cmath>
#include <cstdint>
#include "logic/Config.hpp"
#include "client/GameView.hpp"
#include "logic/Charge.hpp"

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
        trackBossHurt(replica, elapsed);
        _background.draw(_renderer, sceneOf(replica), elapsed);
        drawWorld(replica.registry(), elapsed);
        drawCharge(replica);
        _hud.draw(replica, elapsed);
    }

    void GameView::trackBossHurt(const Replica &replica, const float elapsed)
    {
        const auto &status = replica.status();
        int hp = 0;

        if (status) {
            hp = status->bossHp;
        }
        if (hp > 0 && hp < _lastBossHp) {
            _hurtUntil = elapsed + HURT_DURATION;
        }
        _lastBossHp = hp;
    }

    Engine::Vector2f GameView::shakeOf(const rtype::SpriteId id,
        const float elapsed) const
    {
        if (id != rtype::SpriteId::FortressBoss || elapsed >= _hurtUntil) {
            return {0.f, 0.f};
        }
        const float left = (_hurtUntil - elapsed) / HURT_DURATION;

        return {std::sin(elapsed * 90.f) * HURT_SHAKE * left,
            std::cos(elapsed * 110.f) * HURT_SHAKE * left};
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

    void GameView::drawGauge(const Engine::Vector2f at, const float width,
        const float ratio, const Engine::Color fill)
    {
        constexpr float HEIGHT = 6.f;

        _renderer.drawRect(at, {width, HEIGHT}, {20, 20, 30, 220});
        _renderer.drawRect(at, {width * ratio, HEIGHT}, fill);
    }

    // Les jauges de charge, sous le vaisseau du joueur : le tir de base,
    // puis le rayon laser quand il est equipe.
    void GameView::drawCharge(Replica &replica)
    {
        constexpr float GAP = 8.f;
        constexpr float ROW = 10.f;
        const auto ship = replica.localShip();

        if (!ship) {
            return;
        }
        const float width = ship->second.x * _worldToScreen;
        const Engine::Vector2f at{ship->first.x * _worldToScreen,
            (ship->first.y + ship->second.y) * _worldToScreen + GAP};
        const float time = replica.chargeTime();

        if (rtype::isCharging(time)) {
            const float ratio = rtype::chargeRatio(time);

            drawGauge(at, width, ratio, gaugeColor(ratio, {255, 220, 40, 255}));
        }
        if (replica.laserEquipped() && replica.altTime() > 0.f) {
            const float ratio = replica.altTime()
                / rtype::config::LASER_CHARGE_TIME;

            drawGauge({at.x, at.y + ROW}, width, ratio,
                gaugeColor(ratio, {255, 120, 40, 255}));
        }
    }

    Engine::Color GameView::gaugeColor(const float ratio,
        const Engine::Color filling)
    {
        if (ratio >= 1.f) {
            return {90, 230, 255, 255};
        }
        return filling;
    }

    // Le drone : un carre cyan a coeur blanc qui pulse.
    void GameView::drawDrone(const gameLogic::Vec2Pos &pos,
        const gameLogic::Vec2Hitbox &box, const float elapsed)
    {
        const float pulse = 0.85f + 0.15f * std::sin(elapsed * 8.f);
        const float side = box.x * _worldToScreen * pulse;
        const float lift = (box.x * _worldToScreen - side) / 2.f;
        const Engine::Vector2f at{pos.x * _worldToScreen + lift,
            pos.y * _worldToScreen + lift};

        _renderer.drawRect(at, {side, side}, {90, 230, 255, 255});
        _renderer.drawRect({at.x + side * 0.3f, at.y + side * 0.3f},
            {side * 0.4f, side * 0.4f}, {255, 255, 255, 255});
    }

    // Un segment d'eclair : des petits carres le long de la diagonale de sa
    // boite, qui tremblent pour donner l'air electrique.
    void GameView::drawBolt(const gameLogic::Vec2Pos &pos,
        const gameLogic::Vec2Hitbox &box, const bool rising,
        const float elapsed)
    {
        constexpr int STEPS = 14;
        constexpr float DOT = 5.f;
        constexpr float JITTER = 4.f;

        for (int i = 0; i <= STEPS; ++i) {
            const float t = static_cast<float>(i) / STEPS;
            float y = pos.y + box.y * t;

            if (rising) {
                y = pos.y + box.y * (1.f - t);
            }
            const float shake = std::sin(elapsed * 90.f + static_cast<float>(i)
                * 2.3f) * JITTER;

            _renderer.drawRect({(pos.x + box.x * t) * _worldToScreen,
                y * _worldToScreen + shake}, {DOT + 3.f, DOT + 3.f},
                {90, 230, 255, 160});
            _renderer.drawRect({(pos.x + box.x * t) * _worldToScreen + 1.5f,
                y * _worldToScreen + shake + 1.5f}, {DOT, DOT},
                {255, 255, 255, 255});
        }
    }

    // Le rayon du joueur : un coeur blanc dans une lueur cyan qui pulse.
    void GameView::drawPlayerLaser(const gameLogic::Vec2Pos &pos,
        const gameLogic::Vec2Hitbox &box, const float elapsed)
    {
        const float pulse = 0.8f + 0.2f * std::sin(elapsed * 60.f);
        const Engine::Vector2f at{pos.x * _worldToScreen,
            pos.y * _worldToScreen};
        const Engine::Vector2f size{box.x * _worldToScreen,
            box.y * _worldToScreen * pulse};
        const float inset = size.y * 0.3f;

        const float lift = (box.y * _worldToScreen - size.y) / 2.f;

        _renderer.drawRect({at.x, at.y + lift}, size, {90, 230, 255, 140});
        _renderer.drawRect({at.x, at.y + inset}, {size.x, size.y - 2.f * inset},
            {255, 255, 255, 255});
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
        if (id == rtype::SpriteId::PlayerBoltUp
            || id == rtype::SpriteId::PlayerBoltDown) {
            drawBolt(pos, box, id == rtype::SpriteId::PlayerBoltUp, elapsed);
            return;
        }
        if (id == rtype::SpriteId::PlayerDrone) {
            drawDrone(pos, box, elapsed);
            return;
        }
        if (id == rtype::SpriteId::PlayerLaser) {
            drawPlayerLaser(pos, box, elapsed);
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
        const Engine::Vector2f shake = shakeOf(id, elapsed);

        _renderer.drawResized(sprite.texture,
            {pos.x * _worldToScreen + shake.x,
                pos.y * _worldToScreen + shake.y},
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
            drawBeamCore(at, size);
        }
    }

    void GameView::drawBeamCore(const Engine::Vector2f at,
        const Engine::Vector2f size)
    {
        const Engine::Color white{255, 255, 255, 235};

        if (size.x > size.y) {
            _renderer.drawRect({at.x, at.y + size.y * 0.25f},
                {size.x, size.y * 0.5f}, white);
            return;
        }
        _renderer.drawRect({at.x + size.x * 0.25f, at.y},
            {size.x * 0.5f, size.y}, white);
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
