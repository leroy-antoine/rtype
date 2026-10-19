#include <exception>
#include <filesystem>
#include <iostream>
#include "rtype/ReturnCodes.hpp"
#include "game/App.hpp"

int main(const int, char **argv)
{
    try {
        client::App app(std::filesystem::path(argv[0]).parent_path() / "assets");

        return app.run();
    } catch (const std::exception &e) {
        std::cerr << "R-Type: " << e.what() << std::endl;
        return RTYPE_ERROR;
    }
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
