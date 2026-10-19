#include <optional>
#include <variant>
#include "Controls.hpp"

namespace client {
    void Controls::poll(Engine::Window &window)
    {
        _respawn = false;
        while (const std::optional event = window.pollEvent()) {
            if (std::holds_alternative<Engine::Events::Closed>(*event)) {
                window.close();
            } else if (const auto *pressed = std::get_if<Engine::Events::KeyPressed>(&*event)) {
                if (pressed->key == Engine::Key::Escape) {
                    window.close();
                }
                _respawn = _respawn || pressed->key == Engine::Key::Enter;
                applyKey(pressed->key, true);
            } else if (const auto *released = std::get_if<Engine::Events::KeyReleased>(&*event)) {
                applyKey(released->key, false);
            }
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
