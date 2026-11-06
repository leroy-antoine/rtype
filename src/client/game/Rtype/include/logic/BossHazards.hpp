#pragma once

#include <algorithm>
#include <cmath>
#include <random>
#include "engine/core/Registry.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "DeathRuns.hpp"
#include "Factories.hpp"

namespace rtype {
    // Pendant un combat de boss le niveau ne defile plus : des lasers et une
    // pluie de meteorites arrivent au rythme du tempo, de plus en plus vite
    // quand l'intensite (0 a 1) monte.
    struct HazardClock {
        float time = 0.f;
        float laser = 2.f;
        float meteor = 1.f;
    };

    inline float hazardInterval(const float slow, const float fast, const float amount)
    {
        return slow + (fast - slow) * std::clamp(amount, 0.f, 1.f);
    }

    inline void spawnBossLaser(Engine::Registry &reg, const float time)
    {
        ObstacleStep step = laser(0.f);

        step.period = std::fmod(time, config::LASER_PERIOD);
        createObstacle(reg, step);
    }

    // Une petite boule rapide qui entre par le haut ou par la droite et
    // traverse l'ecran en diagonale.
    inline void spawnMeteor(Engine::Registry &reg, std::mt19937 &rng)
    {
        const float size = config::METEOR_SIZE;
        const float angle = 0.5f + static_cast<float>(rng() % 25) / 100.f;
        const float speed = config::METEOR_SPEED;
        float x = config::MAP_WIDTH + 10.f;
        float y = static_cast<float>(rng() % 600);
        const Engine::Entity meteor = reg.create();

        if (rng() % 2 == 0) {
            x = 500.f + static_cast<float>(rng() % 1400);
            y = -size - 10.f;
        }
        reg.add(meteor, ProjectileTag{});
        reg.add(meteor, gameLogic::Vec2Pos{{x, y}});
        reg.add(meteor, gameLogic::Vec2Hitbox{{size, size}});
        reg.add(meteor, gameLogic::Vec2Velocity{{-std::cos(angle) * speed,
            std::sin(angle) * speed}});
        reg.add(meteor, Damage{config::METEOR_DAMAGE});
        reg.add(meteor, Team{TeamId::Monster});
        reg.add(meteor, Renderable{SpriteId::Meteor});
    }

    inline void bossHazardSystem(Engine::Registry &reg, HazardClock &clock,
        std::mt19937 &rng, const float intensity, const float dt)
    {
        clock.time += dt;
        clock.laser -= dt;
        clock.meteor -= dt;
        if (clock.laser <= 0.f) {
            spawnBossLaser(reg, clock.time);
            clock.laser = hazardInterval(config::HAZARD_LASER_SLOW,
                config::HAZARD_LASER_FAST, intensity);
        }
        if (clock.meteor <= 0.f) {
            spawnMeteor(reg, rng);
            clock.meteor = hazardInterval(config::HAZARD_METEOR_SLOW,
                config::HAZARD_METEOR_FAST, intensity);
        }
    }
}
