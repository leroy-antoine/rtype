#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include "engine/network/BinaryReader.hpp"
#include "engine/network/BinaryWriter.hpp"
#include "logic/Zones.hpp"
#include "mapjson/Limits.hpp"
#include "mapjson/Text.hpp"
#include "rtype/Protocol.hpp"

// The messages of the maps written by the players (see docs/protocol.md).
namespace rtype::protocol {
    // Client to server: the host sends the JSON of its map, in pieces.
    constexpr Engine::MessageType MAP_UPLOAD_BEGIN = 0x19;
    constexpr Engine::MessageType MAP_UPLOAD_CHUNK = 0x1A;
    constexpr Engine::MessageType MAP_UPLOAD_END = 0x1B;
    constexpr Engine::MessageType CUSTOM_RECORDS_GET = 0x1C;

    // Server to client.
    constexpr Engine::MessageType CUSTOM_INFO = 0x2B;
    constexpr Engine::MessageType MAP_UPLOAD_RESULT = 0x2C;
    constexpr Engine::MessageType CUSTOM_RECORDS = 0x2D;

    constexpr std::size_t MAX_UPLOAD_SIZE = mapjson::MAX_FILE_BYTES;
    // 1100 bytes: a full upload is 239 pieces at most, so even if no Ack came
    // back it would stay under the 256 messages that get a client kicked.
    constexpr std::size_t MAX_UPLOAD_CHUNK = 1100;
    constexpr std::size_t MAX_ERROR_TEXT = 200;
    constexpr std::size_t HASH_SIZE = 8;

    // What the clients need to show the map that is being played.
    struct CustomInfo {
        std::string name;
        std::uint64_t hash = 0;
        std::vector<Zone> zones;
    };

    struct UploadResult {
        bool ok = false;
        std::string error;
    };

    // The best times of one custom map, for each difficulty.
    struct CustomRecords {
        std::uint64_t hash = 0;
        DifficultyRecords records;
    };

    // A reason can quote the text of the map, which may hold anything: the
    // decoders refuse control characters, so they are replaced.
    inline std::string printableOnly(std::string text)
    {
        std::ranges::replace_if(text, [](const char c) {
            const auto code = static_cast<unsigned char>(c);

            return code < 0x20 || code == 0x7F;
        }, '?');
        return text;
    }

    inline void writeText(Engine::BinaryWriter &writer,
        const std::string &text)
    {
        writer.writeU8(static_cast<std::uint8_t>(text.size()));
        writer.writeBytes(Payload(text.begin(), text.end()));
    }

    inline std::optional<std::string> readText(Engine::BinaryReader &reader,
        const std::size_t maxLength)
    {
        const std::optional<std::uint8_t> size = reader.readU8();

        if (!size || *size > maxLength) {
            return std::nullopt;
        }
        const std::optional<Payload> bytes = reader.readBytes(*size);

        if (!bytes) {
            return std::nullopt;
        }
        std::string text(bytes->begin(), bytes->end());

        if (!mapjson::isPrintableText(text)) {
            return std::nullopt;
        }
        return text;
    }

    inline void writeHash(Engine::BinaryWriter &writer,
        const std::uint64_t hash)
    {
        writer.writeU32(static_cast<std::uint32_t>(hash >> 32));
        writer.writeU32(static_cast<std::uint32_t>(hash & 0xFFFFFFFFu));
    }

    inline std::optional<std::uint64_t> readHash(Engine::BinaryReader &reader)
    {
        const std::optional<std::uint32_t> high = reader.readU32();
        const std::optional<std::uint32_t> low = reader.readU32();

        if (!high || !low) {
            return std::nullopt;
        }
        return (static_cast<std::uint64_t>(*high) << 32) | *low;
    }

    // MAP_UPLOAD_BEGIN: the size in bytes of the JSON that follows.
    inline Payload encodeUploadBegin(const std::uint32_t size)
    {
        Engine::BinaryWriter writer;

        writer.writeU32(size);
        return writer.data();
    }

    inline std::optional<std::uint32_t> decodeUploadBegin(
        const Payload &payload)
    {
        Engine::BinaryReader reader(payload);
        const std::optional<std::uint32_t> size = reader.readU32();

        if (payload.size() != 4 || !size || *size == 0
            || *size > MAX_UPLOAD_SIZE) {
            return std::nullopt;
        }
        return size;
    }

