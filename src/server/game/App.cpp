#include <csignal>
#include <cstddef>
#include <iostream>
#include <random>
#include <thread>
#include <utility>
#include <variant>
#include <vector>
#include "logic/Builder.hpp"
#include "rtype/Protocol.hpp"
#include "rtype/ReturnCodes.hpp"
#include "App.hpp"

namespace server {
    namespace {
        volatile std::sig_atomic_t running = 1;

        void stop(int)
        {
            running = 0;
        }
    }

    App::App(const std::uint16_t port)
        : _server(port, rtype::config::MAX_PLAYERS, rtype::protocol::VERSION),
        _game(std::random_device{}())
    {
        _game.openLobby();
        _leaderboard.load(LEADERBOARD_FILE);
    }

    int App::run()
    {
        Clock::time_point last = Clock::now();

        std::signal(SIGINT, stop);
        std::signal(SIGTERM, stop);
        std::cout << "R-Type server listening on port " << _server.port() << std::endl;
        while (running) {
            const Clock::time_point now = Clock::now();
            const std::size_t steps = _timestep.advance(std::chrono::duration<float>(now - last).count());

            last = now;
            for (std::size_t i = 0; i < steps; ++i) {
                tick();
            }
            sleep();
        }
        return RTYPE_SUCCESS;
    }

    void App::tick()
    {
        receive();
        _game.update(_timestep.step());
        recordVictory();
        broadcastEvents();
        broadcastStates();
        sendOwnStates();
        if (_ticks++ % STATUS_EVERY_TICKS == 0) {
            broadcastStatus();
        }
        syncAugments();
        syncShop();
        _server.flush();
    }

    void App::recordVictory()
    {
        const rtype::Phase phase = _game.phase();
        const bool won = phase == rtype::Phase::Victory
            && _lastPhase != rtype::Phase::Victory;

        _lastPhase = phase;
        if (!won) {
            return;
        }
        const rtype::GameStatus status = _game.status();

        _leaderboard.add(_game.map(), {status.elapsedMs,
            static_cast<std::uint8_t>(status.players.size())});
        _leaderboard.save(LEADERBOARD_FILE);
        _server.broadcast(rtype::protocol::LEADERBOARD,
            rtype::protocol::encode(_leaderboard.all()),
            Engine::Delivery::Reliable);
    }

    void App::receive()
    {
        _server.update();
        while (const std::optional<Engine::ServerEvent> event = _server.pollEvent()) {
            std::visit([this](const auto &e) { handle(e); }, *event);
        }
    }

    void App::broadcastEvents()
    {
        for (const rtype::GameEvent &event : _game.takeEvents()) {
            std::visit([this](const auto &e) {
                _server.broadcast(rtype::protocol::typeOf(e), rtype::protocol::encode(e), Engine::Delivery::Reliable);
            }, event);
        }
    }

    void App::broadcastStates()
    {
        const auto tick = static_cast<std::uint32_t>(_ticks);

        for (const rtype::protocol::Payload &payload : rtype::protocol::encodeStates(tick, _game.states())) {
            _server.broadcast(rtype::protocol::STATES, payload);
        }
    }

    void App::sendOwnStates()
    {
        for (const auto &[client, slot] : _slots) {
            const std::optional<Engine::Entity> player = rtype::findPlayerBySlot(_game.registry(), slot);
            const auto input = _lastInputs.find(client);

            if (!player || input == _lastInputs.end()) {
                continue;
            }
            const rtype::PlayerEffects &effects =
                _game.registry().get<rtype::PlayerEffects>(*player);
            const rtype::protocol::OwnState state{input->second,
                _game.registry().get<gameLogic::Vec2Pos>(*player),
                effects.inverted, static_cast<std::uint8_t>(
                    effects.speedFactor * rtype::config::SPEED_PERCENT)};

            _server.send(client, rtype::protocol::OWN_STATE, rtype::protocol::encode(state));
        }
    }

    void App::broadcastStatus()
    {
        _server.broadcast(rtype::protocol::STATUS, rtype::protocol::encode(_game.status()));
    }

    void App::sendWorld(const Engine::ClientId client)
    {
        for (const rtype::EntitySpawned &spawn : _game.spawns()) {
            _server.send(client, rtype::protocol::SPAWN, rtype::protocol::encode(spawn), Engine::Delivery::Reliable);
        }
        _server.send(client, rtype::protocol::AUGMENTS, rtype::protocol::encodeAugments(ownedAugments()),
            Engine::Delivery::Reliable);
        _server.send(client, rtype::protocol::SHOP, rtype::protocol::encodeShop(_game.shopOffers()),
            Engine::Delivery::Reliable);
    }

    void App::sleep() const
    {
        const float remaining = _timestep.step() * (1.f - _timestep.alpha());

        std::this_thread::sleep_for(std::chrono::duration<float>(remaining));
    }

