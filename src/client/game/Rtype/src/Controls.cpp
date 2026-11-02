#include <optional>
#include <variant>
#include "client/Controls.hpp"

namespace client {
    void Controls::poll(Engine::Window &window)
    {
        _respawn = false;
        _menuMove = 0;
        while (const std::optional event = window.pollEvent()) {
            std::visit([this, &window](const auto &e) { handle(window, e); }, *event);
        }
    }

    const rtype::PlayerInput &Controls::input() const
    {
        return _input;
    }

    bool Controls::respawnRequested() const
    {
        return _respawn;
    }

    int Controls::menuMove() const
    {
        return _menuMove;
    }

    void Controls::handle(Engine::Window &window, const Engine::Events::Closed &)
    {
        window.close();
    }

    void Controls::handle(Engine::Window &window, const Engine::Events::KeyPressed &event)
    {
        if (event.key == Engine::Key::Escape) {
            window.close();
        }
        if (event.key == Engine::Key::Enter) {
            _respawn = true;
        }
        if (event.key == Engine::Key::Up) {
            --_menuMove;
        }
        if (event.key == Engine::Key::Down) {
            ++_menuMove;
        }
        applyKey(event.key, true);
    }

    void Controls::handle(Engine::Window &, const Engine::Events::KeyReleased &event)
    {
        applyKey(event.key, false);
    }

    void Controls::applyKey(const Engine::Key key, const bool pressed)
    {
        switch (key) {
            case Engine::Key::Up: _input.up = pressed; break;
            case Engine::Key::Down: _input.down = pressed; break;
            case Engine::Key::Left: _input.left = pressed; break;
            case Engine::Key::Right: _input.right = pressed; break;
            case Engine::Key::Space: _input.fire = pressed; break;
            default: break;
        }
    }
}