    // CUSTOM_INFO: name (size u8 + bytes), hash of the map (8), then the zone
    // of each level (count u8 + one byte each).
    inline Payload encode(const CustomInfo &info)
    {
        Engine::BinaryWriter writer;

        writeText(writer, info.name);
        writeHash(writer, info.hash);
        writer.writeU8(static_cast<std::uint8_t>(info.zones.size()));
        for (const Zone zone : info.zones) {
            writer.writeU8(static_cast<std::uint8_t>(zone));
        }
        return writer.data();
    }

    inline std::optional<std::vector<Zone>> readZones(
        Engine::BinaryReader &reader)
    {
        const std::optional<std::uint8_t> count = reader.readU8();
        std::vector<Zone> zones;

        if (!count || *count > mapjson::MAX_LEVELS
            || reader.remaining() != *count) {
            return std::nullopt;
        }
        for (std::size_t i = 0; i < *count; ++i) {
            const std::optional<std::uint8_t> zone = reader.readU8();

            if (!zone || *zone >= static_cast<std::uint8_t>(Zone::Count)) {
                return std::nullopt;
            }
            zones.push_back(static_cast<Zone>(*zone));
        }
        return zones;
    }

    inline std::optional<CustomInfo> decodeCustomInfo(const Payload &payload)
    {
        Engine::BinaryReader reader(payload);
        std::optional<std::string> name = readText(reader, mapjson::MAX_TEXT);
        std::optional<std::uint64_t> hash = readHash(reader);
        std::optional<std::vector<Zone>> zones;

        if (!name || !hash) {
            return std::nullopt;
        }
        zones = readZones(reader);
        if (!zones) {
            return std::nullopt;
        }
        return CustomInfo{std::move(*name), *hash, std::move(*zones)};
    }

    // MAP_UPLOAD_RESULT: ok (u8), then the reason (size u8 + bytes).
    inline Payload encode(const UploadResult &result)
    {
        Engine::BinaryWriter writer;

        writer.writeU8(result.ok);
        writeText(writer,
            printableOnly(result.error.substr(0, MAX_ERROR_TEXT)));
        return writer.data();
    }

    inline std::optional<UploadResult> decodeUploadResult(
        const Payload &payload)
    {
        Engine::BinaryReader reader(payload);
        const std::optional<std::uint8_t> ok = reader.readU8();
        std::optional<std::string> error;

        if (!ok || *ok > 1) {
            return std::nullopt;
        }
        error = readText(reader, MAX_ERROR_TEXT);
        if (!error || reader.remaining() != 0) {
            return std::nullopt;
        }
        return UploadResult{*ok == 1, std::move(*error)};
    }

    // CUSTOM_RECORDS_GET: the hash of the map.
    inline Payload encodeCustomRecordsGet(const std::uint64_t hash)
    {
        Engine::BinaryWriter writer;

        writeHash(writer, hash);
        return writer.data();
    }

    inline std::optional<std::uint64_t> decodeCustomRecordsGet(
        const Payload &payload)
    {
        Engine::BinaryReader reader(payload);
        const std::optional<std::uint64_t> hash = readHash(reader);

        if (payload.size() != HASH_SIZE || !hash) {
            return std::nullopt;
        }
        return hash;
    }

    // CUSTOM_RECORDS: the hash, then for each difficulty (in the order of the
    // enum) the records as in LEADERBOARD: count u8, then per record the time
    // u32, the players u8 and the settings u8.
    inline Payload encode(const CustomRecords &custom)
    {
        Engine::BinaryWriter writer;

        writeHash(writer, custom.hash);
        for (const MapRecords &runs : custom.records) {
            writer.writeU8(static_cast<std::uint8_t>(runs.size()));
            for (const RunRecord &record : runs) {
                writeRecord(writer, record);
            }
        }
        return writer.data();
    }

    inline std::optional<CustomRecords> decodeCustomRecords(
        const Payload &payload)
    {
        Engine::BinaryReader reader(payload);
        const std::optional<std::uint64_t> hash = readHash(reader);
        CustomRecords custom;

        if (!hash) {
            return std::nullopt;
        }
        for (std::size_t d = 0; d < DIFFICULTY_COUNT; ++d) {
            if (!readRecords(reader, custom.records[d])
                || !isOfDifficulty(custom.records[d], d)) {
                return std::nullopt;
            }
        }
        if (reader.remaining() != 0) {
            return std::nullopt;
        }
        custom.hash = *hash;
        return custom;
    }
}
