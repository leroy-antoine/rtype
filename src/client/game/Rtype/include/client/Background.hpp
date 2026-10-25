#pragma once

#include <vector>
#include "engine/window/Renderer.hpp"

namespace client {
    // Fond d'etoiles a trois couches (parallaxe). Les etoiles defilent vers la
    // gauche a la vitesse du niveau (SCROLL_SPEED), les plus proches plus vite.
    class Background {
    public:
        Background(float screenWidth, float screenHeight, float worldToScreen);

        void draw(Engine::Renderer &renderer, float elapsed) const;

    private:
        struct Star {
            float x;
            float y;
            int layer;
        };

        static constexpr int LAYERS = 3;
        static constexpr int STARS_PER_LAYER = 60;

        void drawStar(Engine::Renderer &renderer, const Star &star,
            float elapsed) const;

        float _width;
        float _worldToScreen;
        std::vector<Star> _stars;
    };
}
