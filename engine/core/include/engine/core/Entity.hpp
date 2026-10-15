#pragma once

#include <limits>
#include <cstdint>

namespace Engine {
    struct Entity {
        std::uint64_t id = std::numeric_limits<std::uint64_t>::max();

        bool operator==(const Entity &) const = default;

        [[nodiscard]] std::uint32_t getIndex() const
        {
            return static_cast<std::uint32_t>(this->id & 0xFFFFFFFF);
        }

        [[nodiscard]] std::uint32_t getGeneration() const
        {
            return static_cast<std::uint32_t>(this->id >> 32);
        }
    };
}
