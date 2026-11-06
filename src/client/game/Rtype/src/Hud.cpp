#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include "logic/Config.hpp"
#include "client/AugmentStyle.hpp"
#include "client/Hud.hpp"
#include "client/TimeFormat.hpp"

namespace client {
    namespace {
        using Engine::Color;

        constexpr Color WHITE{255, 255, 255, 255};
        constexpr Color DARK{20, 20, 30, 255};
        constexpr Color GREEN{90, 220, 90, 255};
        constexpr Color RED{235, 70, 60, 255};
        constexpr Color YELLOW{255, 220, 40, 255};
        constexpr Color MAGENTA{235, 90, 255, 255};
        constexpr Color CYAN{90, 230, 255, 255};

        // Une couleur par slot, comme les vaisseaux.
        constexpr std::array<Color, 4> SLOT_COLORS = {{
            {90, 170, 255, 255}, {255, 90, 90, 255},
            {110, 230, 110, 255}, {255, 220, 80, 255}}};

        std::string shopHint(const rtype::GameStatus &status,
            const std::optional<int> self)
        {
            const bool ready = std::ranges::any_of(status.players,
                [&self](const rtype::PlayerStatus &p) {
                    return self && p.slot == *self && p.ready;
                });

            if (ready) {
                return "WAITING FOR THE OTHER PLAYERS";
            }
            return "PRESS SPACE WHEN READY";
        }
    }

    Hud::Hud(Engine::Renderer &renderer, const SpriteBank &sprites,
        const float screenWidth, const float screenHeight)
        : _renderer(renderer), _sprites(sprites), _font(renderer), _width(screenWidth),
        _height(screenHeight)
    {
    }

