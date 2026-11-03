#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <random>
#include "logic/Config.hpp"
#include "client/Background.hpp"

namespace client {
    namespace {
        using Engine::Color;

        struct Theme {
            Color top;
            Color bottom;
            BackdropId nebula;
            Color star;
            float starSpeed;
        };

        constexpr std::array<Theme, 7> THEMES = {{
            {{4, 4, 18, 255}, {10, 16, 44, 255}, BackdropId::NebulaBlue,
                {200, 215, 255, 255}, 1.f},
            {{14, 4, 30, 255}, {40, 10, 60, 255}, BackdropId::NebulaPurple,
                {255, 215, 255, 255}, 1.f},
            {{12, 8, 10, 255}, {38, 24, 20, 255}, BackdropId::NebulaOrange,
                {255, 235, 210, 255}, 1.f},
            {{4, 14, 16, 255}, {10, 38, 36, 255}, BackdropId::NebulaGreen,
                {210, 255, 235, 255}, 1.f},
            {{26, 2, 4, 255}, {64, 8, 10, 255}, BackdropId::NebulaRed,
                {255, 200, 190, 255}, 0.8f},
            {{2, 2, 10, 255}, {6, 10, 34, 255}, BackdropId::NebulaBlue,
                {190, 235, 255, 255}, 5.f},
            {{18, 2, 10, 255}, {46, 6, 26, 255}, BackdropId::NebulaRed,
                {255, 205, 215, 255}, 1.f}}};

        constexpr std::array<float, 3> SPEED_FACTOR = {0.25f, 0.6f, 1.2f};
        constexpr std::array<float, 3> SIZE = {2.f, 3.f, 4.f};
        constexpr std::array<std::uint8_t, 3> BRIGHTNESS = {90, 160, 235};
        constexpr unsigned int SEED = 20261020;
        constexpr int BANDS = 16;
        constexpr float FADE_TIME = 0.7f;
        constexpr float AURA_RATE = 0.4f;
        constexpr float TWO_PI = 6.2831853f;
        constexpr Color CYAN{120, 230, 255, 255};
        constexpr Color RED{200, 20, 20, 255};
        constexpr Color VIOLET{150, 60, 220, 255};
        constexpr Color FLARE{255, 150, 80, 255};

        std::uint8_t mix(const std::uint8_t a, const std::uint8_t b,
            const float t)
        {
            return static_cast<std::uint8_t>(static_cast<float>(a)
                + (static_cast<float>(b) - static_cast<float>(a)) * t);
        }

        Color lerp(const Color &a, const Color &b, const float t)
        {
            return {mix(a.r, b.r, t), mix(a.g, b.g, t), mix(a.b, b.b, t), 255};
        }

        Color withAlpha(Color color, const float alpha)
        {
            color.a = static_cast<std::uint8_t>(std::clamp(alpha, 0.f, 255.f));
            return color;
        }

        const Theme &themeOf(const rtype::Zone zone)
        {
            return THEMES[static_cast<std::size_t>(zone)];
        }

        float wrap(const float value, const float period)
        {
            const float rest = std::fmod(value, period);

            if (rest < 0.f) {
                return rest + period;
            }
            return rest;
        }
    }

    Background::Background(const SpriteBank &sprites, const float screenWidth,
        const float screenHeight, const float worldToScreen)
        : _sprites(sprites), _width(screenWidth), _height(screenHeight),
        _worldToScreen(worldToScreen)
    {
        std::mt19937 rng(SEED);
        std::uniform_real_distribution<float> x(0.f, screenWidth);
        std::uniform_real_distribution<float> y(0.f, screenHeight);
        std::uniform_real_distribution<float> length(60.f, 240.f);
        std::uniform_real_distribution<float> speed(0.6f, 1.4f);

        for (int layer = 0; layer < LAYERS; ++layer) {
            for (int i = 0; i < STARS_PER_LAYER; ++i) {
                _stars.push_back({x(rng), y(rng), layer});
            }
        }
        for (int i = 0; i < STREAKS; ++i) {
            _streaks.push_back({x(rng), y(rng), length(rng), speed(rng)});
        }
    }

    void Background::followZone(const rtype::Zone zone, const float dt)
    {
        if (!_started) {
            _zone = zone;
            _started = true;
        }
        if (zone != _zone) {
            _leaving = true;
        }
        if (!_leaving) {
            _fade = std::max(0.f, _fade - dt / FADE_TIME);
            return;
        }
        _fade = std::min(1.f, _fade + dt / FADE_TIME);
        if (_fade >= 1.f) {
            _zone = zone;
            _leaving = false;
        }
    }

