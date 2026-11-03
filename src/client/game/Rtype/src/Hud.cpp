#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <optional>
#include <string>
#include <vector>
#include "logic/Config.hpp"
#include "client/Hud.hpp"

namespace client {
    namespace {
        using Engine::Color;

        constexpr Color WHITE{255, 255, 255, 255};
        constexpr Color DARK{20, 20, 30, 255};
        constexpr Color GREEN{90, 220, 90, 255};
        constexpr Color RED{235, 70, 60, 255};
        constexpr Color YELLOW{255, 220, 40, 255};

        // Une couleur par slot, comme les vaisseaux.
        constexpr std::array<Color, 4> SLOT_COLORS = {{
            {90, 170, 255, 255}, {255, 90, 90, 255},
            {110, 230, 110, 255}, {255, 220, 80, 255}}};

        std::string augmentLabel(const rtype::AugmentId id)
        {
            switch (id) {
                case rtype::AugmentId::DamageUp: return "DMG";
                case rtype::AugmentId::SpeedUp: return "SPD";
                case rtype::AugmentId::MaxHpUp: return "HP";
                case rtype::AugmentId::FireRateUp: return "FR";
                case rtype::AugmentId::Count: break;
            }
            return "?";
        }

        Color rarityColor(const rtype::AugmentId id)
        {
            const std::optional<rtype::AugmentDef> def = rtype::findAugment(id);

            if (!def) {
                return WHITE;
            }
            switch (def->rarity) {
                case rtype::Rarity::Common: return {170, 170, 170, 255};
                case rtype::Rarity::Rare: return {80, 150, 255, 255};
                case rtype::Rarity::Epic: return {190, 90, 255, 255};
                case rtype::Rarity::Legendary: return {255, 165, 0, 255};
            }
            return WHITE;
        }
    }

    Hud::Hud(Engine::Renderer &renderer, const SpriteBank &sprites,
        const float screenWidth, const float screenHeight)
        : _renderer(renderer), _sprites(sprites), _font(renderer), _width(screenWidth),
        _height(screenHeight)
    {
    }

    void Hud::draw(const Replica &replica, const float elapsed)
    {
        const std::optional<rtype::GameStatus> &status = replica.status();

        if (!status) {
            _font.drawCentered("CONNECTING...", _width / 2.f, _height / 2.f,
                4.f, WHITE);
            return;
        }
        trackPhase(status->phase, elapsed);
        drawPlayers(*status, replica.augments());
        drawCoins(*status, elapsed);
        drawBossBar(*status);
        if (status->phase == rtype::Phase::Playing) {
            drawRespawnHint(*status, replica.slot(), elapsed);
        } else {
            drawEndScreen(*status, elapsed);
        }
    }

    // Retient depuis quand la partie est finie, pour le compte a rebours.
    void Hud::trackPhase(const rtype::Phase phase, const float elapsed)
    {
        if (phase != _phase) {
            _phase = phase;
            _phaseSince = elapsed;
        }
    }

    // Une ligne par joueur : son nom (P1...) et sa barre de vie.
    void Hud::drawPlayers(const rtype::GameStatus &status,
        const rtype::protocol::OwnedAugment &augments)
    {
        float top = 16.f;

        for (const rtype::PlayerStatus &player : status.players) {
            const auto slot = static_cast<std::size_t>(player.slot);
            const float ratio = static_cast<float>(player.hp)
                / static_cast<float>(rtype::config::PLAYER_HP);

            _font.draw("P" + std::to_string(player.slot + 1), {16.f, top},
                2.f, SLOT_COLORS[slot]);
            drawBar({BAR_X, top}, {BAR_WIDTH, BAR_HEIGHT}, ratio, GREEN);
            if (slot < augments.size()) {
                drawAugments(augments[slot], top);
            }
            top += ROW_STEP;
        }
    }

