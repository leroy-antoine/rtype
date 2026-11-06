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
        _custom(std::move(customMaps))
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
            handleBack(window);
            const float dt = advance();
            if (const std::optional<rtype::protocol::BuyResult> result = _replica.takeBuyResult()) {
                shop.showResult(*result, elapsed());
            }
            sendControls(dt, shop);
            _replica.update(dt);
            playSounds(audio);
            window.clear(4, 4, 18);
            view.draw(_replica, _custom, elapsed());
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

        if (!inLobby()) {
            _replica.menu().reset();
        }

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

    // Echap revient au choix de la map, ou quitte depuis ailleurs.
    void App::handleBack(Engine::Window &window)
    {
        if (!_controls.backRequested()) {
            return;
        }
        if (inLobby() && _replica.menu().step() != LobbyStep::Map) {
            _replica.menu().back();
            return;
        }
        window.close();
    }

    void App::useMenu()
    {
        const LobbyStep step = _replica.menu().step();

        if (const auto result = _replica.takeUploadResult()) {
            _custom.setMessage(result->error);
        }
        if (!_replica.isHost() && step == LobbyStep::Settings) {
            _replica.menu().back();
        } else if (step == LobbyStep::Settings) {
            useSettingsStep();
        } else if (step == LobbyStep::Scores) {
            useScoresStep();
        } else if (step == LobbyStep::Custom) {
            useCustomStep();
        } else if (step == LobbyStep::CustomScores) {
            useCustomScoresStep();
        } else {
            useMapStep();
        }
    }

    bool App::pressed(const Engine::Key key) const
    {
        const std::vector<Engine::Key> &keys = _controls.pressedKeys();

        return std::ranges::find(keys, key) != keys.end();
    }

    rtype::Settings App::currentSettings() const
    {
        if (_replica.status()) {
            return _replica.status()->settings;
        }
        return rtype::Settings{};
    }

    void App::useMapStep()
    {
        if (pressed(Engine::Key::Tab)) {
            _custom.rescan();
            _replica.menu().openCustom();
            return;
        }
        _replica.moveMapCursor(_controls.menuMove());
        _replica.menu().moveShown(_controls.menuSide());
        if (!_controls.respawnRequested()) {
            return;
        }
        if (_replica.onLeaderboard()) {
            _replica.menu().openScores();
            _network.requestHistory();
        } else if (_replica.isHost()) {
            _replica.menu().openSettings(currentSettings());
        }
    }

    void App::useScoresStep()
    {
        LobbyMenu &menu = _replica.menu();

        menu.moveScoresMap(_controls.menuSide());
        menu.scrollBy(_controls.menuMove(),
            _replica.runs(menu.scoresMap()).size());
        if (_controls.respawnRequested()) {
            menu.back();
        }
    }

    void App::useSettingsStep()
    {
        LobbyMenu &menu = _replica.menu();

        menu.moveRow(_controls.menuMove());
        if (_controls.menuSide() != 0) {
            _network.changeSettings(menu.change(_controls.menuSide()));
        }
        if (!_controls.respawnRequested()) {
            return;
        }
        if (menu.settingsForCustom()) {
            startCustomMap();
        } else {
            _network.selectMap(_replica.mapCursor());
        }
        menu.back();
    }

    // The maps written by the players: the host picks one, chooses the
    // settings, and the map is sent to the server, which checks it again and
    // starts it.
    void App::useCustomStep()
    {
        LobbyMenu &menu = _replica.menu();

        _custom.moveCursor(_controls.menuMove());
        if (pressed(Engine::Key::R)) {
            _custom.rescan();
        }
        if (pressed(Engine::Key::Tab) || _controls.menuSide() > 0) {
            askCustomRecords();
            return;
        }
        if (!_controls.respawnRequested() || !_replica.isHost()) {
            return;
        }
        if (_custom.selected() == nullptr) {
            _custom.setMessage("CHOOSE A MAP WITHOUT A MISTAKE");
            return;
        }
        menu.openSettings(currentSettings(), true);
    }

    // Tab and Right open the leaderboard of a map without a mistake: the
    // server only knows those.
    void App::askCustomRecords()
    {
        const rtype::mapjson::MapEntry *entry = _custom.selected();

        if (entry == nullptr) {
            _custom.setMessage("NO LEADERBOARD FOR A MAP WITH A MISTAKE");
            if (pressed(Engine::Key::Tab)) {
                _replica.menu().reset();
            }
            return;
        }
        _replica.menu().openCustomScores();
        _network.requestCustomRecords(entry->hash);
    }

    void App::useCustomScoresStep()
    {
        LobbyMenu &menu = _replica.menu();

        menu.moveShown(_controls.menuSide());
        if (pressed(Engine::Key::Tab)) {
            menu.reset();
            return;
        }
        _custom.moveCursor(_controls.menuMove());
        if (_controls.menuMove() != 0 && _custom.selected() != nullptr) {
            _network.requestCustomRecords(_custom.selected()->hash);
        }
    }

    void App::startCustomMap()
    {
        std::string text;
        const std::string error = _custom.readSelected(text);

        if (_network.uploading()) {
            return;
        }
        if (!error.empty()) {
            _custom.setMessage(error);
            return;
        }
        _network.uploadMap(std::move(text));
        _custom.setMessage("SENDING THE MAP...");
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
