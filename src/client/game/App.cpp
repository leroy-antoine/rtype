#include <utility>
#include "engine/window/Renderer.hpp"
#include "engine/window/TextureManager.hpp"
#include "rtype/ReturnCodes.hpp"
#include "App.hpp"
#include "SpriteBank.hpp"

namespace client {
    App::App(std::filesystem::path assets, const std::string &host, const std::uint16_t port)
        : _assets(std::move(assets)), _network(host, port)
    {
    }

    int App::run()
    {
        Engine::Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "R-Type");
        Engine::TextureManager textures;
        Engine::Renderer renderer(window, textures);
        const SpriteBank sprites(textures, _assets);
        GameView view(renderer, sprites, WORLD_TO_SCREEN);

        window.setFramerateLimit(FRAMERATE);
        _start = Clock::now();
        _last = _start;
        if (!loop(window, view)) {
            return RTYPE_ERROR;
        }
        return RTYPE_SUCCESS;
    }

    // Renvoie false si la boucle s'arrete parce que le serveur est perdu.
    bool App::loop(Engine::Window &window, GameView &view)
    {
        while (window.isOpen()) {
            if (!_network.update(_replica)) {
                return false;
            }
            _controls.poll(window);
            sendControls();
            _replica.update(advance());
            window.clear(0, 0, 0);
            view.draw(_replica.registry(), elapsed());
            window.display();
        }
        return true;
    }

    void App::sendControls()
    {
        if (_controls.respawnRequested()) {
            _network.requestRespawn();
        }
        _network.sendInput(_controls.input());
        _network.flush();
    }

    float App::advance()
    {
        const Clock::time_point now = Clock::now();
        const float dt = std::chrono::duration<float>(now - _last).count();

        _last = now;
        return dt;
    }

    float App::elapsed() const
    {
        return std::chrono::duration<float>(_last - _start).count();
    }
}
