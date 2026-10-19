#pragma once

#include "engine/window/Animation.hpp"
#include "engine/window/Types.hpp"

namespace Engine {
    struct Sprite {
        TextureID texture = 0;
        Animation animation;
    };
}
