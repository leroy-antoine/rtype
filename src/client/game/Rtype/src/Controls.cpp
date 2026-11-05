#include <optional>
#include <variant>
#include "client/Controls.hpp"

namespace client {
    void Controls::poll(Engine::Window &window)
    {
        _respawn = false;
        _pause = false;
        _menuMove = 0;
        _pressed.clear();
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

    bool Controls::pauseRequested() const
    {
        return _pause;
    }

    int Controls::menuMove() const
    {
        return _menuMove;
    }

    const std::vector<Engine::Key> &Controls::pressedKeys() const
    {
        return _pressed;
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
        if (event.key == Engine::Key::Backspace) {
            _pause = true;
        }
        if (event.key == Engine::Key::Up) {
            --_menuMove;
        }
        if (event.key == Engine::Key::Down) {
            ++_menuMove;
        }
        if (!_held.contains(event.key)) {
            _pressed.push_back(event.key);
            _held.insert(event.key);
        }
        applyKey(event.key, true);
    }

    void Controls::handle(Engine::Window &, const Engine::Events::KeyReleased &event)
    {
        _held.erase(event.key);
        applyKey(event.key, false);
    }

    void Controls::applyKey(const Engine::Key key, const bool pressed)
    {
        switch (key) {
            case Engine::Key::Up:
            case Engine::Key::Z: _input.up = pressed; break;
            case Engine::Key::Down:
            case Engine::Key::S: _input.down = pressed; break;
            case Engine::Key::Left:
            case Engine::Key::Q: _input.left = pressed; break;
            case Engine::Key::Right:
            case Engine::Key::D: _input.right = pressed; break;
            case Engine::Key::Space: _input.fire = pressed; break;
            default: break;
        }
    }
}
