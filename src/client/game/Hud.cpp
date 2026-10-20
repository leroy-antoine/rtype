#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <string>
#include "Config.hpp"
#include "Hud.hpp"

namespace client {
    namespace {
        using Engine::Color;

        constexpr Color WHITE{255, 255, 255, 255};
        constexpr Color DARK{20, 20, 30, 255};
        constexpr Color GREEN{90, 220, 90, 255};
        constexpr Color RED{235, 70, 60, 255};

        // Une couleur par slot, comme les vaisseaux.
        constexpr std::array<Color, 4> SLOT_COLORS = {{
            {90, 170, 255, 255}, {255, 90, 90, 255},
            {110, 230, 110, 255}, {255, 220, 80, 255}}};
    }

    Hud::Hud(Engine::Renderer &renderer, const float screenWidth,
        const float screenHeight)
        : _renderer(renderer), _font(renderer), _width(screenWidth),
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
        drawPlayers(*status);
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
    void Hud::drawPlayers(const rtype::GameStatus &status)
    {
        float top = 16.f;

        for (const rtype::PlayerStatus &player : status.players) {
            const auto slot = static_cast<std::size_t>(player.slot);
            const float ratio = static_cast<float>(player.hp)
                / static_cast<float>(rtype::config::PLAYER_HP);

            _font.draw("P" + std::to_string(player.slot + 1), {16.f, top},
                2.f, SLOT_COLORS[slot]);
            drawBar({BAR_X, top}, {BAR_WIDTH, BAR_HEIGHT}, ratio, GREEN);
            top += ROW_STEP;
        }
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
