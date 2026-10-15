#pragma once

#include "engine/window/Types.hpp"
#include "engine/window/Window.hpp"
#include "engine/window/TextureManager.hpp"
#include "engine/window/Export.hpp"

namespace Engine {
    class ENGINE_WINDOW_EXPORT Renderer {
        public:
        Renderer(Window &window, const TextureManager &textures);
        ~Renderer();

        void draw(TextureID id, Vector2f position);
        void draw(TextureID id, Vector2f position, IntRect source, Vector2f scale = {1.f, 1.f});

        private:
        Window &_window;
        const TextureManager &_textures;
    };
}
