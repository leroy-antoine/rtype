// #include <SFML/Graphics.hpp>
#include "rtype/ReturnCodes.hpp"
#include "engine/window/Window.hpp"
#include "engine/window/TextureManager.hpp"
#include "engine/window/Renderer.hpp"
#include <variant>



static void handleEvents(Engine::Window &window)
{
    while (const std::optional event = window.pollEvent()) {
        if (std::holds_alternative<Engine::Events::Closed>(*event))
            window.close();
    }
}

int main()
{
    Engine::Window window(800, 600, "Rtype client");
    Engine::TextureManager textures;
    Engine::Renderer renderer(window, textures);

    const Engine::TextureID ship = textures.load("sprites/r-typesheet42.gif");

    window.setFramerateLimit(60);
    while (window.isOpen()) {
        handleEvents(window);
        window.clear(0, 0, 0);
        renderer.draw(ship, {100.f, 300.f}, {0, 0, 33, 17}, {3.f, 3.f});
        window.display();
    }
    return RTYPE_SUCCESS;
}

// static void test(sf::RenderWindow &window)
// {
//     while (const std::optional event = window.pollEvent()) {
//         if (event->is<sf::Event::Closed>())
//             window.close();
//     }
//     window.clear(sf::Color(20, 20, 40));
//     window.display();
// }
//
// int main()
// {
//     sf::RenderWindow window(sf::VideoMode({800u, 600u}), std::string("R-Type client"));
//     window.setFramerateLimit(60);
//
//     while (window.isOpen()) {
//         test(window);
//     }
//     return RTYPE_SUCCESS;
// }
