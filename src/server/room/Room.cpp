#include <iostream>
#include <random>
#include <utility>
#include <variant>
#include "logic/Builder.hpp"
#include "Room.hpp"

namespace server {

    Room::Room(const rtype::protocol::RoomId id, Engine::Server &server, Records &records)
        : _id(id), _server(server), _records(records), _game(std::random_device{}()),
        _custom(records.custom, Records::CUSTOM_FILE)
    {
        _game.openLobby();
        _thread = std::jthread([this](const std::stop_token &stop) {run(stop);});
    }

    void Room::post(Engine::ServerEvent event)
    {
        const std::lock_guard lock(_inboxMutex);
        _inbox.push_back(std::move(event));
    }

    rtype::protocol::RoomId Room::id() const
    {
        return _id;
    }

    rtype::Phase Room::phase() const
    {
        return _phase.load();
    }

    void Room::run(const std::stop_token &stop)
    {
        Clock::time_point last = Clock::now();

        while (!stop.stop_requested()) {
            const Clock::time_point now = Clock::now();
            const std::size_t steps = _timestep.advance(std::chrono::duration<float>(now - last).count());

            last = now;
            for (std::size_t i = 0; i < steps; ++i) {
                tick();
            }
            sleep();
        }
    }

    void Room::tick()
    {
        receive();
        _game.update(_timestep.step());
        _phase.store(_game.phase());
        recordVictory();
        if (_ticks % SEND_EVERY_TICKS == 0) {
            broadcastEvents();
            broadcastStates();
            sendOwnStates();
        }
        if (_ticks++ % STATUS_EVERY_TICKS == 0) {
            broadcastStatus();
        }
        syncAugments();
        syncShop();
        _server.flush();
    }

    void Room::receive()
    {
        std::vector<Engine::ServerEvent> events;

        {
            const std::lock_guard lock(_inboxMutex);
            events.swap(_inbox);
        }
        for (const Engine::ServerEvent &event : events) {
            std::visit([this](const auto &e) {handle(e);}, event);
        }
    }

    void Room::sendToMembers(const Engine::MessageType type, const rtype::protocol::Payload &payload,
       const Engine::Delivery delivery)
    {
        for (const auto &[client, slot] : _slots) {
            _server.send(client, type, payload, delivery);
        }
    }

    void Room::recordVictory()
    {
        const rtype::Phase phase = _game.phase();
        const bool won = phase == rtype::Phase::Victory
            && _lastPhase != rtype::Phase::Victory;

        _lastPhase = phase;
        if (!won) {
            return;
        }
        const rtype::GameStatus status = _game.status();
        const rtype::RunRecord record{status.elapsedMs,
            static_cast<std::uint8_t>(status.players.size()),
            status.settings};

        if (_game.map() == rtype::CUSTOM_MAP) {
            recordCustomVictory(record);
            return;
        }
        {
            const std::lock_guard lock(_records.mutex);

            _records.board.add(_game.map(), record);
            _records.board.save(Records::FILE);
        }
        sendLeaderboard(std::nullopt);
    }

    void Room::recordCustomVictory(const rtype::RunRecord &record)
    {
        rtype::protocol::Payload payload;

        {
            const std::lock_guard lock(_records.mutex);
            const std::uint64_t hash = _custom.recordVictory(record);

            payload = rtype::protocol::encode(_custom.recordsOf(hash));
        }
        sendToMembers(rtype::protocol::CUSTOM_RECORDS, payload, Engine::Delivery::Reliable);
    }

    void Room::sendHistory(const Engine::ClientId client)
    {
        std::vector<rtype::protocol::Payload> pages;

        {
            const std::lock_guard lock(_records.mutex);

            pages = rtype::protocol::historyPages(_records.board.history());
        }
        for (const rtype::protocol::Payload &page : pages) {
            _server.send(client, rtype::protocol::HISTORY, page, Engine::Delivery::Reliable);
        }
    }

    void Room::changeSettings(const int slot, const rtype::protocol::Payload &payload)
    {
        const std::optional<rtype::Settings> settings = rtype::protocol::decodeSettings(payload);

        if (settings && _game.host() == slot && _game.phase() == rtype::Phase::Lobby) {
            _game.setSettings(*settings);
        }
    }

