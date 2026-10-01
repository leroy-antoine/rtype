#include <SFML/Graphics.hpp>
#include "rtype/ReturnCodes.hpp"

static void test(sf::RenderWindow &window)
{
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
    window.clear(sf::Color(20, 20, 40));
    window.display();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), std::string("R-Type client"));
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        test(window);
    }
    return RTYPE_SUCCESS;
}
