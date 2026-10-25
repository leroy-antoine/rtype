#pragma once

#include <string>
#include "engine/window/Renderer.hpp"
#include "engine/window/Types.hpp"

namespace client {
    // Police bitmap 5x7 dessinee avec des rectangles : le moteur n'a pas de
    // rendu de texte. Majuscules, chiffres et quelques signes seulement.
    class Font {
    public:
        static constexpr int GLYPH_WIDTH = 5;
        static constexpr int GLYPH_HEIGHT = 7;

        explicit Font(Engine::Renderer &renderer);

        // Largeur en pixels d'un texte dessine a cette echelle.
        [[nodiscard]] static float width(const std::string &text, float scale);
        [[nodiscard]] static float height(float scale);

        void draw(const std::string &text, Engine::Vector2f position,
            float scale, Engine::Color color);
        void drawCentered(const std::string &text, float centerX, float y,
            float scale, Engine::Color color);

    private:
        void drawGlyph(char c, Engine::Vector2f position, float scale,
            Engine::Color color);
        void drawRow(const char *row, Engine::Vector2f position, float scale,
            Engine::Color color);

        Engine::Renderer &_renderer;
    };
}
