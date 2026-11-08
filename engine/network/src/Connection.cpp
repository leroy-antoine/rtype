#include <utility>
#include <algorithm>
#include <stdexcept>
#include "engine/network/Connection.hpp"
#include "engine/network/BinaryReader.hpp"
#include "engine/network/BinaryWriter.hpp"

namespace Engine {
    Connection::Connection(std::uint32_t token, Clock::time_point now)
        : _token(token), _lastReceived(now), _lastPing(now - Protocol::PING_INTERVAL)
    {
    }

    std::uint32_t Connection::token() const
    {
        return _token;
    }

    std::uint16_t Connection::nextSequence()
    {
        return _sequence++;
    }

    std::optional<Clock::duration> Connection::latency() const
    {
        return _latency;
    }

    bool Connection::timedOut(Clock::time_point now) const
    {
        return now - _lastReceived > Protocol::TIMEOUT;
    }

    bool Connection::broken() const
    {
        return _broken;
    }

    std::optional<DisconnectReason> Connection::peerDisconnect() const
    {
        return _peerDisconnect;
    }

    bool Connection::receive(const Packet &packet, Clock::time_point now)
    {
        if (hasHandshakeMessage(packet)) {
            return false;
        }
        const bool late = _remoteSequence && !Protocol::isNewer(packet.sequence(), *_remoteSequence);

        if (!late) {
            _remoteSequence = packet.sequence();
        }
        _lastReceived = now;
        for (const Message &message : packet.messages()) {
            if (!late || message.reliable || message.type < Protocol::FIRST_USER_MESSAGE) {
                handle(message, now);
            }
        }
        return true;
    }

    std::vector<Message> Connection::takeMessages()
    {
        std::vector<Message> messages = std::move(_received);

        _received.clear();
        return messages;
    }

    void Connection::send(MessageType type, std::vector<std::uint8_t> payload, Delivery delivery,
        Clock::time_point now)
    {
        if (type < Protocol::FIRST_USER_MESSAGE || type > Protocol::LAST_USER_MESSAGE) {
            throw std::invalid_argument("send: message type must be between FIRST_USER_MESSAGE and LAST_USER_MESSAGE");
        }
        if (payload.size() > Protocol::MAX_PAYLOAD_SIZE) {
            throw std::invalid_argument("send: payload is larger than MAX_PAYLOAD_SIZE");
        }
        if (delivery == Delivery::Unreliable) {
            _outbox.push_back({type, false, 0, std::move(payload)});
            return;
        }
        if (_channel.full()) {
            _broken = true;
            return;
        }
        _outbox.push_back(_channel.send(type, std::move(payload), now));
    }

    void Connection::sendEngine(MessageType type, std::vector<std::uint8_t> payload)
    {
        _outbox.push_back({type, false, 0, std::move(payload)});
    }

    void Connection::update(Clock::time_point now)
    {
        if (now - _lastPing >= Protocol::PING_INTERVAL) {
            sendPing(now);
        }
        for (Message &message : _channel.resend(now, resendDelay())) {
            _outbox.push_back(std::move(message));
        }
    }

    std::vector<Packet> Connection::flush()
    {
        std::vector<Packet> packets;

        for (Message &message : _outbox) {
            if (packets.empty() || packets.back().size() + Packet::sizeOf(message) > Protocol::MAX_PACKET_SIZE) {
                packets.emplace_back(nextSequence(), _token);
            }
            packets.back().add(std::move(message));
        }
        _outbox.clear();
        return packets;
    }

    bool Connection::hasHandshakeMessage(const Packet &packet)
    {
        return std::any_of(packet.messages().begin(), packet.messages().end(), [](const Message &message) {
            return message.type == Protocol::CONNECT || message.type == Protocol::ACCEPT
                || message.type == Protocol::REFUSE;
        });
    }

    std::uint16_t Connection::readId(const Message &message)
    {
        BinaryReader reader(message.payload);

        return reader.readU16().value_or(0);
    }

    void Connection::handle(const Message &message, Clock::time_point now)
    {
        switch (message.type) {
            case Protocol::PING:
                sendEngine(Protocol::PONG, message.payload);
                break;
            case Protocol::PONG:
                handlePong(readId(message), now);
                break;
            case Protocol::ACK:
                _channel.acknowledge(readId(message));
                break;
            case Protocol::DISCONNECT:
                _peerDisconnect = static_cast<DisconnectReason>(message.payload[0]);
                break;
            default:
                handleUser(message);
                break;
        }
    }

    void Connection::handleUser(const Message &message)
    {
        if (!message.reliable) {
            _received.push_back(message);
            return;
        }
        if (_channel.receive(message)) {
            sendId(Protocol::ACK, message.id);
        }
        for (Message &ready : _channel.takeReady()) {
            _received.push_back(std::move(ready));
        }
    }

    void Connection::handlePong(std::uint16_t id, Clock::time_point now)
    {
        if (!_pingSentAt || id != _pingId) {
            return;
        }
        _latency = now - *_pingSentAt;
        _pingSentAt.reset();
    }

    void Connection::sendPing(Clock::time_point now)
    {
        ++_pingId;
        sendId(Protocol::PING, _pingId);
        _pingSentAt = now;
        _lastPing = now;
    }

    void Connection::sendId(MessageType type, std::uint16_t id)
    {
        BinaryWriter writer;

        writer.writeU16(id);
        sendEngine(type, writer.data());
    }

    Clock::duration Connection::resendDelay() const
    {
        return std::max<Clock::duration>(Protocol::MIN_RESEND_DELAY, 2 * _latency.value_or(Clock::duration::zero()));
    }
}