    void Room::sendLeaderboard(const std::optional<Engine::ClientId> client)
    {
        rtype::protocol::Payload payload;

        {
            const std::lock_guard lock(_records.mutex);
            payload = rtype::protocol::encode(_records.board.all());
        }
        if (client) {
            _server.send(*client, rtype::protocol::LEADERBOARD, payload, Engine::Delivery::Reliable);
        } else {
            sendToMembers(rtype::protocol::LEADERBOARD, payload, Engine::Delivery::Reliable);
        }
    }

    void Room::broadcastEvents()
    {
        for (const rtype::GameEvent &event : _game.takeEvents()) {
            std::visit([this](const auto &e) {
                sendToMembers(rtype::protocol::typeOf(e), rtype::protocol::encode(e), Engine::Delivery::Reliable);
            }, event);
        }

    }

    void Room::broadcastStates()
    {
        const auto tick = static_cast<std::uint32_t>(_ticks);

        for (const rtype::protocol::Payload &payload : rtype::protocol::encodeStates(tick, _game.states())) {
            sendToMembers(rtype::protocol::STATES, payload);
        }
    }

    void Room::sendOwnStates()
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

    void Room::broadcastStatus()
    {
        sendToMembers(rtype::protocol::STATUS, rtype::protocol::encode(_game.status()));
    }

    void Room::sendWorld(const Engine::ClientId client)
    {
        for (const rtype::EntitySpawned &spawn : _game.spawns()) {
            _server.send(client, rtype::protocol::SPAWN, rtype::protocol::encode(spawn), Engine::Delivery::Reliable);
        }
        _server.send(client, rtype::protocol::AUGMENTS, rtype::protocol::encodeAugments(ownedAugments()),
            Engine::Delivery::Reliable);
        _server.send(client, rtype::protocol::SHOP, rtype::protocol::encodeShop(_game.shopOffers()),
            Engine::Delivery::Reliable);
        if (_game.map() == rtype::CUSTOM_MAP && _custom.selected()) {
            _server.send(client, rtype::protocol::CUSTOM_INFO, rtype::protocol::encode(_custom.info()),
                Engine::Delivery::Reliable);
        }
    }

    void Room::sleep() const
    {
        const float remaining = _timestep.step() * (1.f - _timestep.alpha());
        std::this_thread::sleep_for(std::chrono::duration<float>(remaining));
    }

    std::optional<int> Room::slotOf(const Engine::ClientId client) const
    {
        const auto found = _slots.find(client);

        if (found == _slots.end()) {
            return std::nullopt;
        }
        return found->second;
    }

    void Room::handle(const Engine::ServerEvents::Connected &event)
    {
        const std::optional<int> slot = _game.addPlayer();

        if (!slot) {
            return;
        }
        _slots[event.client] = *slot;
        _server.send(event.client, rtype::protocol::YOU,
            rtype::protocol::encodeYou(*slot), Engine::Delivery::Reliable);
        sendWorld(event.client);
        sendLeaderboard(event.client);
        std::cout << "room " << _id << ": client " << event.client << " joined in slot " << *slot << std::endl;
    }

    void Room::handle(const Engine::ServerEvents::Disconnected &event)
    {
        const std::optional<int> slot = slotOf(event.client);

        if (!slot) {
            return;
        }
        _custom.cancel(*slot);
        _game.removePlayer(*slot);
        _slots.erase(event.client);
        _lastInputs.erase(event.client);
        std::cout << "room " << _id << ": client " << event.client << " left" << std::endl;
    }

