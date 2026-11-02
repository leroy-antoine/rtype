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
#include "logic/Components.hpp"
#include "logic/Config.hpp"
#include "logic/Events.hpp"
#include "logic/Leaderboard.hpp"
#include "logic/MapList.hpp"
#include "logic/Status.hpp"

namespace rtype::protocol {
    using Payload = std::vector<std::uint8_t>;
    struct StatesSnapshot {
        std::uint32_t tick = 0;
        std::vector<EntityState> states;
    };

    struct InputMessage {
        std::uint32_t sequence = 0;
        PlayerInput input;
    };

    struct OwnState {
        std::uint32_t lastInput = 0;
        gameLogic::Vec2Pos pos;
    };

    constexpr std::uint8_t VERSION = 11;

    constexpr Engine::MessageType INPUT = 0x10;
    constexpr Engine::MessageType RESPAWN = 0x11;
    constexpr Engine::MessageType SELECT_MAP = 0x12;

    constexpr Engine::MessageType SPAWN = 0x20;
    constexpr Engine::MessageType DESTROY = 0x21;
    constexpr Engine::MessageType STATES = 0x22;
    constexpr Engine::MessageType STATUS = 0x23;
    constexpr Engine::MessageType YOU = 0x24;
    constexpr Engine::MessageType OWN_STATE = 0x25;
    constexpr Engine::MessageType LEADERBOARD = 0x26;

    constexpr std::uint8_t KEY_UP = 1 << 0;
    constexpr std::uint8_t KEY_DOWN = 1 << 1;
    constexpr std::uint8_t KEY_LEFT = 1 << 2;
    constexpr std::uint8_t KEY_RIGHT = 1 << 3;
    constexpr std::uint8_t KEY_FIRE = 1 << 4;
    constexpr std::uint8_t KEYS_MASK = 0x1F;

    constexpr std::size_t INPUT_SIZE = 5;
    constexpr std::size_t OWN_STATE_SIZE = 12;
    constexpr std::size_t SPAWN_SIZE = 22;
    constexpr std::size_t STATE_SIZE = 12;
    constexpr std::size_t STATES_HEADER_SIZE = 5;
    constexpr std::size_t MAX_STATES_PER_MESSAGE =
        (Engine::Protocol::MAX_PAYLOAD_SIZE - STATES_HEADER_SIZE) / STATE_SIZE;

    // STATUS : phase (1), boss hp (2), boss max hp (2), pieces (4), temps
    // en ms (4), niveau (1), map (1), nombre de joueurs (1), puis par
    // joueur : slot (1), hp (2), vivant (1).
    constexpr std::size_t STATUS_HEADER_SIZE = 16;
    constexpr std::size_t RECORD_SIZE = 5;
    constexpr std::size_t PLAYER_STATUS_SIZE = 4;

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

