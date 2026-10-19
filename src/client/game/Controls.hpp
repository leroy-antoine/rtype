#pragma once

#include "engine/window/Event.hpp"
#include "engine/window/Window.hpp"
#include "Components.hpp"

namespace client {
    class Controls {
    public:
        void poll(Engine::Window &window);

        [[nodiscard]] const rtype::PlayerInput &input() const;
        [[nodiscard]] bool respawnRequested() const;

    private:
        void handle(Engine::Window &window, const Engine::Events::Closed &event);
        void handle(Engine::Window &window, const Engine::Events::KeyPressed &event);
        void handle(Engine::Window &window, const Engine::Events::KeyReleased &event);

        template <typename Event>
        void handle(Engine::Window &, const Event &)
        {
        }

        void applyKey(Engine::Key key, bool pressed);

        rtype::PlayerInput _input;
        bool _respawn = false;
    };
}
