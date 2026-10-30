#pragma once

#include <array>
#include <cstddef>
#include "Components.hpp"
#include "Config.hpp"
#include "Kinds.hpp"

namespace rtype {
    struct MonsterDef {
        int hp;
        float speed;
        int damage;
        float fireCooldown;
        float width;
        float height;
        SpriteId sprite;
    };

    inline constexpr MonsterDef BASIC{config::MONSTER_HP,
        config::MONSTER_SPEED, config::MONSTER_DAMAGE,
        config::MONSTER_FIRE_COOLDOWN, config::MONSTER_WIDTH,
        config::MONSTER_HEIGHT, SpriteId::Monster};

    // Une entree par MonsterKind. Les types pas encore definis reprennent
    // BASIC.
    inline constexpr std::array<MonsterDef,
        static_cast<std::size_t>(MonsterKind::Count)> MONSTERS = {{
        BASIC, BASIC, BASIC, BASIC, BASIC, BASIC}};

    inline const MonsterDef &monsterDef(const MonsterKind kind)
    {
        return MONSTERS[static_cast<std::size_t>(kind)];
    }
}
