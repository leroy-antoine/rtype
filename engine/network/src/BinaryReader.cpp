#include <bit>
#include "engine/network/BinaryReader.hpp"

namespace Engine {
    BinaryReader::BinaryReader(const std::vector<std::uint8_t> &data) : _data(data)
    {
    }

    std::optional<std::uint8_t> BinaryReader::readU8()
    {
        std::uint8_t value;

        if (remaining() < 1) {
            return std::nullopt;
        }
        value = _data[_cursor];
        _cursor += 1;
        return value;
    }

    std::optional<std::uint16_t> BinaryReader::readU16()
    {
        std::uint16_t value;

        if (remaining() < 2) {
            return std::nullopt;
        }
        value = static_cast<std::uint16_t>((_data[_cursor] << 8) | _data[_cursor + 1]);
        _cursor += 2;
        return value;
    }

    std::optional<std::uint32_t> BinaryReader::readU32()
    {
        std::uint32_t value = 0;

        if (remaining() < 4) {
            return std::nullopt;
        }
        for (std::size_t i = 0; i < 4; ++i) {
            value = (value << 8) | _data[_cursor + i];
        }
        _cursor += 4;
        return value;
    }

    std::optional<float> BinaryReader::readFloat()
    {
        std::optional<std::uint32_t> bits = readU32();

        if (!bits) {
            return std::nullopt;
        }
        return std::bit_cast<float>(*bits);
    }

    std::optional<std::uint16_t> BinaryReader::readVarint()
    {
        std::optional<std::uint8_t> low = readU8();
        std::optional<std::uint8_t> high;

        if (!low) {
            return std::nullopt;
        }
        if (*low < 0x80) {
            return *low;
        }
        high = readU8();
        if (!high || *high == 0 || *high >= 0x80) {
            return std::nullopt;
        }
        return static_cast<std::uint16_t>((*low & 0x7F) | (*high << 7));
    }

    std::optional<std::vector<std::uint8_t>> BinaryReader::readBytes(std::size_t size)
    {
        std::vector<std::uint8_t> bytes;

        if (remaining() < size) {
            return std::nullopt;
        }
        bytes.assign(_data.begin() + static_cast<std::ptrdiff_t>(_cursor),
            _data.begin() + static_cast<std::ptrdiff_t>(_cursor + size));
        _cursor += size;
        return bytes;
    }

    std::size_t BinaryReader::remaining() const
    {
        return _data.size() - _cursor;
    }
}
