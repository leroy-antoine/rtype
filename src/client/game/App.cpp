#include <iostream>
#include <optional>
#include <utility>
#include "engine/window/Renderer.hpp"
#include "engine/window/TextureManager.hpp"
#include "rtype/ReturnCodes.hpp"
#include "App.hpp"
#include "SpriteBank.hpp"

namespace client {
    App::App(std::filesystem::path assets) : _assets(std::move(assets))
    {
    }

    int App::run()
    {
        Engine::Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "R-Type");
        Engine::TextureManager textures;
        Engine::Renderer renderer(window, textures);
        const SpriteBank sprites(textures, _assets);
        GameView view(renderer, sprites, WORLD_TO_SCREEN);
        const std::optional<int> slot = _game.addPlayer();

        if (!slot) {
            std::cerr << "R-Type: no free player slot" << std::endl;
            return RTYPE_ERROR;
        }
        window.setFramerateLimit(FRAMERATE);
        _start = Clock::now();
        _last = _start;
        loop(window, view, *slot);
        return RTYPE_SUCCESS;
    }

    void App::loop(Engine::Window &window, GameView &view, const int slot)
    {
        while (window.isOpen()) {
            _controls.poll(window);
            if (_controls.respawnRequested()) {
                _game.respawnPlayer(slot);
            }
            _game.setInput(slot, _controls.input());
            simulate();
            window.clear(0, 0, 0);
            view.draw(_game.registry(), elapsed());
            window.display();
        }
    }

    void App::simulate()
    {
        const Clock::time_point now = Clock::now();

        _accumulator += std::chrono::duration<float>(now - _last).count();
        _last = now;
        while (_accumulator >= rtype::config::FIXED_DT) {
            _game.update(rtype::config::FIXED_DT);
            _accumulator -= rtype::config::FIXED_DT;
        }
    }

    float App::elapsed() const
    {
        return std::chrono::duration<float>(_last - _start).count();
    }
}
