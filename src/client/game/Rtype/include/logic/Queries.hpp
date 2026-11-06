#pragma once

#include <array>
#include <cmath>
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

    inline std::optional<gameLogic::Vec2> nearestPlayerCenter(
        Engine::Registry &reg, const gameLogic::Vec2 &from)
    {
        std::optional<gameLogic::Vec2> best;
        float bestDistance = 0.f;

        for (const Engine::Entity e : collectPlayers(reg)) {
            const gameLogic::Vec2Pos pos = reg.get<gameLogic::Vec2Pos>(e);
            const gameLogic::Vec2Hitbox box = reg.get<gameLogic::Vec2Hitbox>(e);
            const gameLogic::Vec2 center{pos.x + box.x / 2.f,
                pos.y + box.y / 2.f};
            const float distance = std::hypot(center.x - from.x,
                center.y - from.y);

            if (!best || distance < bestDistance) {
                best = center;
                bestDistance = distance;
            }
        }
        return best;
    }

    inline std::vector<Engine::Entity> collectObstacles(Engine::Registry &reg)
    {
        return collectWith<ObstacleTag>(reg);
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


    // En gros ça renvoie le premier slot vide OU un nullopt(rien) si la partie est full
    inline std::optional<int> findFreeSlot(const SlotTable &used)
    {
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

    inline std::optional<Collider> getCollider(const Engine::Registry &reg,
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

    inline std::vector<Collider> collectColliders(const Engine::Registry &reg,
        const std::vector<Engine::Entity> &entities)
    {
        std::vector<Collider> colliders;

        for (const Engine::Entity e : entities) {
            if (const std::optional<Collider> collider = getCollider(reg, e)) {
                colliders.push_back(*collider);
            }
        }
        return colliders;
    }
}
