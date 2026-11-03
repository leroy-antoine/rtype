#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>
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
        constexpr Color MAGENTA{235, 90, 255, 255};
        constexpr Color CYAN{90, 230, 255, 255};

        // Une couleur par slot, comme les vaisseaux.
        constexpr std::uint32_t MS_PER_MINUTE = 60000;
        constexpr std::uint32_t MS_PER_SECOND = 1000;
        constexpr std::uint32_t MS_PER_TENTH = 100;

        std::string twoDigits(const std::uint32_t value)
        {
            std::string text = std::to_string(value);

            if (text.size() < 2) {
                text.insert(0, "0");
            }
            return text;
        }

        std::string formatTime(const std::uint32_t ms)
        {
            const std::uint32_t minutes = ms / MS_PER_MINUTE;
            const std::uint32_t seconds = ms / MS_PER_SECOND % 60;
            const std::uint32_t tenths = ms / MS_PER_TENTH % 10;

            return twoDigits(minutes) + ":" + twoDigits(seconds) + "."
                + std::to_string(tenths);
        }

        constexpr std::array<Color, 4> SLOT_COLORS = {{
            {90, 170, 255, 255}, {255, 90, 90, 255},
            {110, 230, 110, 255}, {255, 220, 80, 255}}};
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
        if (status->phase == rtype::Phase::Lobby) {
            drawLobby(replica);
            return;
        }
        drawPlayers(*status);
        drawCoins(*status, elapsed);
        drawBossBar(*status);
        if (status->phase != rtype::Phase::Playing) {
            drawEndScreen(*status, elapsed);
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
    void Hud::drawPlayers(const rtype::GameStatus &status)
    {
        float top = PLAYERS_TOP;

        for (const rtype::PlayerStatus &player : status.players) {
            const auto slot = static_cast<std::size_t>(player.slot);
            const float ratio = static_cast<float>(player.hp)
                / static_cast<float>(rtype::config::PLAYER_HP);

            _font.draw("P" + std::to_string(player.slot + 1), {MARGIN, top},
                2.f, SLOT_COLORS[slot]);
            drawBar({BAR_X, top}, {BAR_WIDTH, BAR_HEIGHT}, ratio, GREEN);
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

    void Hud::drawLobby(const Replica &replica)
    {
        const rtype::MapId cursor = replica.mapCursor();
        std::string footer = "WAITING FOR P1 TO CHOOSE THE MAP";

        if (replica.isHost()) {
            footer = "UP/DOWN: CHOOSE   ENTER: START";
        }
        _renderer.drawRect({0.f, 0.f}, {_width, _height}, {0, 0, 0, 210});
        _font.drawCentered("SELECT A MAP", _width / 2.f, 50.f, 6.f, WHITE);
        drawMapList(cursor);
        drawRecords(replica.records()[cursor]);
        _font.drawCentered(footer, _width / 2.f, _height - 50.f, 3.f, WHITE);
    }

    void Hud::drawMapList(const rtype::MapId cursor)
    {
        constexpr float TOP = 150.f;
        constexpr float STEP = 52.f;
        constexpr float STRIP_WIDTH = 520.f;

        for (std::size_t i = 0; i < rtype::MAP_COUNT; ++i) {
            const float y = TOP + static_cast<float>(i) * STEP;
            Color color = WHITE;

            if (i == cursor) {
                _renderer.drawRect({(_width - STRIP_WIDTH) / 2.f, y - 8.f},
                    {STRIP_WIDTH, STEP - 6.f}, {40, 70, 140, 255});
                color = YELLOW;
            }
            _font.drawCentered(rtype::MAP_NAMES[i], _width / 2.f, y, 4.f,
                color);
        }
    }

    void Hud::drawRecords(const rtype::MapRecords &records)
    {
        constexpr float TOP = 360.f;
        constexpr float STEP = 36.f;

        _font.drawCentered("BEST TIMES", _width / 2.f, TOP, 3.f, WHITE);
        if (records.empty()) {
            _font.drawCentered("NO RECORD YET", _width / 2.f, TOP + STEP,
                3.f, GREEN);
        }
        for (std::size_t i = 0; i < records.size(); ++i) {
            const std::string line = std::to_string(i + 1) + ". "
                + formatTime(records[i].timeMs) + "  "
                + std::to_string(records[i].players) + "P";

            _font.drawCentered(line, _width / 2.f,
                TOP + STEP * static_cast<float>(i + 1), 3.f, GREEN);
        }
    }
}
