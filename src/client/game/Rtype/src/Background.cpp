#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <random>
#include "logic/Config.hpp"
#include "client/Background.hpp"

namespace client {
    namespace {
        // Par couche, de la plus lointaine a la plus proche.
        constexpr std::array<float, 3> SPEED_FACTOR = {0.25f, 0.6f, 1.2f};
        constexpr std::array<float, 3> SIZE = {2.f, 3.f, 4.f};
        constexpr std::array<std::uint8_t, 3> BRIGHTNESS = {90, 160, 235};
        constexpr unsigned int SEED = 20261020;
    }

    Background::Background(const float screenWidth, const float screenHeight,
        const float worldToScreen)
        : _width(screenWidth), _worldToScreen(worldToScreen)
    {
        std::mt19937 rng(SEED);
        std::uniform_real_distribution<float> x(0.f, screenWidth);
        std::uniform_real_distribution<float> y(0.f, screenHeight);

        for (int layer = 0; layer < LAYERS; ++layer) {
            for (int i = 0; i < STARS_PER_LAYER; ++i) {
                _stars.push_back({x(rng), y(rng), layer});
            }
        }
    }

    void Background::draw(Engine::Renderer &renderer,
        const float elapsed) const
    {
        for (const Star &star : _stars) {
            drawStar(renderer, star, elapsed);
        }
    }

    void Background::drawStar(Engine::Renderer &renderer, const Star &star,
        const float elapsed) const
    {
        const auto layer = static_cast<std::size_t>(star.layer);
        const float speed = rtype::config::SCROLL_SPEED * _worldToScreen
            * SPEED_FACTOR[layer];
        float x = std::fmod(star.x - elapsed * speed, _width);
        const std::uint8_t b = BRIGHTNESS[layer];

        if (x < 0.f) {
            x += _width;
        }
        renderer.drawRect({x, star.y}, {SIZE[layer], SIZE[layer]},
            {b, b, static_cast<std::uint8_t>(std::min(255, b + 20)), 255});
    }
}