    void Room::handle(const Engine::ServerEvents::Message &event)
    {
        const std::optional<int> slot = slotOf(event.client);

        if (!slot) {
            return;
        }
        if (handleCustom(event, *slot)) {
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
        if (event.type == rtype::protocol::HISTORY_REQUEST) {
            sendHistory(event.client);
            return;
        }
        if (event.type == rtype::protocol::SETTINGS) {
            changeSettings(*slot, event.payload);
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

    void Room::leaveMatch(const int slot)
    {
        if (_game.host() == slot) {
            _game.openLobby();
        }
    }

    void Room::pauseMatch(const int slot, const rtype::protocol::Payload &payload)
    {
        const std::optional<bool> paused = rtype::protocol::decodePause(payload);

        if (paused && _game.host() == slot) {
            _game.setPaused(*paused);
        }
    }

    void Room::selectMap(const int slot, const rtype::protocol::Payload &payload)
    {
        const std::optional<rtype::MapId> map = rtype::protocol::decodeSelectMap(payload);

        if (!map || _game.host() != slot) {
            return;
        }
        if (std::optional<rtype::Segments> segments = rtype::buildMap(*map)) {
            _game.startMap(std::move(*segments), *map);
        }
    }

    void Room::handleBuy(const Engine::ClientId client, const int slot, const rtype::protocol::Payload &payload)
    {
        const std::optional<rtype::AugmentId> id = rtype::protocol::decodeBuy(payload);

        if (!id) {
            return;
        }
        const rtype::protocol::BuyResult result{*id, _game.buyAugment(slot, *id)};

        _server.send(client, rtype::protocol::BUY_RESULT, rtype::protocol::encode(result),
            Engine::Delivery::Reliable);
    }

    rtype::protocol::OwnedAugment Room::ownedAugments() const
    {
        rtype::protocol::OwnedAugment owned{};

        for (std::size_t slot = 0; slot < owned.size(); ++slot) {
            owned[slot] = _game.ownedAugments(static_cast<int>(slot));
        }
        return owned;
    }

    void Room::syncAugments()
    {
        rtype::protocol::OwnedAugment owned = ownedAugments();

        if (owned != _sentAugments) {
            sendToMembers(rtype::protocol::AUGMENTS, rtype::protocol::encodeAugments(owned),
                Engine::Delivery::Reliable);
            _sentAugments = std::move(owned);
        }
    }

    void Room::syncShop()
    {
        if (_game.shopOffers() != _sentOffers) {
            sendToMembers(rtype::protocol::SHOP, rtype::protocol::encodeShop(_game.shopOffers()),
                Engine::Delivery::Reliable);
            _sentOffers = _game.shopOffers();
        }
    }


    bool Room::handleCustom(const Engine::ServerEvents::Message &event, const int slot)
    {
        switch (event.type) {
            case rtype::protocol::MAP_UPLOAD_BEGIN:
                uploadBegin(event.client, slot, event.payload);
                return true;
            case rtype::protocol::MAP_UPLOAD_CHUNK:
                if (_game.host() == slot) {
                    _custom.add(slot, event.payload);
                }
                return true;
            case rtype::protocol::MAP_UPLOAD_END:
                uploadEnd(event.client, slot);
                return true;
            case rtype::protocol::CUSTOM_RECORDS_GET:
                sendCustomRecords(event.client, event.payload);
                return true;
            default:
                return false;
        }
    }

    void Room::uploadBegin(const Engine::ClientId client, const int slot, const rtype::protocol::Payload &payload)
    {
        if (_game.host() != slot || _game.phase() != rtype::Phase::Lobby) {
            return;
        }
        if (!_custom.begin(slot, payload)) {
            sendUploadResult(client, {false, "the map is refused"});
        }
    }

    void Room::uploadEnd(const Engine::ClientId client, const int slot)
    {
        if (_game.host() != slot) {
            _custom.cancel(slot);
            return;
        }
        std::variant<LoadedMap, std::string> outcome = _custom.finish(slot);

        if (const std::string *error = std::get_if<std::string>(&outcome)) {
            sendUploadResult(client, {false, *error});
            return;
        }
        startCustom(client, std::get<LoadedMap>(outcome));
    }

    void Room::startCustom(const Engine::ClientId client, const LoadedMap &loaded)
    {
        if (!_game.startMap(rtype::buildRun(loaded.map.levels), rtype::CUSTOM_MAP, loaded.map.zones)) {
            sendUploadResult(client, {false, "a game is already running"});
            return;
        }
        _custom.select(loaded);
        sendToMembers(rtype::protocol::CUSTOM_INFO, rtype::protocol::encode(_custom.info()),
            Engine::Delivery::Reliable);
        sendUploadResult(client, {true, ""});
        std::cout << "room " << _id << ": custom map \"" << loaded.map.name << "\" started" << std::endl;
    }

    void Room::sendUploadResult(const Engine::ClientId client, const rtype::protocol::UploadResult &result)
    {
        _server.send(client, rtype::protocol::MAP_UPLOAD_RESULT, rtype::protocol::encode(result),
            Engine::Delivery::Reliable);
    }

    void Room::sendCustomRecords(const Engine::ClientId client, const rtype::protocol::Payload &payload)
    {
        const std::optional<std::uint64_t> hash = rtype::protocol::decodeCustomRecordsGet(payload);
        rtype::protocol::Payload records;

        if (!hash) {
            return;
        }
        {
            const std::lock_guard lock(_records.mutex);

            records = rtype::protocol::encode(_custom.recordsOf(*hash));
        }
        _server.send(client, rtype::protocol::CUSTOM_RECORDS, records, Engine::Delivery::Reliable);
    }
}
