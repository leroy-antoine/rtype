#include <utility>
#include "engine/network/Packet.hpp"
#include "engine/network/BinaryWriter.hpp"

namespace Engine {
    Packet::Packet(std::uint16_t sequence, std::uint32_t token) : _sequence(sequence), _token(token)
    {
    }

    std::uint16_t Packet::sequence() const
    {
        return _sequence;
    }

    std::uint32_t Packet::token() const
    {
        return _token;
    }

    const std::vector<Message> &Packet::messages() const
    {
        return _messages;
    }

    bool Packet::isHandshake(MessageType type) const
    {
        return _token == 0 && _messages.size() == 1 && _messages[0].type == type;
    }

    void Packet::add(Message message)
    {
        _messages.push_back(std::move(message));
    }

    std::size_t Packet::size() const
    {
        std::size_t total = Protocol::HEADER_SIZE;

        for (const Message &message : _messages) {
            total += sizeOf(message);
        }
        return total;
    }

    std::vector<std::uint8_t> Packet::serialize() const
    {
        BinaryWriter writer;

        writer.writeU16(Protocol::MAGIC);
        writer.writeU16(_sequence);
        writer.writeU32(_token);
        for (const Message &message : _messages) {
            if (message.reliable) {
                writer.writeU8(static_cast<std::uint8_t>(message.type | Protocol::RELIABLE_FLAG));
                writer.writeU16(message.id);
            } else {
                writer.writeU8(message.type);
            }
            writer.writeVarint(static_cast<std::uint16_t>(message.payload.size()));
            writer.writeBytes(message.payload);
        }
        return writer.data();
    }

    std::size_t Packet::sizeOf(const Message &message)
    {
        std::size_t size = 1 + 1 + message.payload.size();

        if (message.reliable) {
            size += 2;
        }
        if (message.payload.size() >= 0x80) {
            size += 1;
        }
        return size;
    }

    std::optional<Packet> Packet::parse(const std::vector<std::uint8_t> &bytes)
    {
        BinaryReader reader(bytes);
        std::optional<std::uint16_t> magic = reader.readU16();
        std::optional<std::uint16_t> sequence = reader.readU16();
        std::optional<std::uint32_t> token = reader.readU32();
        std::optional<Message> message;

        if (bytes.size() > Protocol::MAX_PACKET_SIZE || !magic || *magic != Protocol::MAGIC || !sequence || !token) {
            return std::nullopt;
        }
        Packet packet(*sequence, *token);

        while (reader.remaining() > 0) {
            message = parseMessage(reader);
            if (!message) {
                return std::nullopt;
            }
            packet.add(std::move(*message));
        }
        if (packet._messages.empty()) {
            return std::nullopt;
        }
        return packet;
    }

    std::optional<Message> Packet::parseMessage(BinaryReader &reader)
    {
        std::optional<std::uint8_t> head = reader.readU8();
        std::optional<std::uint16_t> id = 0;
        std::optional<std::uint16_t> size;
        std::optional<std::vector<std::uint8_t>> payload;
        Message message;

        if (!head) {
            return std::nullopt;
        }
        message.type = static_cast<MessageType>(*head & Protocol::TYPE_MASK);
        message.reliable = (*head & Protocol::RELIABLE_FLAG) != 0;
        if (message.reliable) {
            id = reader.readU16();
        }
        size = reader.readVarint();
        if (!id || !size) {
            return std::nullopt;
        }
        payload = reader.readBytes(*size);
        if (!payload) {
            return std::nullopt;
        }
        message.id = *id;
        message.payload = std::move(*payload);
        if (!isValid(message)) {
            return std::nullopt;
        }
        return message;
    }

    bool Packet::isValid(const Message &message)
    {
        std::size_t size = message.payload.size();

        if (message.type >= Protocol::FIRST_USER_MESSAGE) {
            return true;
        }
        if (message.reliable) {
            return false;
        }
        switch (message.type) {
            case Protocol::CONNECT:
                return size >= 1;
            case Protocol::ACCEPT:
                return size == 4;
            case Protocol::REFUSE:
                return size == 1 && message.payload[0] <= 1;
            case Protocol::PING:
            case Protocol::PONG:
            case Protocol::ACK:
                return size == 2;
            case Protocol::DISCONNECT:
                return size == 1 && message.payload[0] <= 2;
            default:
                return false;
        }
    }
}
