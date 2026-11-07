#include <iostream>
#include <optional>
#include <variant>
#include <algorithm>
#include <utility>
#include "rtype/CustomMapProtocol.hpp"
#include "rtype/Protocol.hpp"
#include "Network.hpp"

namespace client {
    namespace {
        template <typename T>
        void applyDecoded(Replica &replica, const std::optional<T> &decoded)
        {
            if (decoded) {
                replica.apply(*decoded);
            }
        }
    }

    void Network::assignSlot(Replica &replica, const std::optional<int> &slot)
    {
        if (slot) {
            replica.assignSlot(*slot);
        }
    }

    bool Network::connected() const
    {
        return _client.connected();
    }

    Network::Network(const std::string &host, const std::uint16_t port)
        : _client(host, port, rtype::protocol::VERSION)
    {
    }

    bool Network::update(Replica &replica)
    {
        _client.update();
        pumpUpload();
        while (const std::optional<Engine::ClientEvent> event = _client.pollEvent()) {
            std::visit([this, &replica](const auto &e) { handle(replica, e); }, *event);
        }
        return _open;
    }

    void Network::sendInput(const std::uint32_t sequence, const rtype::PlayerInput &input)
    {
        if (_client.connected()) {
            _client.send(rtype::protocol::INPUT, rtype::protocol::encodeInput(sequence, input));
        }
    }

    void Network::requestRespawn()
    {
        if (_client.connected()) {
            _client.send(rtype::protocol::RESPAWN, {}, Engine::Delivery::Reliable);
        }
    }

    void Network::selectMap(const rtype::MapId map)
    {
        if (_client.connected()) {
            _client.send(rtype::protocol::SELECT_MAP,
                rtype::protocol::encodeSelectMap(map),
                Engine::Delivery::Reliable);
        }
    }

    void Network::leaveMatch()
    {
        if (_client.connected()) {
            _client.send(rtype::protocol::LEAVE_MATCH, {},
                Engine::Delivery::Reliable);
        }
    }

    void Network::setPause(const bool paused)
    {
        if (_client.connected()) {
            _client.send(rtype::protocol::PAUSE,
                rtype::protocol::encodePause(paused),
                Engine::Delivery::Reliable);
        }
    }

    void Network::requestReady(const bool ready)
    {
        if (_client.connected()) {
            _client.send(rtype::protocol::READY, rtype::protocol::encodeReady(ready), Engine::Delivery::Reliable);
        }
    }

    void Network::requestHistory()
    {
        if (_client.connected()) {
            _client.send(rtype::protocol::HISTORY_REQUEST, {},
                Engine::Delivery::Reliable);
        }
    }

    void Network::changeSettings(const rtype::Settings &settings)
    {
        if (_client.connected()) {
            _client.send(rtype::protocol::SETTINGS,
                rtype::protocol::encodeSettings(settings),
                Engine::Delivery::Reliable);
        }
    }

    void Network::buy(const rtype::AugmentId id)
    {
        if (_client.connected()) {
            _client.send(rtype::protocol::BUY, rtype::protocol::encodeBuy(id), Engine::Delivery::Reliable);
        }
    }

    void Network::uploadMap(std::string text)
    {
        _upload = Upload{std::move(text), 0, false};
    }

    bool Network::uploading() const
    {
        return _upload.has_value();
    }

    void Network::requestCustomRecords(const std::uint64_t hash)
    {
        if (_client.connected()) {
            _client.send(rtype::protocol::CUSTOM_RECORDS_GET,
                rtype::protocol::encodeCustomRecordsGet(hash),
                Engine::Delivery::Reliable);
        }
    }

    void Network::pumpUpload()
    {
        if (!_upload || !_client.connected()) {
            return;
        }
        if (!_upload->begun) {
            _client.send(rtype::protocol::MAP_UPLOAD_BEGIN,
                rtype::protocol::encodeUploadBegin(
                    static_cast<std::uint32_t>(_upload->text.size())),
                Engine::Delivery::Reliable);
            _upload->begun = true;
        }
        sendChunks(*_upload);
    }

