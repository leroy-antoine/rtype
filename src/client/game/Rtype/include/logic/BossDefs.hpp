#pragma once

#include <array>
#include <cstddef>
#include "engine/core/Registry.hpp"
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
        .sprite = SpriteId::Boss,
        .projectile = {SpriteId::MonsterBullet, config::BOSS_BULLET_SIZE,
            config::BOSS_BULLET_SIZE, config::BOSS_BULLET_SPEED,
            config::BOSS_FAN_BULLETS, config::BOSS_FAN_ANGLE}};

    inline constexpr MonsterDef DASHER_BOSS{
        .hp = 280, .speed = 400.f, .damage = 4, .fireCooldown = 1.4f,
        .width = 147.f, .height = 110.f, .sprite = SpriteId::DasherBoss,
        .projectile = {SpriteId::AlienShard, 30.f, 10.f, 520.f, 9, 0.16f}};

    inline constexpr MonsterDef SUMMONER_BOSS{
        .hp = 500, .speed = 100.f, .damage = 10, .fireCooldown = 1.2f,
        .width = 240.f, .height = 240.f, .sprite = SpriteId::SummonerBoss,
        .projectile = {SpriteId::MonsterBullet, 32.f, 32.f, 420.f, 3, 0.25f}};

    inline constexpr MonsterDef FORTRESS_BOSS{
        .hp = 2400, .speed = 200.f, .damage = 12, .fireCooldown = 1.6f,
        .width = 480.f, .height = 320.f, .sprite = SpriteId::FortressBoss,
        .projectile = {SpriteId::LeviathanBolt, 72.f, 24.f, 380.f, 9, 0.18f}};

    inline constexpr std::array<MonsterDef,
        static_cast<std::size_t>(BossKind::Count)> BOSSES = {{
        STANDARD_BOSS, DASHER_BOSS, SUMMONER_BOSS, FORTRESS_BOSS}};

    inline const MonsterDef &bossDef(const BossKind kind)
    {
        return BOSSES[static_cast<std::size_t>(kind)];
    }

    // La vie maximale du boss, difficulte comprise.
    inline int bossMaxHp(const Engine::Registry &reg, const Engine::Entity e)
    {
        if (reg.has<MaxHealth>(e)) {
            return reg.get<MaxHealth>(e).hp;
        }
        return bossDef(reg.get<BossType>(e).kind).hp;
    }
}