    std::optional<int> App::slotOf(const Engine::ClientId client) const
    {
        const auto found = _slots.find(client);

        if (found == _slots.end()) {
            return std::nullopt;
        }
        return found->second;
    }

    void App::handle(const Engine::ServerEvents::Connected &event)
    {
        const std::optional<int> slot = _game.addPlayer();

        if (!slot) {
            _server.disconnect(event.client);
            return;
        }
        _slots[event.client] = *slot;
        _server.send(event.client, rtype::protocol::YOU,
            rtype::protocol::encodeYou(*slot), Engine::Delivery::Reliable);
        sendWorld(event.client);
        _server.send(event.client, rtype::protocol::LEADERBOARD,
            rtype::protocol::encode(_leaderboard.all()),
            Engine::Delivery::Reliable);
        std::cout << "client " << event.client << " joined in slot " << *slot << std::endl;
    }

    void App::handle(const Engine::ServerEvents::Disconnected &event)
    {
        const std::optional<int> slot = slotOf(event.client);

        if (!slot) {
            return;
        }
        _game.removePlayer(*slot);
        _slots.erase(event.client);
        _lastInputs.erase(event.client);
        std::cout << "client " << event.client << " left (reason " << static_cast<int>(event.reason) << ")" << std::endl;
    }

    void App::handle(const Engine::ServerEvents::Message &event)
    {
        const std::optional<int> slot = slotOf(event.client);

        if (!slot) {
            return;
        }
        if (event.type == rtype::protocol::INPUT) {
            if (const std::optional<rtype::protocol::InputMessage> input = rtype::protocol::decodeInput(event.payload)) {
                _game.setInput(*slot, input->input);
                _lastInputs[event.client] = input->sequence;
            }
            return;
        }
        if (event.type == rtype::protocol::READY) {
            if (const std::optional<bool> ready = rtype::protocol::decodeReady(event.payload)) {
                _game.setReady(*slot, *ready);
            }
            return;
        }
        if (event.type == rtype::protocol::RESPAWN) {
            _game.respawnPlayer(*slot);
            return;
        }
        if (event.type == rtype::protocol::SELECT_MAP) {
            selectMap(*slot, event.payload);
        }
        if (event.type == rtype::protocol::LEAVE_MATCH) {
            leaveMatch(*slot);
        }
        if (event.type == rtype::protocol::PAUSE) {
            pauseMatch(*slot, event.payload);
        }
        if (event.type == rtype::protocol::BUY) {
            handleBuy(event.client, *slot, event.payload);
        }
    }

    void App::leaveMatch(const int slot)
    {
        if (_game.host() == slot) {
            _game.openLobby();
        }
    }

    void App::pauseMatch(const int slot,
        const rtype::protocol::Payload &payload)
    {
        const std::optional<bool> paused = rtype::protocol::decodePause(payload);

        if (paused && _game.host() == slot) {
            _game.setPaused(*paused);
        }
    }

    void App::selectMap(const int slot, const rtype::protocol::Payload &payload)
    {
        const std::optional<rtype::MapId> map =
            rtype::protocol::decodeSelectMap(payload);

        if (!map || _game.host() != slot) {
            return;
        }
        if (std::optional<rtype::Segments> segments = rtype::buildMap(*map)) {
            _game.startMap(std::move(*segments), *map);
        }
    }

    void App::handleBuy(const Engine::ClientId client, const int slot, const rtype::protocol::Payload &payload)
    {
        const std::optional<rtype::AugmentId> id = rtype::protocol::decodeBuy(payload);

        if (!id) {
            return;
        }
        const rtype::protocol::BuyResult result{*id, _game.buyAugment(slot, *id)};

        _server.send(client, rtype::protocol::BUY_RESULT, rtype::protocol::encode(result),
            Engine::Delivery::Reliable);
    }

    rtype::protocol::OwnedAugment App::ownedAugments() const
    {
        rtype::protocol::OwnedAugment owned{};

        for (std::size_t slot = 0; slot < owned.size(); ++slot) {
            owned[slot] = _game.ownedAugments(static_cast<int>(slot));
        }
        return owned;
    }

    void App::syncAugments()
    {
        rtype::protocol::OwnedAugment owned = ownedAugments();

        if (owned != _sentAugments) {
            _server.broadcast(rtype::protocol::AUGMENTS, rtype::protocol::encodeAugments(owned),
                Engine::Delivery::Reliable);
            _sentAugments = std::move(owned);
        }
    }

    void App::syncShop()
    {
        if (_game.shopOffers() != _sentOffers) {
            _server.broadcast(rtype::protocol::SHOP, rtype::protocol::encodeShop(_game.shopOffers()),
                Engine::Delivery::Reliable);
            _sentOffers = _game.shopOffers();
        }
    }
}
