#pragma once

#include <cstdint>

namespace Engine {
    struct Address {
        std::uint32_t ip = 0;
        std::uint16_t port = 0;

        bool operator==(const Address &) const = default;
    };
}
