#pragma once

#include <cstdint>

namespace Engine {
    using TextureID = std::uint32_t;

    struct Vector2f {
        float x = 0.0f;
        float y = 0.0f;
    };

    struct Color {
        std::uint8_t r = 255;
        std::uint8_t g = 255;
        std::uint8_t b = 255;
        std::uint8_t a = 255;
    };

    struct IntRect {
        int left = 0;
        int top = 0;
        int width = 0;
        int height = 0;
    };

}