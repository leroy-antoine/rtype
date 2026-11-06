#pragma once

#include <deque>
#include <chrono>
#include <random>
#include <vector>
#include <cstdint>
#include <mutex>
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
    class ENGINE_NETWORK_EXPORT Server
    {
    public:
        Server(std::uint16_t port, std::size_t maxClients, std::uint8_t version);
        ~Server();

        Server(const Server &) = delete;
        Server &operator=(const Server &) = delete;

        std::uint16_t port() const;
        std::size_t clientCount() const;
        std::optional<std::chrono::milliseconds> latency(ClientId client);

        void update();
        std::optional<ServerEvent> pollEvent();
        void send(ClientId client, MessageType type, std::vector<std::uint8_t> payload,
            Delivery delivery = Delivery::Unreliable);
        void broadcast(MessageType type, const std::vector<std::uint8_t> &payload,
            Delivery delivery = Delivery::Unreliable);
        void disconnect(ClientId client);
        void flush();
    private:
        struct Session {
            ClientId id;
            Address address;
            Connection connection;
        };

        Session *find(ClientId id);
        Session *findByToken(std::uint32_t token);
        Session *findByAddress(const Address &address);

        void handleDatagram(const Datagram &datagram, Clock::time_point now);
        void handleConnect(const Address &from, const Packet &packet, Clock::time_point now);
        void handleSession(Session &session, const Packet &packet, Clock::time_point now);
        std::optional<RefuseReason> admit(std::uint8_t clientVersion) const;
        void open(const Address &from, Clock::time_point now);
        void checkSessions(Clock::time_point now);

        void sendAccept(Session &session);
        void sendRefuse(const Address &to, RefuseReason reason);
        void sendDisconnect(Session &session, DisconnectReason reason);
        void flush(Session &session);
        void kick(ClientId id);
        void close(ClientId id, DisconnectReason reason);
        std::uint32_t makeToken();

        UdpSocket _socket;
        std::size_t _maxClients;
        std::uint8_t _version;
        std::vector<Session> _sessions;
        std::deque<ServerEvent> _events;
        ClientId _nextClientId = 0;
        std::random_device _random;
        mutable std::mutex _mutex;
    };
}
