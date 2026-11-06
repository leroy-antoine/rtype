#pragma once

#include <cmath>
#include <vector>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Collectibles.hpp"
#include "Colisions.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Factories.hpp"
#include "Queries.hpp"
#include "MonsterBehavior.hpp"
#include "Solids.hpp"

namespace rtype {
    using KindTest = bool (*)(ObstacleKind);

    inline bool anyKind(const ObstacleKind)
    {
        return true;
    }

    inline bool isRock(const ObstacleKind kind)
    {
        return kind == ObstacleKind::Rock || kind == ObstacleKind::Turret;
    }

    // Un mur est un obstacle physique : un monstre ne peut pas y naitre.
    inline bool isWall(const ObstacleKind kind)
    {
        return kind == ObstacleKind::Rock || kind == ObstacleKind::Spike
            || kind == ObstacleKind::Mine || kind == ObstacleKind::Turret;
    }

    inline bool stopsBullets(const ObstacleKind kind)
    {
        return !isPortal(kind) && kind != ObstacleKind::Laser
            && kind != ObstacleKind::Beam && kind != ObstacleKind::BeamWarn;
    }

    inline bool isDeadly(const ObstacleKind kind)
    {
        return kind == ObstacleKind::Spike || kind == ObstacleKind::Mine
            || kind == ObstacleKind::Beam;
    }

    inline std::vector<Collider> deadlyColliders(Engine::Registry &reg)
    {
        std::vector<Collider> found;

        reg.each<ObstacleTag, gameLogic::Vec2Pos, gameLogic::Vec2Hitbox>(
            [&found](const Engine::Entity e, const ObstacleTag &tag,
                const gameLogic::Vec2Pos &pos,
                const gameLogic::Vec2Hitbox &box) {
                if (isDeadly(tag.kind)) {
                    found.push_back({e, pos, box, Team{}});
                }
            });
        return found;
    }

    inline std::vector<Collider> obstacleColliders(Engine::Registry &reg,
        const KindTest test = anyKind)
    {
        std::vector<Collider> found;

        reg.each<ObstacleTag, gameLogic::Vec2Pos, gameLogic::Vec2Hitbox>(
            [&found, test](const Engine::Entity e, const ObstacleTag &tag,
                const gameLogic::Vec2Pos &pos,
                const gameLogic::Vec2Hitbox &box) {
                if (test(tag.kind)) {
                    found.push_back({e, pos, box, Team{}});
                }
            });
        return found;
    }

    inline void obstacleMotionSystem(Engine::Registry &reg, const float dt)
    {
        reg.each<ObstacleMotion, gameLogic::Vec2Pos>(
            [dt](const Engine::Entity, ObstacleMotion &motion,
                gameLogic::Vec2Pos &pos) {
                motion.age += dt;
                pos.y = motion.baseY
                    + motion.amplitude * std::sin(motion.omega * motion.age);
            });
    }

    inline void spawnTurretShot(Engine::Registry &reg,
        const gameLogic::Vec2 &from, const gameLogic::Vec2 &direction)
    {
        const float size = config::TURRET_SHOT_SIZE;
        const float muzzle = config::TURRET_MUZZLE;
        const Engine::Entity shot = reg.create();

        reg.add(shot, ProjectileTag{});
        reg.add(shot, gameLogic::Vec2Pos{{
            from.x + direction.x * muzzle - size / 2.f,
            from.y + direction.y * muzzle - size / 2.f}});
        reg.add(shot, gameLogic::Vec2Hitbox{{size, size}});
        reg.add(shot, gameLogic::Vec2Velocity{{
            direction.x * config::TURRET_SHOT_SPEED,
            direction.y * config::TURRET_SHOT_SPEED}});
        reg.add(shot, Damage{config::TURRET_DAMAGE});
        reg.add(shot, Team{TeamId::Monster});
        reg.add(shot, Renderable{SpriteId::MonsterBullet});
    }

    // Le tir part hors de la tourelle, sinon son propre bloc l absorberait.
    inline void fireTurret(Engine::Registry &reg, const Engine::Entity turret)
    {
        const gameLogic::Vec2 from = centerOf(reg, turret);
        const auto target = nearestPlayerCenter(reg, from);

        if (!target || from.x > config::MAP_WIDTH - 100.f) {
            return;
        }
        const float dx = target->x - from.x;
        const float dy = target->y - from.y;
        const float length = std::max(1.f, std::hypot(dx, dy));

        spawnTurretShot(reg, from, {dx / length, dy / length});
    }

    inline ObstacleKind beamKind(const float age)
    {
        const float phase = std::fmod(age, config::LASER_PERIOD);

        if (phase < config::LASER_OFF) {
            return ObstacleKind::Laser;
        }
        if (phase < config::LASER_OFF + config::LASER_WARN) {
            return ObstacleKind::BeamWarn;
        }
        return ObstacleKind::Beam;
    }