    void Network::sendChunks(Upload &upload)
    {
        for (std::size_t i = 0; i < CHUNKS_PER_UPDATE
            && upload.sent < upload.text.size(); ++i) {
            const std::size_t size = std::min(rtype::protocol::MAX_UPLOAD_CHUNK,
                upload.text.size() - upload.sent);
            const std::string piece = upload.text.substr(upload.sent, size);

            _client.send(rtype::protocol::MAP_UPLOAD_CHUNK,
                std::vector<std::uint8_t>(piece.begin(), piece.end()),
                Engine::Delivery::Reliable);
            upload.sent += size;
        }
        if (upload.sent >= upload.text.size()) {
            _client.send(rtype::protocol::MAP_UPLOAD_END, {},
                Engine::Delivery::Reliable);
            _upload.reset();
        }
    }

    void Network::flush()
    {
        _client.flush();
    }

    void Network::handle(Replica &, const Engine::ClientEvents::Connected &)
    {
        std::cout << "R-Type: connected" << std::endl;
    }

    void Network::handle(Replica &, const Engine::ClientEvents::Refused &event)
    {
        std::cerr << "R-Type: refused by the server (reason " << static_cast<int>(event.reason) << ")" << std::endl;
        _open = false;
    }

    void Network::handle(Replica &, const Engine::ClientEvents::Unreachable &)
    {
        std::cerr << "R-Type: server unreachable" << std::endl;
        _open = false;
    }

    void Network::handle(Replica &, const Engine::ClientEvents::Disconnected &event)
    {
        std::cerr << "R-Type: disconnected (reason " << static_cast<int>(event.reason) << ")" << std::endl;
        _open = false;
    }

    void Network::handle(Replica &replica, const Engine::ClientEvents::Message &event)
    {
        switch (event.type) {
            case rtype::protocol::SPAWN: applyDecoded(replica, rtype::protocol::decodeSpawn(event.payload)); break;
            case rtype::protocol::DESTROY: applyDecoded(replica, rtype::protocol::decodeDestroy(event.payload)); break;
            case rtype::protocol::STATES: applyDecoded(replica, rtype::protocol::decodeStates(event.payload)); break;
            case rtype::protocol::STATUS: applyDecoded(replica, rtype::protocol::decodeStatus(event.payload)); break;
            case rtype::protocol::OWN_STATE: applyDecoded(replica, rtype::protocol::decodeOwnState(event.payload)); break;
            case rtype::protocol::LEADERBOARD: applyDecoded(replica, rtype::protocol::decodeLeaderboard(event.payload)); break;
            case rtype::protocol::HISTORY: applyDecoded(replica, rtype::protocol::decodeHistory(event.payload)); break;
            case rtype::protocol::AUGMENTS: applyDecoded(replica, rtype::protocol::decodeAugments(event.payload)); break;
            case rtype::protocol::SHOP: applyDecoded(replica, rtype::protocol::decodeShop(event.payload)); break;
            case rtype::protocol::BUY_RESULT: applyDecoded(replica, rtype::protocol::decodeBuyResult(event.payload)); break;
            case rtype::protocol::CUSTOM_INFO:
                applyDecoded(replica,
                    rtype::protocol::decodeCustomInfo(event.payload));
                break;
            case rtype::protocol::MAP_UPLOAD_RESULT:
                applyDecoded(replica,
                    rtype::protocol::decodeUploadResult(event.payload));
                break;
            case rtype::protocol::CUSTOM_RECORDS:
                applyDecoded(replica,
                    rtype::protocol::decodeCustomRecords(event.payload));
                break;
            case rtype::protocol::YOU: assignSlot(replica, rtype::protocol::decodeYou(event.payload)); break;
            default: break;
        }
    }
}
