#pragma once

#include <array>
#include <cstddef>
#include "Components.hpp"
#include "Config.hpp"
#include "Kinds.hpp"

namespace rtype {
    struct ProjectileDef {
        SpriteId sprite;
        float width;
        float height;
        float speed;
        int shots;
        float spread;
        bool drop = false;
    };

    struct MonsterDef {
        int hp;
        float speed;
        int damage;
        float fireCooldown;
        float width;
        float height;
        SpriteId sprite;
        ProjectileDef projectile;
    };

    inline constexpr MonsterDef BASIC{config::MONSTER_HP,
        config::MONSTER_SPEED, config::MONSTER_DAMAGE,
        config::MONSTER_FIRE_COOLDOWN, config::MONSTER_WIDTH,
        config::MONSTER_HEIGHT, SpriteId::Monster,
        {SpriteId::MonsterBullet, config::BULLET_WIDTH, config::BULLET_HEIGHT,
            config::BULLET_SPEED, 1, 0.f}};

    inline constexpr MonsterDef TANK{
        .hp = 120, .speed = 120.f, .damage = 20, .fireCooldown = 3.f,
        .width = 169.f, .height = 130.f, .sprite = SpriteId::Tank,
        .projectile = {SpriteId::MonsterBullet, 40.f, 28.f, 350.f, 1, 0.f}};

    inline constexpr MonsterDef KAMIKAZE{
        .hp = 15, .speed = 450.f, .damage = 30, .fireCooldown = 60.f,
        .width = 64.f, .height = 40.f, .sprite = SpriteId::Kamikaze,
        .projectile = BASIC.projectile};

    inline constexpr MonsterDef ELITE{
        .hp = 60, .speed = 280.f, .damage = 15, .fireCooldown = 1.2f,
        .width = 100.f, .height = 100.f, .sprite = SpriteId::Elite,
        .projectile = {SpriteId::MonsterBullet, 22.f, 15.f, 1000.f, 2, 0.15f}};

    inline constexpr MonsterDef ROCKET{
        .hp = 45, .speed = 140.f, .damage = 35, .fireCooldown = 3.5f,
        .width = 105.f, .height = 108.f, .sprite = SpriteId::Rocket,
        .projectile = {SpriteId::RocketMissile, 70.f, 28.f, 650.f, 1, 0.f}};

    inline constexpr MonsterDef BOMBER{
        .hp = 150, .speed = 90.f, .damage = 25, .fireCooldown = 2.5f,
        .width = 96.f, .height = 195.f, .sprite = SpriteId::Bomber,
        .projectile = {SpriteId::Bomb, 40.f, 33.f, 220.f, 1, 0.f, true}};

    // Un mini-boss : un monstre normal en plus gros, avec le triple de vie.
    constexpr MonsterDef strengthened(MonsterDef base)
    {
        base.hp *= config::MINI_HP_FACTOR;
        base.damage = static_cast<int>(static_cast<float>(base.damage)
            * config::MINI_DAMAGE_FACTOR);
        base.fireCooldown *= config::MINI_COOLDOWN_FACTOR;
        base.width *= config::MINI_SIZE_FACTOR;
        base.height *= config::MINI_SIZE_FACTOR;
        base.projectile.shots += 1;
        return base;
    }

    inline constexpr std::array<MonsterDef,
        static_cast<std::size_t>(MonsterKind::Count)> MONSTERS = {{
        BASIC, TANK, KAMIKAZE, ELITE, ROCKET, BOMBER, strengthened(TANK),
        strengthened(ELITE), strengthened(BOMBER)}};

    inline const MonsterDef &monsterDef(const MonsterKind kind)
    {
        return MONSTERS[static_cast<std::size_t>(kind)];
    }
}
