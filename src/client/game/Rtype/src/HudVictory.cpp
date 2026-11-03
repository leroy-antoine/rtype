#include <algorithm>
#include <cmath>
#include <string>
#include "client/Hud.hpp"
#include "logic/Config.hpp"

namespace client {
    namespace {
        using Engine::Color;

        constexpr Color WHITE{255, 255, 255, 255};
        constexpr Color GREEN{90, 220, 90, 255};
        constexpr Color RED{235, 70, 60, 255};
        constexpr Color YELLOW{255, 220, 40, 255};
        constexpr int CONFETTI = 90;
        constexpr float TALLY_TIME = 1.6f;
        constexpr float FALL_SPEED = 90.f;
        constexpr float PI = 3.14159265f;

        std::string clock(const std::uint32_t ms)
        {
            const std::uint32_t minutes = ms / 60000;
            const std::uint32_t seconds = ms / 1000 % 60;
            const std::uint32_t tenths = ms / 100 % 10;
            std::string text = std::to_string(minutes) + ":";

            if (seconds < 10) {
                text += "0";
            }
            return text + std::to_string(seconds) + "."
                + std::to_string(tenths);
        }

        Color confettiColor(const int index)
        {
            constexpr Color PALETTE[4] = {{255, 220, 40, 255},
                {90, 220, 90, 255}, {90, 200, 255, 255}, {255, 90, 200, 255}};

            return PALETTE[index % 4];
        }

        float hash(const int index, const int salt)
        {
            const float value = std::sin(static_cast<float>(index * 127
                + salt * 311)) * 43758.5453f;

            return value - std::floor(value);
        }
    }

    void Hud::drawConfetti(const float age)
    {
        for (int i = 0; i < CONFETTI; ++i) {
            const float speed = FALL_SPEED * (0.6f + hash(i, 1));
            const float fall = hash(i, 2) * _height + age * speed;
            const float y = std::fmod(fall, _height + 20.f) - 10.f;
            const float sway = std::sin(age * 2.f + hash(i, 3) * 2.f * PI);
            const float x = hash(i, 4) * _width + sway * 24.f;
            const float size = 5.f + hash(i, 5) * 6.f;

            _renderer.drawRect({x, y}, {size, size * 0.6f}, confettiColor(i));
        }
    }

    void Hud::drawVictoryTitle(const float age)
    {
        const float pulse = 10.f + std::sin(age * 4.f) * 0.8f;
        const float rise = std::min(1.f, age / 0.5f);
        const float y = _height * 0.2f - (1.f - rise) * 60.f;

        _font.drawCentered("VICTORY", _width / 2.f, y, pulse, GREEN);
    }

    Engine::Color Hud::timeColor(const rtype::GameStatus &status,
        const Replica &replica) const
    {
        const auto &all = replica.records();

        if (status.map >= all.size()) {
            return WHITE;
        }
        const rtype::MapRecords &records = all[status.map];

        if (records.empty() || status.elapsedMs <= records.front().timeMs) {
            return GREEN;
        }
        return RED;
    }

    void Hud::drawVictoryStats(const rtype::GameStatus &status,
        const Replica &replica, const float age)
    {
        const float tally = std::min(1.f, age / TALLY_TIME);
        const auto count = [tally](const std::uint32_t value) {
            return std::to_string(static_cast<std::uint32_t>(
                static_cast<float>(value) * tally));
        };
        const float middle = _width / 2.f;
        const float top = _height * 0.42f;

        _font.drawCentered("DEATHS  " + count(status.deaths), middle, top,
            4.f, WHITE);
        _font.drawCentered("COINS  " + count(status.coins), middle,
            top + 60.f, 4.f, YELLOW);
        _font.drawCentered("TIME  " + clock(static_cast<std::uint32_t>(
            static_cast<float>(status.elapsedMs) * tally)), middle,
            top + 120.f, 5.f, timeColor(status, replica));
    }

    void Hud::drawVictoryScreen(const rtype::GameStatus &status,
        const Replica &replica, const float elapsed)
    {
        const float age = elapsed - _phaseSince;
        const float left = rtype::config::RESTART_DELAY - age;
        const int seconds = std::max(0, static_cast<int>(std::ceil(left)));

        _renderer.drawRect({0.f, 0.f}, {_width, _height}, {0, 0, 0, 170});
        drawConfetti(age);
        drawVictoryTitle(age);
        drawVictoryStats(status, replica, age);
        _font.drawCentered("BACK TO THE MENU IN " + std::to_string(seconds),
            _width / 2.f, _height * 0.85f, 3.f, WHITE);
    }
}
