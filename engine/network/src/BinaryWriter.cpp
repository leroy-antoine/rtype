#include <bit>
#include <stdexcept>
#include "engine/network/BinaryWriter.hpp"

namespace Engine {
    void BinaryWriter::writeU8(std::uint8_t value)
    {
        _data.push_back(value);
    }

    void BinaryWriter::writeU16(std::uint16_t value)
    {
        writeU8(static_cast<std::uint8_t>(value >> 8));
        writeU8(static_cast<std::uint8_t>(value));
    }

    void BinaryWriter::writeU32(std::uint32_t value)
    {
        writeU16(static_cast<std::uint16_t>(value >> 16));
        writeU16(static_cast<std::uint16_t>(value));
    }

    void BinaryWriter::writeFloat(float value)
    {
        writeU32(std::bit_cast<std::uint32_t>(value));
    }

    void BinaryWriter::writeVarint(std::uint16_t value)
    {
        if (value > MAX_VARINT) {
            throw std::invalid_argument("writeVarint: value does not fit in two bytes");
        }
        if (value < 0x80) {
            writeU8(static_cast<std::uint8_t>(value));
            return;
        }
        writeU8(static_cast<std::uint8_t>((value & 0x7F) | 0x80));
        writeU8(static_cast<std::uint8_t>(value >> 7));
    }

    void BinaryWriter::writeBytes(const std::vector<std::uint8_t> &bytes)
    {
        _data.insert(_data.end(), bytes.begin(), bytes.end());
    }

    const std::vector<std::uint8_t> &BinaryWriter::data() const
    {
        return _data;
    }
}
