#include <iostream>
#include <optional>
#include <variant>
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
        while (const std::optional<Engine::ClientEvent> event = _client.pollEvent()) {
            std::visit([this, &replica](const auto &e) { handle(replica, e); }, *event);
        }
        return _open;
    }

    void Network::sendInput(const rtype::PlayerInput &input)
    {
        if (_client.connected()) {
            _client.send(rtype::protocol::INPUT, rtype::protocol::encode(input));
        }
    }

    void Network::requestRespawn()
    {
        if (_client.connected()) {
            _client.send(rtype::protocol::RESPAWN, {}, Engine::Delivery::Reliable);
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
            case rtype::protocol::YOU: assignSlot(replica, rtype::protocol::decodeYou(event.payload)); break;
            default: break;
        }
    }
}
