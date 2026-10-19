#include <utility>
#include "engine/network/ReliableChannel.hpp"

namespace Engine {
    bool ReliableChannel::full() const
    {
        return _pending.size() >= Protocol::MAX_PENDING;
    }

    Message ReliableChannel::send(MessageType type, std::vector<std::uint8_t> payload, Clock::time_point now)
    {
        Message message{type, true, _nextId, std::move(payload)};

        ++_nextId;
        _pending.push_back({message, now});
        return message;
    }

    void ReliableChannel::acknowledge(std::uint16_t id)
    {
        std::erase_if(_pending, [id](const Pending &pending) {
            return pending.message.id == id;
        });
    }

    std::vector<Message> ReliableChannel::resend(Clock::time_point now, Clock::duration delay)
    {
        std::vector<Message> messages;

        for (Pending &pending : _pending) {
            if (now - pending.sentAt >= delay) {
                messages.push_back(pending.message);
                pending.sentAt = now;
            }
        }
        return messages;
    }

    bool ReliableChannel::receive(const Message &message)
    {
        std::uint16_t ahead = static_cast<std::uint16_t>(message.id - _expected);
        std::optional<Message> &slot = _early[message.id % Protocol::MAX_EARLY];

        if (Protocol::isNewer(_expected, message.id)) {
            return true;
        }
        if (ahead >= Protocol::MAX_EARLY) {
            return false;
        }
        if (!slot) {
            slot = message;
        }
        return true;
    }

    std::vector<Message> ReliableChannel::takeReady()
    {
        std::vector<Message> ready;

        while (_early[_expected % Protocol::MAX_EARLY]) {
            ready.push_back(std::move(*_early[_expected % Protocol::MAX_EARLY]));
            _early[_expected % Protocol::MAX_EARLY].reset();
            ++_expected;
        }
        return ready;
    }
}
