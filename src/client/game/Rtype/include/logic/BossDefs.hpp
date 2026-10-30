#pragma once

#include <array>
#include <cstddef>
#include "Components.hpp"
#include "Config.hpp"
#include "Kinds.hpp"
#include "MonsterDefs.hpp"

namespace rtype {
    inline constexpr MonsterDef STANDARD_BOSS{
        .hp = config::BOSS_HP, .speed = config::BOSS_SPEED,
        .damage = config::BOSS_DAMAGE,
        .fireCooldown = config::BOSS_FIRE_COOLDOWN,
        .width = config::BOSS_WIDTH, .height = config::BOSS_HEIGHT,
        .sprite = SpriteId::Boss};

    inline constexpr MonsterDef DASHER_BOSS{
        .hp = 400, .speed = 400.f, .damage = 20, .fireCooldown = 0.5f,
        .width = 120.f, .height = 80.f, .sprite = SpriteId::Boss};

    inline constexpr MonsterDef SUMMONER_BOSS{
        .hp = 500, .speed = 100.f, .damage = 15, .fireCooldown = 1.2f,
        .width = 300.f, .height = 200.f, .sprite = SpriteId::Boss};

    inline constexpr MonsterDef FORTRESS_BOSS{
        .hp = 1200, .speed = 60.f, .damage = 25, .fireCooldown = 1.5f,
        .width = 600.f, .height = 360.f, .sprite = SpriteId::Boss};

    inline constexpr std::array<MonsterDef,
        static_cast<std::size_t>(BossKind::Count)> BOSSES = {{
        STANDARD_BOSS, DASHER_BOSS, SUMMONER_BOSS, FORTRESS_BOSS}};

    inline const MonsterDef &bossDef(const BossKind kind)
    {
        return BOSSES[static_cast<std::size_t>(kind)];
    }
}