    void Background::follow(const Scene &scene, const float dt)
    {
        float target = 0.f;

        followZone(scene.zone, dt);
        if (scene.aura != Aura::None) {
            _aura = scene.aura;
            target = 1.f;
        }
        _auraLevel = std::clamp(_auraLevel + (target - _auraLevel) * AURA_RATE
            * dt * 2.f, 0.f, 1.f);
        _boosted = scene.boosted;
    }

    void Background::draw(Engine::Renderer &renderer, const Scene &scene,
        const float elapsed)
    {
        const float dt = std::clamp(elapsed - _last, 0.f, 0.1f);

        _last = elapsed;
        follow(scene, dt);
        drawSky(renderer, elapsed);
        drawNebula(renderer, elapsed);
        drawDebris(renderer, elapsed);
        drawStars(renderer, elapsed);
        drawAura(renderer, elapsed);
        drawFade(renderer);
    }

    void Background::drawSky(Engine::Renderer &renderer,
        const float) const
    {
        const Theme &theme = themeOf(_zone);
        const float band = _height / static_cast<float>(BANDS);

        for (int i = 0; i < BANDS; ++i) {
            const float t = static_cast<float>(i) / (BANDS - 1);

            renderer.drawRect({0.f, band * static_cast<float>(i)},
                {_width, band + 1.f}, lerp(theme.top, theme.bottom, t));
        }
    }

    float Background::warpFactor() const
    {
        if (_boosted) {
            return 10.5f;
        }
        return 7.f;
    }

    float Background::rumble(const float elapsed) const
    {
        if (_aura != Aura::Fortress) {
            return 0.f;
        }
        return 4.f * _auraLevel * std::sin(elapsed * 7.f);
    }

    void Background::drawBand(Engine::Renderer &renderer, const BackdropId id,
        const float scale, const float speed, const float y,
        const float elapsed) const
    {
        const Engine::Sprite &sprite = _sprites.backdrop(id);
        const Engine::IntRect frame = sprite.animation.firstFrame;
        const float width = static_cast<float>(frame.width) * scale;
        const float height = static_cast<float>(frame.height) * scale;

        for (float x = -wrap(elapsed * speed, width); x < _width; x += width) {
            renderer.drawResized(sprite.texture, {x, y + rumble(elapsed)},
                frame, {width, height});
        }
    }

    void Background::drawNebula(Engine::Renderer &renderer,
        const float elapsed) const
    {
        const float speed = rtype::config::SCROLL_SPEED * _worldToScreen;
        const BackdropId id = themeOf(_zone).nebula;
        const float sway = 8.f * std::sin(elapsed * 0.2f);

        drawBand(renderer, id, _width / 640.f, speed * 0.12f, sway, elapsed);
    }

    void Background::drawDebris(Engine::Renderer &renderer,
        const float elapsed) const
    {
        const float speed = rtype::config::SCROLL_SPEED * _worldToScreen;

        if (_zone == rtype::Zone::Asteroids) {
            drawBand(renderer, BackdropId::Rocks, 1.2f, speed * 0.45f, 60.f,
                elapsed);
            drawBand(renderer, BackdropId::Rocks, 2.2f, speed * 0.85f, 360.f,
                elapsed);
        } else if (_zone == rtype::Zone::Wreckage) {
            drawBand(renderer, BackdropId::Girders, 1.6f, speed * 0.5f, 0.f,
                elapsed);
            renderer.drawRect({0.f, _height - 36.f}, {_width, 36.f},
                {14, 22, 24, 230});
        } else if (_zone == rtype::Zone::Core) {
            drawVignette(renderer, FLARE,
                40.f + 24.f * std::sin(elapsed * 1.9f));
        } else if (_zone == rtype::Zone::Warp) {
            drawStreaks(renderer, elapsed, speed * warpFactor(), CYAN, 1.f);
        }
    }

    void Background::drawStars(Engine::Renderer &renderer,
        const float elapsed) const
    {
        for (const Star &star : _stars) {
            drawStar(renderer, star, elapsed);
        }
    }