    void Hud::draw(const Replica &replica, const LobbyMenu &menu,
        const float elapsed)
    {
        const std::optional<rtype::GameStatus> &status = replica.status();

        if (!status) {
            _font.drawCentered("CONNECTING...", _width / 2.f, _height / 2.f,
                4.f, WHITE);
            return;
        }
        trackPhase(status->phase, elapsed);
        if (status->phase == rtype::Phase::Lobby) {
            drawLobby(replica, menu);
            return;
        }
        drawPlayers(*status, replica.augments());
        drawCoins(*status, elapsed);
        drawBossBar(*status);
        if (status->phase == rtype::Phase::Victory) {
            drawVictoryScreen(*status, replica, elapsed);
        } else if (status->phase == rtype::Phase::Shop) {
            drawShopScreen(*status, replica, elapsed);
        } else if (status->phase != rtype::Phase::Playing) {
            drawEndScreen(elapsed);
        } else if (!status->paused) {
            drawRespawnHint(*status, replica.slot(), elapsed);
            drawMenuHint(replica);
        }
        drawTimer(*status, elapsed);
        drawLevel(*status);
        drawEffects(replica);
        if (status->paused) {
            drawPauseMenu(replica);
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
        float top = PLAYERS_TOP;

        for (const rtype::PlayerStatus &player : status.players) {
            const auto slot = static_cast<std::size_t>(player.slot);
            const float ratio = static_cast<float>(player.hp)
                / static_cast<float>(rtype::config::PLAYER_HP);

            _font.draw("P" + std::to_string(player.slot + 1), {MARGIN, top},
                2.f, SLOT_COLORS[slot]);
            drawBar({BAR_X, top}, {BAR_WIDTH, BAR_HEIGHT}, ratio, GREEN);
            float x = BAR_X + BAR_WIDTH + AUGMENT_GAP;

            if (status.phase == rtype::Phase::Shop) {
                x = drawReady(player, x, top);
            }
            if (slot < augments.size()) {
                drawAugments(augments[slot], x, top);
            }
            top += ROW_STEP;
        }
    }

    float Hud::textTop()
    {
        return MARGIN + (ICON_SIZE - Font::height(TEXT_SCALE)) / 2.f;
    }

    void Hud::drawIcon(const Engine::Sprite &sprite,
        const Engine::Vector2f position, const float elapsed)
    {
        _renderer.drawResized(sprite.texture, position,
            sprite.animation.frameAt(elapsed), {ICON_SIZE, ICON_SIZE});
    }

    void Hud::drawTimer(const rtype::GameStatus &status, const float elapsed)
    {
        const std::string text = formatTime(status.elapsedMs);
        const float iconX = MARGIN + Font::width(text, TEXT_SCALE) + ICON_GAP;

        _font.draw(text, {MARGIN, textTop()}, TEXT_SCALE, WHITE);
        drawIcon(_sprites.clock(), {iconX, MARGIN}, elapsed);
    }

    void Hud::drawLevel(const rtype::GameStatus &status)
    {
        _font.drawCentered("LEVEL " + std::to_string(status.level),
            _width / 2.f, textTop(), TEXT_SCALE, WHITE);
    }

    void Hud::drawEffects(const Replica &replica)
    {
        const Prediction &prediction = replica.prediction();
        float top = _height - 90.f;

        if (prediction.inverted()) {
            _font.draw("CONTROLS REVERSED", {MARGIN, top}, 3.f, MAGENTA);
            top -= 36.f;
        }
        if (prediction.speedFactor() > 1.f) {
            _font.draw("SPEED UP", {MARGIN, top}, 3.f, CYAN);
        }
    }

    void Hud::drawAugments(const std::vector<rtype::AugmentId> &ids,
        float x, const float top)
    {
        const float textHeight = Font::height(AUGMENT_TEXT_SCALE);
        const float textY = top + (BAR_HEIGHT - textHeight) / 2.f;

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
        const float textX = _width - MARGIN - Font::width(text, TEXT_SCALE);
        const float iconX = textX - ICON_GAP - ICON_SIZE;

        drawIcon(_sprites.get(rtype::SpriteId::Coin), {iconX, MARGIN}, elapsed);
        _font.draw(text, {textX, textTop()}, TEXT_SCALE, YELLOW);
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

    void Hud::drawMenuHint(const Replica &replica)
    {
        if (replica.isHost()) {
            _font.drawCentered("BACKSPACE: PAUSE", _width - 120.f,
                _height - 20.f, 2.f, {160, 160, 160, 255});
        }
    }

    void Hud::drawPauseMenu(const Replica &replica)
    {
        std::string hint = "WAITING FOR P1 TO RESUME";

        if (replica.isHost()) {
            hint = "BACKSPACE: RESUME   ENTER: MAP MENU";
        }
        _renderer.drawRect({0.f, 0.f}, {_width, _height}, {0, 0, 0, 170});
        _font.drawCentered("PAUSED", _width / 2.f, _height / 2.f - 40.f, 7.f,
            WHITE);
        _font.drawCentered(hint, _width / 2.f, _height / 2.f + 40.f, 3.f,
            WHITE);
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

    void Hud::drawEndScreen(const float elapsed)
    {
        const float left = rtype::config::RESTART_DELAY - (elapsed - _phaseSince);
        const int seconds = std::max(0, static_cast<int>(std::ceil(left)));

        _renderer.drawRect({0.f, 0.f}, {_width, _height}, {0, 0, 0, 150});
        _font.drawCentered("GAME OVER", _width / 2.f, _height / 2.f - 50.f,
            10.f, RED);
        _font.drawCentered("NEW GAME IN " + std::to_string(seconds),
            _width / 2.f, _height / 2.f + 50.f, 3.f, WHITE);
    }

    float Hud::drawReady(const rtype::PlayerStatus &player, const float x,
        const float top)
    {
        const std::string label = player.ready ? "READY" : "NOT READY";

        _font.draw(label, {x, top}, 2.f, player.ready ? GREEN : RED);
        return x + Font::width(label, 2.f) + AUGMENT_GAP;
    }

    void Hud::drawShopScreen(const rtype::GameStatus &status,
        const Replica &replica, const float elapsed)
    {
        const float bottom = _height - SHOP_TEXT_MARGIN;

        if (replica.shopOffers().empty()) {
            _font.drawCentered("SHOP", _width / 2.f, _height / 2.f - 50.f,
                10.f, YELLOW);
        }
        if (status.countdown > 0.f) {
            _font.drawCentered("NEXT LEVEL IN "
                + std::to_string(std::lround(std::ceil(status.countdown))),
                _width / 2.f, bottom, 3.f, WHITE);
            return;
        }
        if (std::fmod(elapsed, 1.f) < 0.5f) {
            _font.drawCentered(shopHint(status, replica.slot()), _width / 2.f,
                bottom, 3.f, WHITE);
        }
    }
}
