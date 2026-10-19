#pragma once

#include <chrono>
#include <cstdint>
#include <cstddef>

namespace Engine {
    using MessageType = std::uint8_t;
    using Clock = std::chrono::steady_clock;

    enum class Delivery {
        Unreliable,
        Reliable
    };

    enum class RefuseReason : std::uint8_t {
        Version = 0,
        Full = 1
    };

    enum class DisconnectReason : std::uint8_t {
        Quit = 0,
        Shutdown = 1,
        Kicked = 2,
        Timeout = 3
    };

    namespace Protocol {
        constexpr std::size_t MAX_PENDING = 256;
        constexpr std::size_t MAX_EARLY = 64;
        constexpr std::size_t MAX_CONNECT_ATTEMPTS = 10;
        constexpr std::size_t DISCONNECT_COPIES = 3;

        constexpr std::chrono::milliseconds CONNECT_RETRY_DELAY{500};
        constexpr std::chrono::milliseconds PING_INTERVAL{1000};
        constexpr std::chrono::milliseconds TIMEOUT{5000};
        constexpr std::chrono::milliseconds MIN_RESEND_DELAY{100};

        constexpr std::uint16_t MAGIC = 0x5254;
        constexpr std::size_t HEADER_SIZE = 8;
        constexpr std::size_t MAX_PACKET_SIZE = 1200;
        constexpr std::size_t MAX_PAYLOAD_SIZE = 1187;

        constexpr std::uint8_t RELIABLE_FLAG = 0x80;
        constexpr std::uint8_t TYPE_MASK = 0x7F;

        constexpr MessageType CONNECT = 0x00;
        constexpr MessageType ACCEPT = 0x01;
        constexpr MessageType REFUSE = 0x02;
        constexpr MessageType PING = 0x03;
        constexpr MessageType PONG = 0x04;
        constexpr MessageType DISCONNECT = 0x05;
        constexpr MessageType ACK = 0x06;

        constexpr MessageType FIRST_USER_MESSAGE = 0x10;
        constexpr MessageType LAST_USER_MESSAGE = 0x7F;

        constexpr bool isNewer(std::uint16_t a, std::uint16_t b)
        {
            return a != b && static_cast<std::uint16_t>(a - b) < 0x8000;
        }
    }
}
