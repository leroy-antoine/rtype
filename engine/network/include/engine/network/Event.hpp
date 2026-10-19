#pragma once

#include <vector>
#include <cstdint>
#include <variant>
#include "engine/network/Protocol.hpp"

namespace Engine {
    using ClientId = std::uint32_t;

    namespace ServerEvents {
        struct Connected {
            ClientId client;
        };

        struct Disconnected {
            ClientId client;
            DisconnectReason reason;
        };

        struct Message {
            ClientId client;
            MessageType type;
            std::vector<std::uint8_t> payload;
        };
    }

    namespace ClientEvents {
        struct Connected {
        };

        struct Refused {
            RefuseReason reason;
        };

        struct Unreachable {
        };

        struct Disconnected {
            DisconnectReason reason;
        };

        struct Message {
            MessageType type;
            std::vector<std::uint8_t> payload;
        };
    }

    using ServerEvent = std::variant<
        ServerEvents::Connected,
        ServerEvents::Disconnected,
        ServerEvents::Message
    >;

    using ClientEvent = std::variant<
        ClientEvents::Connected,
        ClientEvents::Refused,
        ClientEvents::Unreachable,
        ClientEvents::Disconnected,
        ClientEvents::Message
    >;
}
