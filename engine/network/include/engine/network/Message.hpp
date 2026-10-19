#pragma once

#include <vector>
#include <cstdint>
#include "engine/network/Protocol.hpp"

namespace Engine {
    struct Message {
        MessageType type = 0;
        bool reliable = false;
        std::uint16_t id = 0;
        std::vector<std::uint8_t> payload;
    };
}
