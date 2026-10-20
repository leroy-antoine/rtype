#include <array>
#include <cctype>
#include <cstddef>
#include "Font.hpp"

namespace client {
    namespace {
        struct Glyph {
            char symbol;
            std::array<const char *, Font::GLYPH_HEIGHT> rows;
        };

        constexpr float SPACING = 1.f; // -> Colonnes vides entre deux lettres

        constexpr std::array<Glyph, 42> GLYPHS = {{
            {'A', {"01110", "10001", "10001", "11111", "10001", "10001", "10001"}},
            {'B', {"11110", "10001", "10001", "11110", "10001", "10001", "11110"}},
            {'C', {"01110", "10001", "10000", "10000", "10000", "10001", "01110"}},
            {'D', {"11110", "10001", "10001", "10001", "10001", "10001", "11110"}},
            {'E', {"11111", "10000", "10000", "11110", "10000", "10000", "11111"}},
            {'F', {"11111", "10000", "10000", "11110", "10000", "10000", "10000"}},
            {'G', {"01110", "10001", "10000", "10111", "10001", "10001", "01111"}},
            {'H', {"10001", "10001", "10001", "11111", "10001", "10001", "10001"}},
            {'I', {"01110", "00100", "00100", "00100", "00100", "00100", "01110"}},
            {'J', {"00111", "00010", "00010", "00010", "00010", "10010", "01100"}},
            {'K', {"10001", "10010", "10100", "11000", "10100", "10010", "10001"}},
            {'L', {"10000", "10000", "10000", "10000", "10000", "10000", "11111"}},
            {'M', {"10001", "11011", "10101", "10101", "10001", "10001", "10001"}},
            {'N', {"10001", "11001", "10101", "10011", "10001", "10001", "10001"}},
            {'O', {"01110", "10001", "10001", "10001", "10001", "10001", "01110"}},
            {'P', {"11110", "10001", "10001", "11110", "10000", "10000", "10000"}},
            {'Q', {"01110", "10001", "10001", "10001", "10101", "10010", "01101"}},
            {'R', {"11110", "10001", "10001", "11110", "10100", "10010", "10001"}},
            {'S', {"01111", "10000", "10000", "01110", "00001", "00001", "11110"}},
            {'T', {"11111", "00100", "00100", "00100", "00100", "00100", "00100"}},
            {'U', {"10001", "10001", "10001", "10001", "10001", "10001", "01110"}},
            {'V', {"10001", "10001", "10001", "10001", "10001", "01010", "00100"}},
            {'W', {"10001", "10001", "10001", "10101", "10101", "10101", "01010"}},
            {'X', {"10001", "10001", "01010", "00100", "01010", "10001", "10001"}},
            {'Y', {"10001", "10001", "01010", "00100", "00100", "00100", "00100"}},
            {'Z', {"11111", "00001", "00010", "00100", "01000", "10000", "11111"}},
            {'0', {"01110", "10001", "10011", "10101", "11001", "10001", "01110"}},
            {'1', {"00100", "01100", "00100", "00100", "00100", "00100", "01110"}},
            {'2', {"01110", "10001", "00001", "00010", "00100", "01000", "11111"}},
            {'3', {"11110", "00001", "00001", "01110", "00001", "00001", "11110"}},
            {'4', {"00010", "00110", "01010", "10010", "11111", "00010", "00010"}},
            {'5', {"11111", "10000", "11110", "00001", "00001", "10001", "01110"}},
            {'6', {"00110", "01000", "10000", "11110", "10001", "10001", "01110"}},
            {'7', {"11111", "00001", "00010", "00100", "01000", "01000", "01000"}},
            {'8', {"01110", "10001", "10001", "01110", "10001", "10001", "01110"}},
            {'9', {"01110", "10001", "10001", "01111", "00001", "00010", "01100"}},
            {':', {"00000", "00100", "00000", "00000", "00000", "00100", "00000"}},
            {'.', {"00000", "00000", "00000", "00000", "00000", "01100", "01100"}},
            {'-', {"00000", "00000", "00000", "11111", "00000", "00000", "00000"}},
            {'!', {"00100", "00100", "00100", "00100", "00100", "00000", "00100"}},
            {'/', {"00001", "00010", "00010", "00100", "01000", "01000", "10000"}},
        }};

        const Glyph *find(const char symbol)
        {
            const char upper = static_cast<char>(
                std::toupper(static_cast<unsigned char>(symbol)));

            for (const Glyph &glyph : GLYPHS) {
                if (glyph.symbol == upper) {
                    return &glyph;
                }
            }
            return nullptr;
        }
    }

    Font::Font(Engine::Renderer &renderer) : _renderer(renderer)
    {
    }

    float Font::width(const std::string &text, const float scale)
    {
        if (text.empty()) {
            return 0.f;
        }
        const auto letters = static_cast<float>(text.size());

        return (letters * (GLYPH_WIDTH + SPACING) - SPACING) * scale;
    }

    float Font::height(const float scale)
    {
        return GLYPH_HEIGHT * scale;
    }

    void Font::draw(const std::string &text, const Engine::Vector2f position,
        const float scale, const Engine::Color color)
    {
        Engine::Vector2f cursor = position;

        for (const char c : text) {
            drawGlyph(c, cursor, scale, color);
            cursor.x += (GLYPH_WIDTH + SPACING) * scale;
        }
    }

    void Font::drawCentered(const std::string &text, const float centerX,
        const float y, const float scale, const Engine::Color color)
    {
        draw(text, {centerX - width(text, scale) / 2.f, y}, scale, color);
    }

    void Font::drawGlyph(const char c, const Engine::Vector2f position,
        const float scale, const Engine::Color color)
    {
        const Glyph *glyph = find(c);

        if (glyph == nullptr) {
            return;
        }
        for (std::size_t y = 0; y < glyph->rows.size(); ++y) {
            drawRow(glyph->rows[y],
                {position.x, position.y + static_cast<float>(y) * scale},
                scale, color);
        }
    }

    // Une rangee de pixels : les pixels allumes qui se touchent forment un
    // seul rectangle.
    void Font::drawRow(const char *row, const Engine::Vector2f position,
        const float scale, const Engine::Color color)
    {
        int x = 0;

        while (x < GLYPH_WIDTH) {
            if (row[x] != '1') {
                ++x;
                continue;
            }
            const int start = x;

            while (x < GLYPH_WIDTH && row[x] == '1') {
                ++x;
            }
            _renderer.drawRect(
                {position.x + static_cast<float>(start) * scale, position.y},
                {static_cast<float>(x - start) * scale, scale}, color);
        }
    }
}
