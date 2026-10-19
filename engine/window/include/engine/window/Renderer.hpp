#pragma once

#include "engine/window/Sprite.hpp"
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
        void drawResized(TextureID id, Vector2f position, IntRect source, Vector2f size);
        void draw(const Sprite &sprite, Vector2f position, Vector2f size, float elapsed);


        private:
        Window &_window;
        const TextureManager &_textures;
    };
}
