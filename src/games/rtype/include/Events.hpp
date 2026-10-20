#pragma once

#include <cstdint>
#include <variant>
#include "utils/Vectors.hpp"
#include "Components.hpp"

namespace rtype {
    // Une entite affichable vient d'apparaitre (joueur, monstre, balle, ...).
    struct EntitySpawned {
        std::uint32_t id = 0;
        SpriteId sprite = SpriteId::Player;
        int slot = 0;
        gameLogic::Vec2Pos pos;
        gameLogic::Vec2Hitbox box;
    };

    struct EntityDestroyed {
        std::uint32_t id = 0;
    };

    // Position d'une entite qui bouge, a la fin d'un tick.
    struct EntityState {
        std::uint32_t id = 0;
        gameLogic::Vec2Pos pos;
    };

    using GameEvent = std::variant<EntitySpawned, EntityDestroyed>;
}
