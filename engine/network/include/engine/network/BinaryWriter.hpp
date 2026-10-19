#pragma once

#include <vector>
#include <cstdint>
#include "engine/network/Export.hpp"

namespace Engine {
    class ENGINE_NETWORK_EXPORT BinaryWriter
    {
    public:
        static constexpr std::uint16_t MAX_VARINT = 16383;

        void writeU8(std::uint8_t value);
        void writeU16(std::uint16_t value);
        void writeU32(std::uint32_t value);
        void writeFloat(float value);
        void writeVarint(std::uint16_t value);
        void writeBytes(const std::vector<std::uint8_t> &bytes);

        const std::vector<std::uint8_t> &data() const;
    private:
        std::vector<std::uint8_t> _data;
    };
}
