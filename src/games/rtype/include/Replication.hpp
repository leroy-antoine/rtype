#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <vector>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Components.hpp"
#include "Events.hpp"

namespace rtype {
    // Entites connues des clients, rangees par NetId.
    using TrackedTable = std::map<std::uint32_t, Engine::Entity>;

    inline int slotOf(const Engine::Registry &reg, const Engine::Entity e)
    {
        if (reg.has<PlayerTag>(e)) {
            return reg.get<PlayerTag>(e).slot;
        }
        if (reg.has<MonsterTag>(e)) {
            return reg.get<MonsterTag>(e).slot;
        }
        return 0;
    }

    inline bool isReplicable(const Engine::Registry &reg,
        const Engine::Entity e)
    {
        return reg.has<NetId>(e)
            && reg.has<Renderable>(e)
            && reg.has<gameLogic::Vec2Pos>(e)
            && reg.has<gameLogic::Vec2Hitbox>(e);
    }

    inline std::optional<EntitySpawned> describeSpawn(
        const Engine::Registry &reg, const Engine::Entity e)
    {
        if (!isReplicable(reg, e)) {
            return std::nullopt;
        }
        return EntitySpawned{reg.get<NetId>(e).value,
            reg.get<Renderable>(e).id, slotOf(reg, e),
            reg.get<gameLogic::Vec2Pos>(e),
            reg.get<gameLogic::Vec2Hitbox>(e)};
    }

    // Entites affichables qui n'ont pas encore d'identifiant reseau.
    inline std::vector<Engine::Entity> collectUntracked(Engine::Registry &reg)
    {
        std::vector<Engine::Entity> untracked;

        reg.each<Renderable>([&reg, &untracked](const Engine::Entity e,
            const Renderable &) {
            if (!reg.has<NetId>(e)) {
                untracked.push_back(e);
            }
        });
        return untracked;
    }

    // Toutes les entites connues, pour un client qui arrive en cours de jeu.
    inline std::vector<EntitySpawned> collectSpawns(
        const Engine::Registry &reg, const TrackedTable &tracked)
    {
        std::vector<EntitySpawned> spawns;

        for (const auto &[id, e] : tracked) {
            if (const std::optional<EntitySpawned> spawn =
                describeSpawn(reg, e)) {
                spawns.push_back(*spawn);
            }
        }
        return spawns;
    }

    // Seules les entites qui ont une vitesse bougent: les explosions non.
    inline std::vector<EntityState> collectStates(
        const Engine::Registry &reg, const TrackedTable &tracked)
    {
        std::vector<EntityState> states;

        for (const auto &[id, e] : tracked) {
            if (reg.has<gameLogic::Vec2Velocity>(e)
                && reg.has<gameLogic::Vec2Pos>(e)) {
                states.push_back({id, reg.get<gameLogic::Vec2Pos>(e)});
            }
        }
        return states;
    }
}
