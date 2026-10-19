#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "engine/window/Window.hpp"

namespace Engine {
    struct Window::Impl {
        Impl(std::uint32_t width, std::uint32_t height, const std::string &title)
        : window(sf::VideoMode({width, height}), title) {}
        sf::RenderWindow window;
    };
}
