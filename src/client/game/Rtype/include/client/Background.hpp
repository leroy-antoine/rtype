#pragma once

#include <optional>
#include <vector>
#include "engine/window/Renderer.hpp"
#include "logic/Zones.hpp"
#include "SpriteBank.hpp"

namespace client {
    // L'ambiance du boss qui est a l'ecran.
    enum class Aura { None, Standard, Dasher, Summoner, Fortress };

    // Ce que le fond doit montrer : la zone, le boss, la vitesse du joueur.
    struct Scene {
        rtype::Zone zone = rtype::Zone::DeepSpace;
        Aura aura = Aura::None;
        bool boosted = false;
    };

    // Fond du jeu : degrade, nuages, decor propre a la zone, etoiles a trois
    // couches (parallaxe), puis l'aura du boss. Les etoiles defilent vers la
    // gauche a la vitesse du niveau (SCROLL_SPEED). Tout change doucement,
    // sans clignotement : un fondu au noir entre deux zones, des pulsations
    // lentes pendant un boss.
    class Background {
    public:
        Background(const SpriteBank &sprites, float screenWidth,
            float screenHeight, float worldToScreen);

        void draw(Engine::Renderer &renderer, const Scene &scene,
            float elapsed);

    private:
        struct Star {
            float x;
            float y;
            int layer;
        };

        struct Streak {
            float x;
            float y;
            float length;
            float speed;
        };

        static constexpr int LAYERS = 3;
        static constexpr int STARS_PER_LAYER = 60;
        static constexpr int STREAKS = 36;

        void follow(const Scene &scene, float dt);
        void followZone(rtype::Zone zone, float dt);
        void drawSky(Engine::Renderer &renderer, float elapsed) const;
        void drawStars(Engine::Renderer &renderer, float elapsed) const;
        void drawStar(Engine::Renderer &renderer, const Star &star,
            float elapsed) const;
        void drawNebula(Engine::Renderer &renderer, float elapsed) const;
        void drawBand(Engine::Renderer &renderer, BackdropId id, float scale,
            float speed, float y, float elapsed) const;
        void drawDebris(Engine::Renderer &renderer, float elapsed) const;
        void drawStreaks(Engine::Renderer &renderer, float elapsed,
            float speed, Engine::Color color, float strength) const;
        void drawAura(Engine::Renderer &renderer, float elapsed) const;
        void drawVignette(Engine::Renderer &renderer, Engine::Color color,
            float alpha) const;
        void drawBars(Engine::Renderer &renderer, float elapsed) const;
        [[nodiscard]] float rumble(float elapsed) const;
        [[nodiscard]] float warpFactor() const;
        void drawRings(Engine::Renderer &renderer, float elapsed) const;
        void drawFade(Engine::Renderer &renderer) const;

        const SpriteBank &_sprites;
        float _width;
        float _height;
        float _worldToScreen;
        std::vector<Star> _stars;
        std::vector<Streak> _streaks;
        rtype::Zone _zone = rtype::Zone::DeepSpace;
        Aura _aura = Aura::None;
        float _auraLevel = 0.f;
        float _fade = 0.f;
        bool _leaving = false;
        bool _boosted = false;
        bool _started = false;
        float _last = 0.f;
    };
}
