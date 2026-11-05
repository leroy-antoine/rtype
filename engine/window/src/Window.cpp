#include <SFML/Graphics.hpp>
#include "engine/window/Window.hpp"
#include "engine/window/WindowImpl.hpp"

namespace Engine {
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
                case sf::Keyboard::Key::Backspace: return Key::Backspace;
                case sf::Keyboard::Key::Tab: return Key::Tab;
                case sf::Keyboard::Key::A: return Key::A;
                case sf::Keyboard::Key::Z: return Key::Z;
                case sf::Keyboard::Key::E: return Key::E;
                case sf::Keyboard::Key::R: return Key::R;
                case sf::Keyboard::Key::Q: return Key::Q;
                case sf::Keyboard::Key::S: return Key::S;
                case sf::Keyboard::Key::D: return Key::D;
                case sf::Keyboard::Key::W: return Key::W;
                case sf::Keyboard::Key::X: return Key::X;
                case sf::Keyboard::Key::C: return Key::C;
                default: return Key::Unknown;
            }
        }

        std::optional<Event> convert(const sf::Event::Closed &) { return Events::Closed{}; }
        std::optional<Event> convert(const sf::Event::FocusLost &) { return Events::FocusLost{}; }
        std::optional<Event> convert(const sf::Event::FocusGained &) { return Events::FocusGained{}; }
        std::optional<Event> convert(const sf::Event::Resized &e) { return Events::Resized{e.size.x, e.size.y}; }
        std::optional<Event> convert(const sf::Event::KeyPressed &e) { return Events::KeyPressed{toKey(e.code)}; }
        std::optional<Event> convert(const sf::Event::KeyReleased &e) { return Events::KeyReleased{toKey(e.code)}; }

        // pour tous les events SFML qu'on gere pas
        template <typename T>
        std::optional<Event> convert(const T &) { return std::nullopt; }

        std::optional<Event> translate(const sf::Event &event)
        {
            return event.visit([](const auto &e) { return convert(e); });
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