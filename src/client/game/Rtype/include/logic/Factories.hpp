#pragma once

#include <algorithm>
#include <cstddef>
#include <random>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "BossDefs.hpp"
#include "Waves.hpp"
#include "MonsterDefs.hpp"
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
        reg.add(e, Charge{});
        reg.add(e, Damage{config::PLAYER_DAMAGE});
        reg.add(e, Team{TeamId::Player});
        reg.add(e, PlayerEffects{});
        reg.add(e, Renderable{SpriteId::Player});
        return e;
    }

    inline void addMonsterStats(Engine::Registry &reg, const Engine::Entity e,
        const MonsterDef &def)
    {
        reg.add(e, gameLogic::Vec2Hitbox{{def.width, def.height}});
        reg.add(e, Speed{def.speed});
        reg.add(e, Health{def.hp});
        reg.add(e, Weapon{def.fireCooldown, def.fireCooldown});
        reg.add(e, Damage{def.damage});
        reg.add(e, Renderable{def.sprite});
    }

    inline Engine::Entity createMonster(Engine::Registry &reg,
        const MonsterKind kind = MonsterKind::Basic, const int slot = 0)
    {
        const Engine::Entity e = reg.create();

        reg.add(e, MonsterTag{slot});
        reg.add(e, MonsterType{kind});
        reg.add(e, MonsterState{});
        reg.add(e, gameLogic::Vec2Pos{});
        reg.add(e, gameLogic::Vec2Direction{});
        reg.add(e, gameLogic::Vec2Velocity{});
        reg.add(e, Team{TeamId::Monster});
        addMonsterStats(reg, e, monsterDef(kind));
        return e;
    }

    // Le boss est un monstre plus gros, avec BossTag pour le reconnaitre.
    inline void setBossStats(Engine::Registry &reg, const Engine::Entity e,
        const MonsterDef &def)
    {
        reg.get<gameLogic::Vec2Hitbox>(e) =
            gameLogic::Vec2Hitbox{{def.width, def.height}};
        reg.get<Speed>(e) = Speed{def.speed};
        reg.get<Health>(e) = Health{def.hp};
        reg.get<Weapon>(e) = Weapon{def.fireCooldown, def.fireCooldown};
        reg.get<Damage>(e) = Damage{def.damage};
        reg.get<Renderable>(e) = Renderable{def.sprite};
    }

    inline Engine::Entity createBoss(Engine::Registry &reg,
        const BossKind kind = BossKind::Standard)
    {
        const Engine::Entity e = createMonster(reg);

        reg.add(e, BossTag{});
        reg.add(e, BossType{kind});
        if (kind == BossKind::Fortress) {
            reg.add(e, LeviathanState{});
        }
        setBossStats(reg, e, bossDef(kind));
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

        const float size = std::max(box.x, box.y);

        if (reg.has<BossTag>(e)) {
            return std::max(size, config::BOSS_EXPLOSION_MIN_SIZE);
        }
        return size;
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
        reg.add(e, Renderable{SpriteId::Coin});
        return e;
    }

    inline SpriteId obstacleSprite(const ObstacleKind kind)
    {
        switch (kind) {
            case ObstacleKind::Spike: return SpriteId::SpikeWall;
            case ObstacleKind::ReversePortal: return SpriteId::ReversePortal;
            case ObstacleKind::FastPortal: return SpriteId::FastPortal;
            case ObstacleKind::NormalPortal: return SpriteId::NormalPortal;
            case ObstacleKind::Laser: return SpriteId::LaserPost;
            case ObstacleKind::Turret: return SpriteId::TurretBlock;
            case ObstacleKind::Mine: return SpriteId::MineBall;
            case ObstacleKind::Beam: return SpriteId::BeamOn;
            case ObstacleKind::BeamWarn: return SpriteId::BeamWarn;
            default: return SpriteId::RockWall;
        }
    }

    inline void addObstacleMotion(Engine::Registry &reg, const Engine::Entity e,
        const ObstacleStep &step, const float age = 0.f)
    {
        constexpr float TWO_PI = 6.2831853f;

        if (step.amplitude != 0.f && step.period > 0.f) {
            reg.add(e, ObstacleMotion{step.y, step.amplitude,
                TWO_PI / step.period, age});
        }
    }

    inline Engine::Entity createObstacle(Engine::Registry &reg,
        const ObstacleStep &step, const float overshoot = 0.f)
    {
        const Engine::Entity e = reg.create();
        const float age = overshoot / config::SCROLL_SPEED;

        reg.add(e, ObstacleTag{step.kind});
        reg.add(e, gameLogic::Vec2Pos{{config::MAP_WIDTH
            - overshoot * step.speed, step.y}});
        reg.add(e, gameLogic::Vec2Hitbox{{step.width, step.height}});
        reg.add(e, gameLogic::Vec2Velocity{{
            -config::OBSTACLE_SPEED * step.speed, 0.f}});
        reg.add(e, Renderable{obstacleSprite(step.kind)});
        addObstacleMotion(reg, e, step, age);
        if (step.kind == ObstacleKind::Laser) {
            reg.add(e, LaserCycle{step.period + age});
        }
        if (step.kind == ObstacleKind::Turret) {
            reg.add(e, TurretFire{config::TURRET_INTERVAL,
                config::TURRET_INTERVAL});
        }
        return e;
    }

    inline Engine::Entity createCoinCounter(Engine::Registry &reg)
    {
        const Engine::Entity e = reg.create();

        reg.add(e, CoinCounter{});
        return e;
    }
}
