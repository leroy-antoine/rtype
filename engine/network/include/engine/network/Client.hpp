#pragma once

#include <deque>
#include <chrono>
#include <string>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <optional>
#include "engine/network/Event.hpp"
#include "engine/network/Export.hpp"
#include "engine/network/Packet.hpp"
#include "engine/network/Address.hpp"
#include "engine/network/Protocol.hpp"
#include "engine/network/UdpSocket.hpp"
#include "engine/network/Connection.hpp"

namespace Engine {
    class ENGINE_NETWORK_EXPORT Client
    {
    public:
        Client(const std::string &host, std::uint16_t port, std::uint8_t version);
        ~Client();

        Client(const Client &) = delete;
        Client &operator=(const Client &) = delete;

        bool connected() const;
        std::optional<std::chrono::milliseconds> latency() const;

        void update();
        std::optional<ClientEvent> pollEvent();
        void send(MessageType type, std::vector<std::uint8_t> payload, Delivery delivery = Delivery::Unreliable);
        void disconnect();
        void flush();
    private:
        enum class State {
            Connecting,
            Connected,
            Closed
        };

        void handleDatagram(const Datagram &datagram, Clock::time_point now);
        void handleHandshake(const Packet &packet, Clock::time_point now);
        void handleSession(const Packet &packet, Clock::time_point now);
        void tryConnect(Clock::time_point now);
        void checkConnection(Clock::time_point now);
        void sendDisconnect(DisconnectReason reason);
        void flushConnection();
        void close(DisconnectReason reason);

        Address _server;
        UdpSocket _socket;
        std::uint8_t _version;
        State _state = State::Connecting;
        std::optional<Connection> _connection;
        std::size_t _attempts = 0;
        Clock::time_point _lastAttempt;
        std::uint16_t _handshakeSequence = 0;
        std::deque<ClientEvent> _events;
    };
}
