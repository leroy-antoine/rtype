#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>
#include <optional>
#include "engine/network/Export.hpp"
#include "engine/network/Message.hpp"
#include "engine/network/BinaryReader.hpp"

namespace Engine {
    class ENGINE_NETWORK_EXPORT Packet
    {
    public:
        Packet(std::uint16_t sequence, std::uint32_t token);

        std::uint16_t sequence() const;
        std::uint32_t token() const;
        const std::vector<Message> &messages() const;
        bool isHandshake(MessageType type) const;

        void add(Message message);
        std::size_t size() const;
        std::vector<std::uint8_t> serialize() const;

        static std::size_t sizeOf(const Message &message);
        static std::optional<Packet> parse(const std::vector<std::uint8_t> &bytes);
    private:
        static std::optional<Message> parseMessage(BinaryReader &reader);
        static bool isValid(const Message &message);

        std::uint16_t _sequence;
        std::uint32_t _token;
        std::vector<Message> _messages;
    };
}
