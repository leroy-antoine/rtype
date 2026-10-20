#pragma once

#include <cstdint>
#include <variant>
#include "utils/Vectors.hpp"
#include "Components.hpp"

namespace rtype {
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

    struct EntityState {
        std::uint32_t id = 0;
        gameLogic::Vec2Pos pos;
    };

    using GameEvent = std::variant<EntitySpawned, EntityDestroyed>;
}
