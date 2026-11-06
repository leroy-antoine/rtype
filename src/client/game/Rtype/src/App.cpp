#include <algorithm>
#include <utility>
#include <vector>
#include "engine/window/Renderer.hpp"
#include "engine/window/TextureManager.hpp"
#include "rtype/ReturnCodes.hpp"
#include "client/App.hpp"
#include "client/SpriteBank.hpp"

namespace client {
    App::App(std::filesystem::path assets, std::filesystem::path customMaps,
        const std::string &host, const std::uint16_t port)
        : _assets(std::move(assets)), _network(host, port),
        _menu(std::move(customMaps))
    {
    }

    int App::run()
    {
        Engine::Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "R-Type");
        Engine::TextureManager textures;
        Engine::Renderer renderer(window, textures);
        const SpriteBank sprites(textures, _assets);
        GameView view(renderer, sprites, WORLD_TO_SCREEN, WINDOW_WIDTH, WINDOW_HEIGHT);
        Audio audio(_assets);
        ShopPanel shop(renderer, sprites, WINDOW_WIDTH, WINDOW_HEIGHT);

        window.setFramerateLimit(FRAMERATE);
        _start = Clock::now();
        _last = _start;
        if (!loop(window, view, audio, shop)) {
            return RTYPE_ERROR;
        }
        return RTYPE_SUCCESS;
    }

    bool App::loop(Engine::Window &window, GameView &view, Audio &audio, ShopPanel &shop)
    {
        while (window.isOpen()) {
            if (!_network.update(_replica)) {
                return false;
            }
            _controls.poll(window);
            const float dt = advance();
            if (const std::optional<rtype::protocol::BuyResult> result = _replica.takeBuyResult()) {
                shop.showResult(*result, elapsed());
            }
            sendControls(dt, shop);
            _replica.update(dt);
            playSounds(audio);
            window.clear(4, 4, 18);
            view.draw(_replica, _menu, elapsed());
            shop.draw(_replica, elapsed());
            window.display();
        }
        return true;
    }

    void App::playSounds(Audio &audio)
    {
        const std::vector<Cue> cues = _replica.takeCues();

        if (listening()) {
            audio.play(cues);
        }
        audio.follow(_replica);
    }

    bool App::listening()
    {
        if (!_network.connected()) {
            _connectedSince.reset();
            return false;
        }
        if (!_connectedSince) {
            _connectedSince = elapsed();
        }
        return elapsed() - *_connectedSince >= LISTEN_DELAY;
    }

    void App::sendControls(const float dt, ShopPanel &shop)
    {
        sendReady();
        rtype::PlayerInput input = _controls.input();

        if (ShopPanel::isOpen(_replica)) {
            useShop(shop);
            input = {};
        } else if (inLobby()) {
            useMenu();
        } else if (isPaused()) {
            usePause();
            input = {};
        } else {
            useMatch();
        }
        _network.sendInput(_replica.predict(input, dt), input);
        _network.flush();
    }

    void App::useShop(ShopPanel &shop)
    {
        const std::vector<Engine::Key> &keys = _controls.pressedKeys();

        if (const std::optional<rtype::AugmentId> id =
            shop.handle(keys, _replica, elapsed())) {
            _network.buy(*id);
        }
    }

    bool App::isPaused() const
    {
        const auto &status = _replica.status();

        return status && status->paused;
    }

    void App::useMatch()
    {
        if (_controls.pauseRequested() && _replica.isHost()) {
            _network.setPause(true);
        } else if (_controls.respawnRequested()) {
            _network.requestRespawn();
        }
    }

    void App::usePause()
    {
        if (!_replica.isHost()) {
            return;
        }
        if (_controls.pauseRequested()) {
            _network.setPause(false);
        } else if (_controls.respawnRequested()) {
            _network.leaveMatch();
        }
    }

    bool App::inLobby() const
    {
        const auto &status = _replica.status();

        return status && status->phase == rtype::Phase::Lobby;
    }

    void App::useMenu()
    {
        if (const auto result = _replica.takeUploadResult()) {
            _menu.setMessage(result->error);
        }
        _menu.navigate(_controls.pressedKeys());
        if (_menu.tab() != Tab::Maps) {
            useCustomMenu();
            return;
        }
        _replica.moveMapCursor(_controls.menuMove());
        if (_controls.respawnRequested() && _replica.isHost()) {
            _network.selectMap(_replica.mapCursor());
        }
    }

    // The maps written by the players: the host picks one, it is sent to the
    // server, which checks it again and starts it.
    void App::useCustomMenu()
    {
        _menu.moveCursor(_controls.menuMove());
        if (const auto hash = _menu.takeRecordsRequest()) {
            _network.requestCustomRecords(*hash);
        }
        if (_menu.tab() == Tab::Custom && _controls.respawnRequested()
            && _replica.isHost()) {
            startCustomMap();
        }
    }

    void App::startCustomMap()
    {
        std::string text;

        if (_network.uploading()) {
            return;
        }
        const std::string error = _menu.readSelected(text);

        if (!error.empty()) {
            _menu.setMessage(error);
            return;
        }
        _network.uploadMap(std::move(text));
        _menu.setMessage("SENDING THE MAP...");
    }

    void App::sendReady()
    {
        const std::optional<rtype::GameStatus> &status = _replica.status();
        const bool space = std::ranges::find(_controls.pressedKeys(),
            Engine::Key::Space) != _controls.pressedKeys().end();

        if (!status || status->phase != rtype::Phase::Shop || !space) {
            return;
        }
        _network.requestReady(!isReady(*status));
    }

    bool App::isReady(const rtype::GameStatus &status) const
    {
        const std::optional<int> self = _replica.slot();

        return std::ranges::any_of(status.players,
            [&self](const rtype::PlayerStatus &player) {
                return self && player.slot == *self && player.ready;
            });
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
