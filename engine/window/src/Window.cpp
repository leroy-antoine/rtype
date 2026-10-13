#include <SFML/Graphics.hpp>
#include "engine/window/Window.hpp"

namespace Engine {
    struct Window::Impl {
        Impl(std::uint32_t width, std::uint32_t height, const std::string &title)
        : window(sf::VideoMode({width, height}), title) {}
        sf::RenderWindow window;
    };

    namespace {
        Key toKey(sf::Keyboard::Key key)
        {
            switch (key) {
                case sf::Keyboard::Key::Up: return Key::Up;
                case sf::Keyboard::Key::Down: return Key::Down;
                case sf::Keyboard::Key::Left: return Key::Left;
                case sf::Keyboard::Key::Right: return Key::Right;
                case sf::Keyboard::Key::Space: return Key::Space;
                case sf::Keyboard::Key::Enter: return Key::Enter;
                case sf::Keyboard::Key::Escape: return Key::Escape;
                default: return Key::Unknown;
            }
        }

        std::optional<Event> translate(const sf::Event &event)
        {
            if (event.is<sf::Event::Closed>())
                return Events::Closed{};
            if (event.is<sf::Event::FocusLost>())
                return Events::FocusLost{};
            if (event.is<sf::Event::FocusGained>())
                return Events::FocusGained{};
            if (const auto *e = event.getIf<sf::Event::Resized>())
                return Events::Resized{e->size.x, e->size.y};
            if (const auto *e = event.getIf<sf::Event::KeyPressed>())
                return Events::KeyPressed{toKey(e->code)};
            if (const auto *e = event.getIf<sf::Event::KeyReleased>())
                return Events::KeyReleased{toKey(e->code)};
            return std::nullopt;
        }
    }

    Window::Window(std::uint32_t width, std::uint32_t height, const std::string &title)
        : _impl(std::make_unique<Impl>(width, height, title)) {}

    Window::~Window() = default;

    bool Window::isOpen() const {
        return _impl->window.isOpen();
    }

    void Window::close() {
        _impl->window.close();
    }

    void Window::setFramerateLimit(unsigned int limit) {
        _impl->window.setFramerateLimit(limit);
    }

    void Window::display() {
        _impl->window.display();
    }

    void Window::clear(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
        _impl->window.clear(sf::Color(r, g, b));
    }

    std::optional<Event> Window::pollEvent() {
        while (const std::optional event = _impl->window.pollEvent()) {
            if (std::optional<Event> translated = translate(*event))
                return translated;
        }
        return std::nullopt;
    }
}