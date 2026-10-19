#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <optional>
#include "engine/network/Export.hpp"
#include "engine/network/Message.hpp"
#include "engine/network/Protocol.hpp"

namespace Engine {
    class ENGINE_NETWORK_EXPORT ReliableChannel
    {
    public:
        bool full() const;
        Message send(MessageType type, std::vector<std::uint8_t> payload, Clock::time_point now);
        void acknowledge(std::uint16_t id);
        std::vector<Message> resend(Clock::time_point now, Clock::duration delay);

        bool receive(const Message &message);
        std::vector<Message> takeReady();
    private:
        struct Pending {
            Message message;
            Clock::time_point sentAt;
        };

        std::uint16_t _nextId = 0;
        std::vector<Pending> _pending;
        std::uint16_t _expected = 0;
        std::array<std::optional<Message>, Protocol::MAX_EARLY> _early;
    };
}
