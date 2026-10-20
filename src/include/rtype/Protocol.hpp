#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>
#include "engine/network/BinaryReader.hpp"
#include "engine/network/BinaryWriter.hpp"
#include "engine/network/Protocol.hpp"
#include "utils/Vectors.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Events.hpp"

namespace rtype::protocol {
    using Payload = std::vector<std::uint8_t>;

    constexpr std::uint8_t VERSION = 1;

    constexpr Engine::MessageType INPUT = 0x10;
    constexpr Engine::MessageType RESPAWN = 0x11;

    constexpr Engine::MessageType SPAWN = 0x20;
    constexpr Engine::MessageType DESTROY = 0x21;
    constexpr Engine::MessageType STATES = 0x22;

    constexpr std::uint8_t KEY_UP = 1 << 0;
    constexpr std::uint8_t KEY_DOWN = 1 << 1;
    constexpr std::uint8_t KEY_LEFT = 1 << 2;
    constexpr std::uint8_t KEY_RIGHT = 1 << 3;
    constexpr std::uint8_t KEY_FIRE = 1 << 4;
    constexpr std::uint8_t KEYS_MASK = 0x1F;

    constexpr std::size_t SPAWN_SIZE = 22;
    constexpr std::size_t STATE_SIZE = 12;
    constexpr std::size_t MAX_STATES_PER_MESSAGE =
        (Engine::Protocol::MAX_PAYLOAD_SIZE - 1) / STATE_SIZE;

    inline void writeVec2(Engine::BinaryWriter &writer,
        const gameLogic::Vec2 &v)
    {
        writer.writeFloat(v.x);
        writer.writeFloat(v.y);
    }

    inline std::optional<gameLogic::Vec2> readVec2(
        Engine::BinaryReader &reader)
    {
        const std::optional<float> x = reader.readFloat();
        const std::optional<float> y = reader.readFloat();

        if (!x || !y || !std::isfinite(*x) || !std::isfinite(*y)) {
            return std::nullopt;
        }
        return gameLogic::Vec2{*x, *y};
    }

    inline bool hasKey(const std::uint8_t keys, const std::uint8_t key)
    {
        return (keys & key) != 0;
    }

    inline Payload encode(const PlayerInput &input)
    {
        std::uint8_t keys = 0;

        if (input.up)
            keys |= KEY_UP;
        if (input.down)
            keys |= KEY_DOWN;
        if (input.left)
            keys |= KEY_LEFT;
        if (input.right)
            keys |= KEY_RIGHT;
        if (input.fire)
            keys |= KEY_FIRE;
        return {keys};
    }

    inline std::optional<PlayerInput> decodeInput(const Payload &payload)
    {
        if (payload.size() != 1 || (payload[0] & ~KEYS_MASK) != 0) {
            return std::nullopt;
        }
        const std::uint8_t keys = payload[0];

        return PlayerInput{hasKey(keys, KEY_UP), hasKey(keys, KEY_DOWN),
            hasKey(keys, KEY_LEFT), hasKey(keys, KEY_RIGHT),
            hasKey(keys, KEY_FIRE)};
    }

    inline Engine::MessageType typeOf(const EntitySpawned &)
    {
        return SPAWN;
    }

    inline Engine::MessageType typeOf(const EntityDestroyed &)
    {
        return DESTROY;
    }

    inline Payload encode(const EntitySpawned &spawn)
    {
        Engine::BinaryWriter writer;

        writer.writeU32(spawn.id);
        writer.writeU8(static_cast<std::uint8_t>(spawn.sprite));
        writer.writeU8(static_cast<std::uint8_t>(spawn.slot));
        writeVec2(writer, spawn.pos);
        writeVec2(writer, spawn.box);
        return writer.data();
    }

    inline bool isValidSpawn(const std::uint8_t sprite, const std::uint8_t slot)
    {
        if (sprite >= static_cast<std::uint8_t>(SpriteId::Count)) {
            return false;
        }
        if (sprite == static_cast<std::uint8_t>(SpriteId::Player)) {
            return slot < config::MAX_PLAYERS;
        }
        return true;
    }

    inline std::optional<EntitySpawned> decodeSpawn(const Payload &payload)
    {
        Engine::BinaryReader reader(payload);
        const std::optional<std::uint32_t> id = reader.readU32();
        const std::optional<std::uint8_t> sprite = reader.readU8();
        const std::optional<std::uint8_t> slot = reader.readU8();
        const std::optional<gameLogic::Vec2> pos = readVec2(reader);
        const std::optional<gameLogic::Vec2> box = readVec2(reader);

        if (payload.size() != SPAWN_SIZE || !id || !sprite || !slot
            || !pos || !box || !isValidSpawn(*sprite, *slot)) {
            return std::nullopt;
        }
        return EntitySpawned{*id, static_cast<SpriteId>(*sprite), *slot,
            gameLogic::Vec2Pos{*pos}, gameLogic::Vec2Hitbox{*box}};
    }

    inline Payload encode(const EntityDestroyed &destroy)
    {
        Engine::BinaryWriter writer;

        writer.writeU32(destroy.id);
        return writer.data();
    }

    inline std::optional<EntityDestroyed> decodeDestroy(const Payload &payload)
    {
        Engine::BinaryReader reader(payload);
        const std::optional<std::uint32_t> id = reader.readU32();

        if (!id || reader.remaining() != 0) {
            return std::nullopt;
        }
        return EntityDestroyed{*id};
    }

    inline Payload encodeStateChunk(const std::span<const EntityState> chunk)
    {
        Engine::BinaryWriter writer;

        writer.writeU8(static_cast<std::uint8_t>(chunk.size()));
        for (const EntityState &state : chunk) {
            writer.writeU32(state.id);
            writeVec2(writer, state.pos);
        }
        return writer.data();
    }

    inline std::vector<Payload> encodeStates(
        const std::vector<EntityState> &states)
    {
        const std::span<const EntityState> all(states);
        std::vector<Payload> payloads;

        for (std::size_t first = 0; first < all.size();
            first += MAX_STATES_PER_MESSAGE) {
            const std::size_t count =
                std::min(MAX_STATES_PER_MESSAGE, all.size() - first);

            payloads.push_back(encodeStateChunk(all.subspan(first, count)));
        }
        return payloads;
    }

    inline std::optional<EntityState> readState(Engine::BinaryReader &reader)
    {
        const std::optional<std::uint32_t> id = reader.readU32();
        const std::optional<gameLogic::Vec2> pos = readVec2(reader);

        if (!id || !pos) {
            return std::nullopt;
        }
        return EntityState{*id, gameLogic::Vec2Pos{*pos}};
    }

    inline std::optional<std::vector<EntityState>> decodeStates(
        const Payload &payload)
    {
        Engine::BinaryReader reader(payload);
        const std::optional<std::uint8_t> count = reader.readU8();
        std::vector<EntityState> states;

        if (!count || reader.remaining() != *count * STATE_SIZE) {
            return std::nullopt;
        }
        for (std::size_t i = 0; i < *count; ++i) {
            const std::optional<EntityState> state = readState(reader);

            if (!state) {
                return std::nullopt;
            }
            states.push_back(*state);
        }
        return states;
    }
}
