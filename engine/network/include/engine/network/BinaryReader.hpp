#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>
#include <optional>
#include "engine/network/Export.hpp"

namespace Engine {
    class ENGINE_NETWORK_EXPORT BinaryReader
    {
    public:
        explicit BinaryReader(const std::vector<std::uint8_t> &data);

        std::optional<std::uint8_t> readU8();
        std::optional<std::uint16_t> readU16();
        std::optional<std::uint32_t> readU32();
        std::optional<float> readFloat();
        std::optional<std::uint16_t> readVarint();
        std::optional<std::vector<std::uint8_t>> readBytes(std::size_t size);

        std::size_t remaining() const;
    private:
        const std::vector<std::uint8_t> &_data;
        std::size_t _cursor = 0;
    };
}
