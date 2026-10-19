#pragma once

#include <vector>
#include <cstdint>
#include <optional>
#include "engine/network/Export.hpp"
#include "engine/network/Packet.hpp"
#include "engine/network/Message.hpp"
#include "engine/network/Protocol.hpp"
#include "engine/network/ReliableChannel.hpp"

namespace Engine {
    class ENGINE_NETWORK_EXPORT Connection
    {
    public:
        Connection(std::uint32_t token, Clock::time_point now);

        std::uint32_t token() const;
        std::uint16_t nextSequence();
        std::optional<Clock::duration> latency() const;
        bool timedOut(Clock::time_point now) const;
        bool broken() const;
        std::optional<DisconnectReason> peerDisconnect() const;

        bool receive(const Packet &packet, Clock::time_point now);
        std::vector<Message> takeMessages();

        void send(MessageType type, std::vector<std::uint8_t> payload, Delivery delivery, Clock::time_point now);
        void sendEngine(MessageType type, std::vector<std::uint8_t> payload);
        void update(Clock::time_point now);
        std::vector<Packet> flush();
    private:
        static bool hasHandshakeMessage(const Packet &packet);
        static std::uint16_t readId(const Message &message);

        void handle(const Message &message, Clock::time_point now);
        void handleUser(const Message &message);
        void handlePong(std::uint16_t id, Clock::time_point now);
        void sendPing(Clock::time_point now);
        void sendId(MessageType type, std::uint16_t id);
        Clock::duration resendDelay() const;

        std::uint32_t _token;
        std::uint16_t _sequence = 0;
        std::optional<std::uint16_t> _remoteSequence;
        Clock::time_point _lastReceived;
        ReliableChannel _channel;
        bool _broken = false;
        std::optional<DisconnectReason> _peerDisconnect;
        std::uint16_t _pingId = 0;
        Clock::time_point _lastPing;
        std::optional<Clock::time_point> _pingSentAt;
        std::optional<Clock::duration> _latency;
        std::vector<Message> _outbox;
        std::vector<Message> _received;
    };
}
