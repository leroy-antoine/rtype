#pragma once

#include <cstdint>

namespace Engine {
    using TextureID = std::uint32_t;

    struct Vector2f {
        float x = 0.0f;
        float y = 0.0f;
    };

    struct IntRect {
        int left = 0;
        int top = 0;
        int width = 0;
        int height = 0;
    };

}