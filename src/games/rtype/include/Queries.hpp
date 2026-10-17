#pragma once

#include <array>
#include <cstddef>
#include <optional>
#include <vector>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Components.hpp"
#include "Colisions.hpp"
#include "Config.hpp"


namespace rtype {
    using SlotTable = std::array<bool, config::MAX_PLAYERS>;
    template<typename Tag>
    std::vector<Engine::Entity> collectWith(Engine::Registry &reg)
    {
        std::vector<Engine::Entity> entities;

        reg.each<Tag>([&entities](const Engine::Entity e, const Tag &) {
            entities.push_back(e);
        });
        return entities;
    }

    inline std::vector<Engine::Entity> collectPlayers(Engine::Registry &reg)
    {
        return collectWith<PlayerTag>(reg);
    }

    inline std::vector<Engine::Entity> collectMonsters(Engine::Registry &reg)
    {
        return collectWith<MonsterTag>(reg);
    }

    inline std::vector<Engine::Entity> collectProjectileEntities(
        Engine::Registry &reg)
    {
        return collectWith<ProjectileTag>(reg);
    }


    inline SlotTable collectUsedSlots(Engine::Registry &reg)
    {
        SlotTable used{};

        reg.each<PlayerTag>([&used](Engine::Entity, const PlayerTag &tag) {
            const auto index = static_cast<std::size_t>(tag.slot);

            if (tag.slot >= 0 && index < used.size())
                used[index] = true;
        });
        return used;
    }

    // En gros ça renvoie le premier slot vide OU un nullopt(rien) si la partie est full
    inline std::optional<int> findFreeSlot(Engine::Registry &reg)
    {
        const SlotTable used = collectUsedSlots(reg);

        for (std::size_t i = 0; i < used.size(); ++i) {
            if (!used[i])
                return static_cast<int>(i);
        }
        return std::nullopt;
    }

    // Entite du joueur qui occupe ce slot, ou rien s'il n'y en a pas.
    inline std::optional<Engine::Entity> findPlayerBySlot(
        Engine::Registry &reg, const int slot)
    {
        for (const Engine::Entity e : collectPlayers(reg)) {
            if (reg.get<PlayerTag>(e).slot == slot) {
                return e;
            }
        }
        return std::nullopt;
    }

    inline std::optional<Collider> getCollider(Engine::Registry &reg,
                                               const Engine::Entity e)
    {
        if (!reg.has<gameLogic::Vec2Pos>(e)
            || !reg.has<gameLogic::Vec2Hitbox>(e)
            || !reg.has<Team>(e)) {
            return std::nullopt;
        }
        return Collider{e, reg.get<gameLogic::Vec2Pos>(e),
            reg.get<gameLogic::Vec2Hitbox>(e), reg.get<Team>(e)};
    }
}
