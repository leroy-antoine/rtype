#pragma once

#include <cstddef>
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
        reg.add(e, Weapon{config::PLAYER_FIRE_COOLDOWN});
        reg.add(e, Damage{config::PLAYER_DAMAGE});
        reg.add(e, Team{TeamId::Player});
        reg.add(e, Renderable{SpriteId::Player});
        return e;
    }

    inline Engine::Entity createMonster(Engine::Registry &reg, const int slot)
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

    inline Engine::Entity createBullet(Engine::Registry &reg,
        const Engine::Entity shooter)
    {
        if (!isShooter(reg, shooter)) {
            return Engine::Entity{};
        }
        const gameLogic::Vec2Pos pos = reg.get<gameLogic::Vec2Pos>(shooter);
        const gameLogic::Vec2Hitbox box =
            reg.get<gameLogic::Vec2Hitbox>(shooter);
        const Team team = reg.get<Team>(shooter);
        const Damage damage = reg.get<Damage>(shooter);
        const float direction = bulletDirection(team);
        const Engine::Entity e = reg.create();

        reg.add(e, ProjectileTag{});
        reg.add(e, bulletStartPos(pos, box, direction));
        reg.add(e, gameLogic::Vec2Hitbox{{config::BULLET_WIDTH,
            config::BULLET_HEIGHT}});
        reg.add(e, gameLogic::Vec2Velocity{{direction * config::BULLET_SPEED,
            0.f}});
        reg.add(e, damage);
        reg.add(e, team);
        reg.add(e, Renderable{team.id == TeamId::Player
            ? SpriteId::PlayerBullet : SpriteId::MonsterBullet});
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
}
