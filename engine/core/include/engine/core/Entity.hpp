#pragma once

#include <limits>
#include <cstdint>
#include <cstddef>
#include <functional>

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

namespace std {
    template<>
    struct hash<Engine::Entity> {
        std::size_t operator()(const Engine::Entity &e) const noexcept
        {
            return std::hash<std::uint64_t>{}(e.id);
        }
    };
}
