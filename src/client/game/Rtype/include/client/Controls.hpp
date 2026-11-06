#pragma once

#include <set>
#include <vector>
#include "engine/window/Event.hpp"
#include "engine/window/Window.hpp"
#include "logic/Components.hpp"

namespace client {
    class Controls {
    public:
        void poll(Engine::Window &window);

        [[nodiscard]] const rtype::PlayerInput &input() const;
        [[nodiscard]] bool respawnRequested() const;
        [[nodiscard]] bool pauseRequested() const;
        [[nodiscard]] int menuMove() const;
        [[nodiscard]] int menuSide() const;
        [[nodiscard]] bool backRequested() const;
        [[nodiscard]] const std::vector<Engine::Key> &pressedKeys() const;

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
        bool _pause = false;
        int _menuMove = 0;
        int _menuSide = 0;
        bool _back = false;
        std::vector<Engine::Key> _pressed;
        std::set<Engine::Key> _held;
    };
}
