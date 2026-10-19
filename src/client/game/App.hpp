#pragma once

#include <chrono>
#include <filesystem>
#include "engine/window/Window.hpp"
#include "Config.hpp"
#include "Controls.hpp"
#include "Game.hpp"
#include "GameView.hpp"

namespace client {
    class App {
    public:
        explicit App(std::filesystem::path assets);

        int run();

    private:
        using Clock = std::chrono::steady_clock;

        static constexpr unsigned int WINDOW_WIDTH = 1280;
        static constexpr unsigned int WINDOW_HEIGHT = 720;
        static constexpr unsigned int FRAMERATE = 60;
        static constexpr float WORLD_TO_SCREEN = static_cast<float>(WINDOW_WIDTH) / rtype::config::MAP_WIDTH;

        void loop(Engine::Window &window, GameView &view, int slot);
        void simulate();
        [[nodiscard]] float elapsed() const;

        std::filesystem::path _assets;
        rtype::Game _game;
        Controls _controls;
        Clock::time_point _start;
        Clock::time_point _last;
        float _accumulator = 0.f;
    };
}
