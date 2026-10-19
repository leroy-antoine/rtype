#include <chrono>
#include <iostream>
#include <optional>
#include <utility>
#include "engine/window/Renderer.hpp"
#include "engine/window/TextureManager.hpp"
#include "engine/window/Window.hpp"
#include "rtype/ReturnCodes.hpp"
#include "App.hpp"
#include "Config.hpp"
#include "Controls.hpp"
#include "Game.hpp"
#include "GameView.hpp"
#include "SpriteBank.hpp"

namespace client {
    namespace {
        constexpr unsigned int WINDOW_WIDTH = 1280;
        constexpr unsigned int WINDOW_HEIGHT = 720;
        constexpr float WORLD_TO_SCREEN = static_cast<float>(WINDOW_WIDTH) / rtype::config::MAP_WIDTH;
    }

    App::App(std::filesystem::path assets) : _assets(std::move(assets))
    {
    }

    int App::run()
    {
        using Clock = std::chrono::steady_clock;

        Engine::Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "R-Type");
        Engine::TextureManager textures;
        Engine::Renderer renderer(window, textures);
        const SpriteBank sprites(textures, _assets);
        GameView view(renderer, sprites, WORLD_TO_SCREEN);
        Controls controls;
        rtype::Game game;
        const std::optional<int> slot = game.addPlayer();
        const Clock::time_point start = Clock::now();
        Clock::time_point last = start;
        float accumulator = 0.f;

        if (!slot) {
            std::cerr << "R-Type: no free player slot" << std::endl;
            return RTYPE_ERROR;
        }
        window.setFramerateLimit(60);
        while (window.isOpen()) {
            controls.poll(window);
            if (controls.respawnRequested() && !game.isPlayerAlive(*slot)) {
                game.removePlayer(*slot);
                game.addPlayer();
            }
            game.setInput(*slot, controls.input());

            const Clock::time_point now = Clock::now();
            accumulator += std::chrono::duration<float>(now - last).count();
            last = now;
            while (accumulator >= rtype::config::FIXED_DT) {
                game.update(rtype::config::FIXED_DT);
                accumulator -= rtype::config::FIXED_DT;
            }

            window.clear(0, 0, 0);
            view.draw(game.registry(), std::chrono::duration<float>(now - start).count());
            window.display();
        }
        return RTYPE_SUCCESS;
    }
}