    void Hud::drawAugments(const std::vector<rtype::AugmentId> &ids,
        const float top)
    {
        const float textHeight = Font::height(AUGMENT_TEXT_SCALE);
        const float textY = top + (BAR_HEIGHT - textHeight) / 2.f;
        float x = BAR_X + BAR_WIDTH + AUGMENT_GAP;

        for (const rtype::AugmentId id : ids) {
            const std::string label = augmentLabel(id);
            const float textWidth = Font::width(label, AUGMENT_TEXT_SCALE);

            _renderer.drawRect({x - AUGMENT_PADDING, textY - AUGMENT_PADDING},
                {textWidth + 2.f * AUGMENT_PADDING,
                    textHeight + 2.f * AUGMENT_PADDING}, DARK);
            _font.draw(label, {x, textY}, AUGMENT_TEXT_SCALE,
                rarityColor(id));
            x += textWidth + 2.f * AUGMENT_PADDING + AUGMENT_PADDING;
        }
    }

    void Hud::drawCoins(const rtype::GameStatus &status, const float elapsed)
    {
        const std::string text = std::to_string(status.coins);
        const float textWidth = Font::width(text, COIN_TEXT_SCALE);
        const float textX = _width - COIN_MARGIN - textWidth;
        const float iconX = textX - COIN_ICON_GAP - COIN_ICON_SIZE;
        const float textY = COIN_MARGIN
            + (COIN_ICON_SIZE - Font::height(COIN_TEXT_SCALE)) / 2.f;
        const Engine::Sprite &sprite = _sprites.get(rtype::SpriteId::Coin);

        _renderer.drawResized(sprite.texture, {iconX, COIN_MARGIN},
            sprite.animation.frameAt(elapsed),
            {COIN_ICON_SIZE, COIN_ICON_SIZE});
        _font.draw(text, {textX, textY}, COIN_TEXT_SCALE, YELLOW);
    }

    void Hud::drawBar(const Engine::Vector2f position,
        const Engine::Vector2f size, const float ratio, const Color fill)
    {
        const float clamped = std::clamp(ratio, 0.f, 1.f);

        _renderer.drawRect({position.x - 2.f, position.y - 2.f},
            {size.x + 4.f, size.y + 4.f}, DARK);
        _renderer.drawRect(position, {size.x * clamped, size.y}, fill);
    }

    void Hud::drawBossBar(const rtype::GameStatus &status)
    {
        if (status.bossMaxHp <= 0 || status.phase != rtype::Phase::Playing) {
            return;
        }
        const float ratio = static_cast<float>(status.bossHp)
            / static_cast<float>(status.bossMaxHp);
        const float top = _height - 38.f;

        _font.drawCentered("BOSS", _width / 2.f, top - 22.f, 2.f, WHITE);
        drawBar({(_width - BOSS_BAR_WIDTH) / 2.f, top}, {BOSS_BAR_WIDTH, 16.f},
            ratio, RED);
    }

    // Message au joueur dont le vaisseau est detruit.
    void Hud::drawRespawnHint(const rtype::GameStatus &status,
        const std::optional<int> self, const float elapsed)
    {
        const auto mine = std::ranges::find_if(status.players,
            [&self](const rtype::PlayerStatus &p) {
                return self && p.slot == *self;
            });

        if (mine == status.players.end() || mine->alive) {
            return;
        }
        if (static_cast<int>(elapsed * 2.f) % 2 == 0) {
            _font.drawCentered("PRESS ENTER TO RESPAWN", _width / 2.f,
                _height / 2.f, 4.f, WHITE);
        }
    }

    void Hud::drawEndScreen(const rtype::GameStatus &status,
        const float elapsed)
    {
        const float left = rtype::config::RESTART_DELAY - (elapsed - _phaseSince);
        const int seconds = std::max(0, static_cast<int>(std::ceil(left)));
        std::string title = "GAME OVER";
        Color color = RED;

        if (status.phase == rtype::Phase::Victory) {
            title = "VICTORY";
            color = GREEN;
        }
        _renderer.drawRect({0.f, 0.f}, {_width, _height}, {0, 0, 0, 150});
        _font.drawCentered(title, _width / 2.f, _height / 2.f - 50.f, 10.f,
            color);
        _font.drawCentered("NEW GAME IN " + std::to_string(seconds),
            _width / 2.f, _height / 2.f + 50.f, 3.f, WHITE);
    }
}