    inline Engine::Entity spawnBeam(Engine::Registry &reg,
        const Engine::Entity post, const ObstacleKind kind)
    {
        const gameLogic::Vec2Pos pos = reg.get<gameLogic::Vec2Pos>(post);
        const gameLogic::Vec2Hitbox box =
            reg.get<gameLogic::Vec2Hitbox>(post);
        const Engine::Entity beam = createObstacle(reg,
            {0.f, pos.y, box.x, box.y, kind});

        reg.get<gameLogic::Vec2Pos>(beam).x = pos.x;
        reg.get<gameLogic::Vec2Velocity>(beam) =
            reg.get<gameLogic::Vec2Velocity>(post);
        return beam;
    }

    inline void updateBeam(Engine::Registry &reg, const Engine::Entity post)
    {
        const ObstacleKind wanted = beamKind(reg.get<LaserCycle>(post).age);
        const LaserCycle cycle = reg.get<LaserCycle>(post);
        const bool same = cycle.hasBeam && reg.alive(cycle.beam)
            && reg.get<ObstacleTag>(cycle.beam).kind == wanted;

        if (same || (!cycle.hasBeam && wanted == ObstacleKind::Laser)) {
            return;
        }
        if (cycle.hasBeam && reg.alive(cycle.beam)) {
            reg.destroy(cycle.beam);
        }
        LaserCycle &next = reg.get<LaserCycle>(post);

        next.hasBeam = wanted != ObstacleKind::Laser;
        if (next.hasBeam) {
            const Engine::Entity beam = spawnBeam(reg, post, wanted);

            reg.get<LaserCycle>(post).beam = beam;
        }
    }

    inline void laserSystem(Engine::Registry &reg, const float dt)
    {
        std::vector<Engine::Entity> posts;

        reg.each<LaserCycle>([&posts, dt](const Engine::Entity e,
            LaserCycle &cycle) {
            cycle.age += dt;
            posts.push_back(e);
        });
        for (const Engine::Entity post : posts) {
            updateBeam(reg, post);
        }
    }

    inline void turretSystem(Engine::Registry &reg, const float dt)
    {
        std::vector<Engine::Entity> ready;

        reg.each<TurretFire>([&ready, dt](const Engine::Entity e,
            TurretFire &fire) {
            fire.timer -= dt;
            if (fire.timer <= 0.f) {
                fire.timer = fire.interval;
                ready.push_back(e);
            }
        });
        for (const Engine::Entity turret : ready) {
            fireTurret(reg, turret);
        }
    }

    inline void pushPlayers(Engine::Registry &reg)
    {
        const std::vector<Collider> rocks = obstacleColliders(reg, isRock);

        for (const Collider &player : collectColliders(reg,
            collectPlayers(reg))) {
            reg.get<gameLogic::Vec2Pos>(player.entity) =
                resolveSolids(player, rocks);
        }
    }

    inline void killOnSpikes(Engine::Registry &reg)
    {
        const std::vector<Collider> spikes = deadlyColliders(reg);

        for (const Collider &player : collectColliders(reg,
            collectPlayers(reg))) {
            if (isAlive(reg, player.entity) && touchesAny(player, spikes)) {
                reg.get<Health>(player.entity).hp = 0;
            }
        }
    }

    inline void absorbProjectiles(Engine::Registry &reg)
    {
        const std::vector<Collider> blocks =
            obstacleColliders(reg, stopsBullets);

        for (const Collider &bullet : collectProjectiles(reg)) {
            if (touchesAny(bullet, blocks)) {
                createExplosionOn(reg, bullet.entity,
                    config::HIT_EXPLOSION_SIZE);
                reg.destroy(bullet.entity);
            }
        }
    }

    inline void applyPortal(PlayerEffects &effects, const ObstacleKind kind)
    {
        if (kind == ObstacleKind::ReversePortal) {
            effects.inverted = !effects.inverted;
        } else if (kind == ObstacleKind::FastPortal) {
            effects.speedFactor = config::FAST_SPEED_FACTOR;
        } else if (kind == ObstacleKind::NormalPortal) {
            effects.speedFactor = 1.f;
        }
    }

    inline void crossPortals(Engine::Registry &reg)
    {
        const std::vector<Collider> portals = obstacleColliders(reg, isPortal);

        for (const Collider &player : collectColliders(reg,
            collectPlayers(reg))) {
            for (const Collider &portal : portals) {
                PlayerEffects &effects =
                    reg.get<PlayerEffects>(player.entity);

                if (overlaps(player, portal)
                    && effects.lastPortal != portal.entity) {
                    effects.lastPortal = portal.entity;
                    applyPortal(effects, reg.get<ObstacleTag>(
                        portal.entity).kind);
                }
            }
        }
    }

    inline void obstacleSystem(Engine::Registry &reg)
    {
        pushPlayers(reg);
        killOnSpikes(reg);
        crossPortals(reg);
        absorbProjectiles(reg);
    }

    inline void destroyFarObstacles(Engine::Registry &reg)
    {
        for (const Collider &block : obstacleColliders(reg)) {
            if (isFarOutOfBounds(block)) {
                reg.destroy(block.entity);
            }
        }
    }
}