    inline std::uint8_t encodeKeys(const PlayerInput &input)
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
        return keys;
    }

    inline Payload encodeInput(const std::uint32_t sequence,
        const PlayerInput &input)
    {
        Engine::BinaryWriter writer;

        writer.writeU32(sequence);
        writer.writeU8(encodeKeys(input));
        return writer.data();
    }

    inline std::optional<InputMessage> decodeInput(const Payload &payload)
    {
        Engine::BinaryReader reader(payload);
        const std::optional<std::uint32_t> sequence = reader.readU32();
        const std::optional<std::uint8_t> keys = reader.readU8();

        if (payload.size() != INPUT_SIZE || !sequence || !keys
            || (*keys & ~KEYS_MASK) != 0) {
            return std::nullopt;
        }
        return InputMessage{*sequence, PlayerInput{hasKey(*keys, KEY_UP),
            hasKey(*keys, KEY_DOWN), hasKey(*keys, KEY_LEFT),
            hasKey(*keys, KEY_RIGHT), hasKey(*keys, KEY_FIRE)}};
    }

    inline Payload encode(const OwnState &state)
    {
        Engine::BinaryWriter writer;

        writer.writeU32(state.lastInput);
        writeVec2(writer, state.pos);
        return writer.data();
    }

    inline std::optional<OwnState> decodeOwnState(const Payload &payload)
    {
        Engine::BinaryReader reader(payload);
        const std::optional<std::uint32_t> lastInput = reader.readU32();
        const std::optional<gameLogic::Vec2> pos = readVec2(reader);

        if (payload.size() != OWN_STATE_SIZE || !lastInput || !pos) {
            return std::nullopt;
        }
        return OwnState{*lastInput, gameLogic::Vec2Pos{*pos}};
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

    inline Payload encodeStateChunk(const std::uint32_t tick,
          const std::span<const EntityState> chunk)
    {
        Engine::BinaryWriter writer;

        writer.writeU32(tick);
        writer.writeU8(static_cast<std::uint8_t>(chunk.size()));
        for (const EntityState &state : chunk) {
            writer.writeU32(state.id);
            writeVec2(writer, state.pos);
        }
        return writer.data();
    }

    inline std::vector<Payload> encodeStates(const std::uint32_t tick,
        const std::vector<EntityState> &states)
    {
        const std::span<const EntityState> all(states);
        std::vector<Payload> payloads;

        for (std::size_t first = 0; first < all.size();
            first += MAX_STATES_PER_MESSAGE) {
            const std::size_t count =
                std::min(MAX_STATES_PER_MESSAGE, all.size() - first);

            payloads.push_back(encodeStateChunk(tick, all.subspan(first, count)));
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

    inline std::optional<StatesSnapshot> decodeStates(const Payload &payload)
    {
        Engine::BinaryReader reader(payload);
        const std::optional<std::uint32_t> tick = reader.readU32();
        const std::optional<std::uint8_t> count = reader.readU8();
        StatesSnapshot snapshot;

        if (!tick || !count || reader.remaining() != *count * STATE_SIZE) {
            return std::nullopt;
        }
        snapshot.tick = *tick;
        for (std::size_t i = 0; i < *count; ++i) {
            const std::optional<EntityState> state = readState(reader);

            if (!state) {
                return std::nullopt;
            }
            snapshot.states.push_back(*state);
        }
        return snapshot;
    }

    // YOU : le slot que le serveur a attribue au client qui vient de se connecter.
    inline Payload encodeYou(const int slot)
    {
        return {static_cast<std::uint8_t>(slot)};
    }

    inline std::optional<int> decodeYou(const Payload &payload)
    {
        if (payload.size() != 1 || payload[0] >= config::MAX_PLAYERS) {
            return std::nullopt;
        }
        return payload[0];
    }

    inline Payload encode(const GameStatus &status)
    {
        Engine::BinaryWriter writer;

        writer.writeU8(static_cast<std::uint8_t>(status.phase));
        writer.writeU16(static_cast<std::uint16_t>(status.bossHp));
        writer.writeU16(static_cast<std::uint16_t>(status.bossMaxHp));
        writer.writeU32(status.coins);
        writer.writeU32(status.elapsedMs);
        writer.writeU8(status.level);
        writer.writeU8(status.map);
        writer.writeU8(static_cast<std::uint8_t>(status.players.size()));
        for (const PlayerStatus &player : status.players) {
            writer.writeU8(static_cast<std::uint8_t>(player.slot));
            writer.writeU16(static_cast<std::uint16_t>(player.hp));
            writer.writeU8(player.alive ? 1 : 0);
        }
        return writer.data();
    }

    inline std::optional<PlayerStatus> readPlayerStatus(
        Engine::BinaryReader &reader)
    {
        const std::optional<std::uint8_t> slot = reader.readU8();
        const std::optional<std::uint16_t> hp = reader.readU16();
        const std::optional<std::uint8_t> alive = reader.readU8();

        if (!slot || !hp || !alive || *slot >= config::MAX_PLAYERS
            || *alive > 1) {
            return std::nullopt;
        }
        return PlayerStatus{*slot, *hp, *alive == 1};
    }

    struct StatusHeader {
        std::uint8_t phase = 0;
        std::uint16_t bossHp = 0;
        std::uint16_t bossMaxHp = 0;
        std::uint32_t coins = 0;
        std::uint32_t elapsedMs = 0;
        std::uint8_t level = 0;
        std::uint8_t map = 0;
        std::uint8_t count = 0;
    };

    inline std::optional<StatusHeader> readStatusHeader(
        Engine::BinaryReader &reader)
    {
        const std::optional<std::uint8_t> phase = reader.readU8();
        const std::optional<std::uint16_t> bossHp = reader.readU16();
        const std::optional<std::uint16_t> bossMaxHp = reader.readU16();
        const std::optional<std::uint32_t> coins = reader.readU32();
        const std::optional<std::uint32_t> elapsedMs = reader.readU32();
        const std::optional<std::uint8_t> level = reader.readU8();
        const std::optional<std::uint8_t> map = reader.readU8();
        const std::optional<std::uint8_t> count = reader.readU8();

        if (!phase || !bossHp || !bossMaxHp || !coins || !elapsedMs
            || !level || !map || !count) {
            return std::nullopt;
        }
        return StatusHeader{*phase, *bossHp, *bossMaxHp, *coins, *elapsedMs,
            *level, *map, *count};
    }

    inline bool isValid(const StatusHeader &header, const std::size_t left)
    {
        return header.phase <= static_cast<std::uint8_t>(Phase::Lobby)
            && header.map < MAP_COUNT
            && header.count <= config::MAX_PLAYERS
            && header.bossHp <= header.bossMaxHp
            && left == header.count * PLAYER_STATUS_SIZE;
    }

    inline bool readPlayers(Engine::BinaryReader &reader,
        const std::size_t count, std::vector<PlayerStatus> &players)
    {
        for (std::size_t i = 0; i < count; ++i) {
            const std::optional<PlayerStatus> player = readPlayerStatus(reader);

            if (!player) {
                return false;
            }
            players.push_back(*player);
        }
        return true;
    }

    inline std::optional<GameStatus> decodeStatus(const Payload &payload)
    {
        Engine::BinaryReader reader(payload);
        const std::optional<StatusHeader> header = readStatusHeader(reader);
        GameStatus status;

        if (!header || !isValid(*header, reader.remaining())) {
            return std::nullopt;
        }
        status.phase = static_cast<Phase>(header->phase);
        status.bossHp = header->bossHp;
        status.bossMaxHp = header->bossMaxHp;
        status.coins = header->coins;
        status.elapsedMs = header->elapsedMs;
        status.level = header->level;
        status.map = header->map;
        if (!readPlayers(reader, header->count, status.players)) {
            return std::nullopt;
        }
        return status;
    }

    // SELECT_MAP : l'identifiant de la map choisie par l'hote.
    inline Payload encodeSelectMap(const MapId map)
    {
        return {map};
    }

    inline std::optional<MapId> decodeSelectMap(const Payload &payload)
    {
        if (payload.size() != 1 || payload[0] >= MAP_COUNT) {
            return std::nullopt;
        }
        return payload[0];
    }

    // LEADERBOARD : pour chaque map, le nombre de records (1) puis par
    // record le temps en ms (4) et le nombre de joueurs (1).
    inline Payload encode(const AllRecords &records)
    {
        Engine::BinaryWriter writer;

        for (const MapRecords &map : records) {
            writer.writeU8(static_cast<std::uint8_t>(map.size()));
            for (const RunRecord &record : map) {
                writer.writeU32(record.timeMs);
                writer.writeU8(record.players);
            }
        }
        return writer.data();
    }

    inline bool readRecords(Engine::BinaryReader &reader, MapRecords &out)
    {
        const std::optional<std::uint8_t> count = reader.readU8();

        if (!count || *count > MAX_RECORDS
            || reader.remaining() < *count * RECORD_SIZE) {
            return false;
        }
        for (std::size_t i = 0; i < *count; ++i) {
            const std::optional<std::uint32_t> time = reader.readU32();
            const std::optional<std::uint8_t> players = reader.readU8();

            if (!time || !players || *players == 0
                || *players > config::MAX_PLAYERS) {
                return false;
            }
            out.push_back({*time, *players});
        }
        return true;
    }

    inline std::optional<AllRecords> decodeLeaderboard(const Payload &payload)
    {
        Engine::BinaryReader reader(payload);
        AllRecords records;

        for (MapRecords &map : records) {
            if (!readRecords(reader, map)) {
                return std::nullopt;
            }
        }
        if (reader.remaining() != 0) {
            return std::nullopt;
        }
        return records;
    }
}
