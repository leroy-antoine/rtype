#pragma once

#include <optional>
#include <vector>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Components.hpp"
#include "Colisions.hpp"


namespace rtype {
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
