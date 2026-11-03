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
#include "Solids.hpp"

namespace rtype {
    using KindTest = bool (*)(ObstacleKind);

    inline bool anyKind(const ObstacleKind)
    {
        return true;
    }

    inline bool isRock(const ObstacleKind kind)
    {
        return kind == ObstacleKind::Rock;
    }

    inline bool isSpike(const ObstacleKind kind)
    {
        return kind == ObstacleKind::Spike;
    }

    inline bool stopsBullets(const ObstacleKind kind)
    {
        return !isPortal(kind);
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
        const std::vector<Collider> spikes = obstacleColliders(reg, isSpike);

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
