#pragma once

#include "engine/window/Window.hpp"
#include "Components.hpp"

namespace client {
    class Controls {
    public:
        void poll(Engine::Window &window);

        [[nodiscard]] const rtype::PlayerInput &input() const;
        [[nodiscard]] bool respawnRequested() const;

    private:
        void applyKey(Engine::Key key, bool pressed);

        rtype::PlayerInput _input;
        bool _respawn = false;
    };
}