    void Background::drawStar(Engine::Renderer &renderer, const Star &star,
        const float elapsed) const
    {
        const Theme &theme = themeOf(_zone);
        const auto layer = static_cast<std::size_t>(star.layer);
        const float speed = rtype::config::SCROLL_SPEED * _worldToScreen
            * SPEED_FACTOR[layer] * theme.starSpeed;
        const float x = wrap(star.x - elapsed * speed, _width);
        const float b = static_cast<float>(BRIGHTNESS[layer]) / 255.f;
        const Color color = lerp({0, 0, 0, 255}, theme.star, b);
        const float length = SIZE[layer] * std::max(1.f, theme.starSpeed / 2.f);

        renderer.drawRect({x, star.y}, {length, SIZE[layer]}, color);
    }

    void Background::drawStreaks(Engine::Renderer &renderer,
        const float elapsed, const float speed, const Engine::Color color,
        const float strength) const
    {
        for (const Streak &streak : _streaks) {
            const float moving = speed * streak.speed;
            const float span = _width + streak.length;
            const float x = wrap(streak.x - elapsed * moving, span)
                - streak.length;

            renderer.drawRect({x, streak.y}, {streak.length, 2.f},
                withAlpha(color, 90.f * strength));
        }
    }

    void Background::drawVignette(Engine::Renderer &renderer,
        const Engine::Color color, const float alpha) const
    {
        constexpr int LAYERS_DEEP = 6;
        constexpr float DEPTH = 36.f;

        for (int k = 0; k < LAYERS_DEEP; ++k) {
            const float fade = 1.f - static_cast<float>(k) / LAYERS_DEEP;
            const Color c = withAlpha(color, alpha * fade * fade / 2.f);
            const float d = DEPTH * static_cast<float>(k);

            renderer.drawRect({0.f, d}, {_width, DEPTH}, c);
            renderer.drawRect({0.f, _height - d - DEPTH}, {_width, DEPTH}, c);
            renderer.drawRect({d, 0.f}, {DEPTH, _height}, c);
            renderer.drawRect({_width - d - DEPTH, 0.f}, {DEPTH, _height}, c);
        }
    }

    void Background::drawBars(Engine::Renderer &renderer,
        const float elapsed) const
    {
        for (int i = 0; i < 5; ++i) {
            const float phase =
                std::sin(elapsed * 1.1f + static_cast<float>(i));
            const float alpha = 26.f + 22.f * phase;

            renderer.drawRect({0.f, _height * (0.12f + 0.19f
                * static_cast<float>(i))}, {_width, 6.f},
                withAlpha(RED, alpha * _auraLevel));
        }
    }

    void Background::drawRings(Engine::Renderer &renderer,
        const float elapsed) const
    {
        const Engine::Sprite &ring = _sprites.backdrop(BackdropId::Ring);

        for (int i = 0; i < 3; ++i) {
            const float phase = wrap(elapsed * 0.16f
                + static_cast<float>(i) / 3.f, 1.f);
            const float size = 90.f + phase * 1300.f;

            renderer.drawResized(ring.texture,
                {_width * 0.72f - size / 2.f, _height / 2.f - size / 2.f},
                ring.animation.firstFrame, {size, size});
        }
    }

    void Background::drawAura(Engine::Renderer &renderer,
        const float elapsed) const
    {
        const float speed = rtype::config::SCROLL_SPEED * _worldToScreen;
        const float pulse = 0.65f + 0.35f * std::sin(elapsed * 2.4f);

        if (_auraLevel <= 0.f || _aura == Aura::None) {
            return;
        }
        if (_aura == Aura::Standard) {
            drawVignette(renderer, RED, 110.f * _auraLevel * pulse);
        } else if (_aura == Aura::Dasher) {
            drawStreaks(renderer, elapsed, speed * 9.f, CYAN,
                1.2f * _auraLevel);
        } else if (_aura == Aura::Summoner) {
            renderer.drawRect({0.f, 0.f}, {_width, _height},
                withAlpha(VIOLET, 22.f * _auraLevel));
            drawRings(renderer, elapsed);
        } else {
            drawVignette(renderer, RED, 170.f * _auraLevel * pulse);
            drawBars(renderer, elapsed);
        }
    }

    void Background::drawFade(Engine::Renderer &renderer) const
    {
        if (_fade > 0.f) {
            renderer.drawRect({0.f, 0.f}, {_width, _height},
                {0, 0, 0, static_cast<std::uint8_t>(255.f * _fade)});
        }
    }
}
