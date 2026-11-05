#pragma once

#include <algorithm>
#include <cstddef>
#include <random>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Bullet.hpp"

namespace rtype {
    inline Engine::Entity createPlayer(Engine::Registry &reg, const int slot)
    {
        const auto index = static_cast<std::size_t>(slot);

        if (slot < 0 || index >= config::MAX_PLAYERS) {
            return Engine::Entity{};
        }
        const Engine::Entity e = reg.create();

        reg.add(e, PlayerTag{slot});
        reg.add(e, PlayerInput{});
        reg.add(e, gameLogic::Vec2Pos{{config::PLAYER_START_X,
            config::PLAYER_START_Y[index]}});
        reg.add(e, gameLogic::Vec2Direction{});
        reg.add(e, gameLogic::Vec2Velocity{});
        reg.add(e, gameLogic::Vec2Hitbox{{config::PLAYER_WIDTH,
            config::PLAYER_HEIGHT}});
        reg.add(e, Speed{config::PLAYER_SPEED});
        reg.add(e, Health{config::PLAYER_HP});
        reg.add(e, MaxHealth{config::PLAYER_HP});
        reg.add(e, Weapon{config::PLAYER_FIRE_COOLDOWN});
        reg.add(e, Damage{config::PLAYER_DAMAGE});
        reg.add(e, Team{TeamId::Player});
        reg.add(e, Renderable{SpriteId::Player});
        return e;
    }

    inline Engine::Entity createMonster(Engine::Registry &reg,
        const int slot = 0)
    {
        const Engine::Entity e = reg.create();

        reg.add(e, MonsterTag{slot});
        reg.add(e, gameLogic::Vec2Pos{});
        reg.add(e, gameLogic::Vec2Direction{});
        reg.add(e, gameLogic::Vec2Velocity{});
        reg.add(e, gameLogic::Vec2Hitbox{{config::MONSTER_WIDTH,
            config::MONSTER_HEIGHT}});
        reg.add(e, Speed{config::MONSTER_SPEED});
        reg.add(e, Health{config::MONSTER_HP});
        reg.add(e, Weapon{config::MONSTER_FIRE_COOLDOWN,
            config::MONSTER_FIRE_COOLDOWN});
        reg.add(e, Damage{config::MONSTER_DAMAGE});
        reg.add(e, Team{TeamId::Monster});
        reg.add(e, Renderable{SpriteId::Monster});
        return e;
    }

    // Le boss est un monstre plus gros, avec BossTag pour le reconnaitre.
    inline Engine::Entity createBoss(Engine::Registry &reg)
    {
        const Engine::Entity e = createMonster(reg);

        reg.add(e, BossTag{});
        reg.get<gameLogic::Vec2Hitbox>(e) = gameLogic::Vec2Hitbox{{
            config::BOSS_WIDTH, config::BOSS_HEIGHT}};
        reg.get<Speed>(e) = Speed{config::BOSS_SPEED};
        reg.get<Health>(e) = Health{config::BOSS_HP};
        reg.get<Weapon>(e) = Weapon{config::BOSS_FIRE_COOLDOWN,
            config::BOSS_FIRE_COOLDOWN};
        reg.get<Damage>(e) = Damage{config::BOSS_DAMAGE};
        reg.get<Renderable>(e) = Renderable{SpriteId::Boss};
        return e;
    }

    // Balle tiree par shooter avec la vitesse donnee.
    inline Engine::Entity createBullet(Engine::Registry &reg,
        const Engine::Entity shooter, const gameLogic::Vec2 &velocity)
    {
        if (!isShooter(reg, shooter)) {
            return Engine::Entity{};
        }
        const gameLogic::Vec2Pos pos = reg.get<gameLogic::Vec2Pos>(shooter);
        const gameLogic::Vec2Hitbox box =
            reg.get<gameLogic::Vec2Hitbox>(shooter);
        const Team team = reg.get<Team>(shooter);
        const Damage damage = reg.get<Damage>(shooter);
        const BulletLook look = bulletLook(reg, shooter, team);
        float direction = 1.f;
        const Engine::Entity e = reg.create();

        if (velocity.x < 0.f) {
            direction = -1.f;
        }
        reg.add(e, ProjectileTag{});
        reg.add(e, bulletStartPos(pos, box, look, direction));
        reg.add(e, gameLogic::Vec2Hitbox{{look.width, look.height}});
        reg.add(e, gameLogic::Vec2Velocity{velocity});
        reg.add(e, damage);
        reg.add(e, team);
        reg.add(e, Renderable{look.sprite});
        return e;
    }

    inline Engine::Entity createExplosion(Engine::Registry &reg,
        const gameLogic::Vec2 &center, const float size)
    {
        const Engine::Entity e = reg.create();

        reg.add(e, gameLogic::Vec2Pos{{center.x - size / 2.f,
            center.y - size / 2.f}});
        reg.add(e, gameLogic::Vec2Hitbox{{size, size}});
        reg.add(e, Renderable{SpriteId::Explosion});
        reg.add(e, Lifetime{});
        return e;
    }

    inline float explosionSize(const Engine::Registry &reg, const Engine::Entity e)
    {
        if (!reg.has<gameLogic::Vec2Hitbox>(e)) {
            return 0.f;
        }
        const gameLogic::Vec2Hitbox &box = reg.get<gameLogic::Vec2Hitbox>(e);

        return std::max(box.x, box.y);
    }

    inline Engine::Entity createExplosionOn(Engine::Registry &reg,
        const Engine::Entity target, const float size)
    {
        if (!reg.has<gameLogic::Vec2Pos>(target)
            || !reg.has<gameLogic::Vec2Hitbox>(target)) {
            return Engine::Entity{};
        }
        const gameLogic::Vec2Pos pos = reg.get<gameLogic::Vec2Pos>(target);
        const gameLogic::Vec2Hitbox box = reg.get<gameLogic::Vec2Hitbox>(target);

        return createExplosion(reg, {pos.x + box.x / 2.f, pos.y + box.y / 2.f}, size);
    }

    inline Engine::Entity createCoin(Engine::Registry &reg, std::mt19937 &rng)
    {
        std::uniform_int_distribution<std::size_t> dist(config::COIN_MIN_VALUE,
                                                        config::COIN_MAX_VALUE);
        const Engine::Entity e = reg.create();

        reg.add(e, coins{dist(rng)});
        reg.add(e, gameLogic::Vec2Hitbox{{config::COIN_WIDTH,
            config::COIN_HEIGHT}});
        reg.add(e, gameLogic::Vec2Velocity{{-config::SCROLL_SPEED, 0.f}});
        reg.add(e, Renderable{SpriteId::Coin});
        return e;
    }

    inline Engine::Entity createCoinCounter(Engine::Registry &reg)
    {
        const Engine::Entity e = reg.create();

        reg.add(e, CoinCounter{});
        return e;
    }
}
