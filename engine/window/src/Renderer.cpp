#include <SFML/Graphics.hpp>
#include "engine/window/Renderer.hpp"
#include "engine/window/WindowImpl.hpp"
#include "engine/window/TextureManagerImpl.hpp"

namespace Engine {

    Renderer::Renderer(Window &window, const TextureManager &textures)
        : _window(window), _textures(textures) {}
    Renderer::~Renderer() = default;

    void Renderer::draw(TextureID id, Vector2f position) {
        sf::Sprite sprite(_textures._impl->textures.at(id));

        sprite.setPosition({position.x, position.y});
        _window._impl->window.draw(sprite);
    }

    void Renderer::draw(TextureID id, Vector2f position, IntRect source, Vector2f scale) {
        sf::Sprite sprite(_textures._impl->textures.at(id),
            sf::IntRect({source.left, source.top}, {source.width, source.height}));
        sprite.setPosition({position.x, position.y});
        sprite.setScale({scale.x, scale.y});
        _window._impl->window.draw(sprite);
    }
    void Renderer::drawResized(TextureID id, Vector2f position, IntRect source, Vector2f size) {
        if (source.width == 0 || source.height == 0)
            return;
        draw(id, position, source, {size.x / static_cast<float>(source.width),
            size.y / static_cast<float>(source.height)});
    }

    void Renderer::draw(const Sprite &sprite, Vector2f position, Vector2f size, float elapsed) {
        drawResized(sprite.texture, position, sprite.animation.frameAt(elapsed), size);
    }
}
