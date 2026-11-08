#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <optional>
#include <vector>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Colisions.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Factories.hpp"
#include "Queries.hpp"

namespace rtype {
    // Les niveaux 2 et 3 tirent plus vite, le niveau 3 tire deux balles.
    inline constexpr std::array<float, config::DRONE_MAX_LEVEL>
        DRONE_COOLDOWNS = {0.6f, 0.45f, 0.45f};

    inline int shotsOf(const int level)
    {
        if (level >= config::DRONE_MAX_LEVEL) {
            return 2;
        }
        return 1;
    }

    inline float cooldownOf(const int level)
    {
        const int index = std::clamp(level, 1, config::DRONE_MAX_LEVEL) - 1;

        return DRONE_COOLDOWNS[static_cast<std::size_t>(index)];
    }

    inline gameLogic::Vec2 rotated(const gameLogic::Vec2 &v, const float angle)
    {
        return {v.x * std::cos(angle) - v.y * std::sin(angle),
            v.x * std::sin(angle) + v.y * std::cos(angle)};
    }

    inline gameLogic::Vec2Pos dronePerch(Engine::Registry &reg,
        const Engine::Entity owner)
    {
        const gameLogic::Vec2Pos pos = reg.get<gameLogic::Vec2Pos>(owner);

        const float x = pos.x + config::PLAYER_WIDTH + config::DRONE_OFFSET_X;

        return gameLogic::Vec2Pos{{std::min(x,
            config::MAP_WIDTH - config::DRONE_SIZE),
            std::max(0.f, pos.y - config::DRONE_OFFSET_Y)}};
    }

    inline Engine::Entity createDrone(Engine::Registry &reg,
        const Engine::Entity owner)
    {
        const Engine::Entity drone = reg.create();

        reg.add(drone, dronePerch(reg, owner));
        reg.add(drone, gameLogic::Vec2Hitbox{{config::DRONE_SIZE,
            config::DRONE_SIZE}});
        reg.add(drone, gameLogic::Vec2Velocity{});
        reg.add(drone, Team{TeamId::Player});
        reg.add(drone, Damage{1});
        reg.add(drone, Renderable{SpriteId::PlayerDrone});
        reg.add(drone, DroneBody{owner, 0.f});
        return drone;
    }

    // Le drone rejoint sa place a cote du vaisseau avec un leger retard.
    inline void followOwner(Engine::Registry &reg, const Engine::Entity drone,
        const float dt)
    {
        const gameLogic::Vec2Pos perch = dronePerch(reg,
            reg.get<DroneBody>(drone).owner);
        gameLogic::Vec2Pos &pos = reg.get<gameLogic::Vec2Pos>(drone);
        const float pull = std::min(1.f, config::DRONE_FOLLOW * dt);

        pos = gameLogic::Vec2Pos{{pos.x + (perch.x - pos.x) * pull,
            pos.y + (perch.y - pos.y) * pull}};
    }

    inline std::optional<Collider> nearestEnemy(Engine::Registry &reg,
        const gameLogic::Vec2 &from)
    {
        std::optional<Collider> best;
        float bestDistance = config::DRONE_RANGE;

        for (const Collider &enemy : livingEnemies(reg)) {
            const gameLogic::Vec2 at = centerOfBox(enemy);
            const float distance = std::hypot(at.x - from.x, at.y - from.y);

            if (distance <= bestDistance) {
                best = enemy;
                bestDistance = distance;
            }
        }
        return best;
    }

    inline void droneFire(Engine::Registry &reg, const Engine::Entity drone,
        const int level, const gameLogic::Vec2 &aim)
    {
        const float speed = config::BULLET_SPEED;
        const float length = std::max(std::hypot(aim.x, aim.y), 1.f);
        const gameLogic::Vec2 direction{aim.x / length * speed,
            aim.y / length * speed};
        const int shots = shotsOf(level);

        for (int i = 0; i < shots; ++i) {
            const float side = static_cast<float>(i) - (shots - 1) / 2.f;

            createBullet(reg, drone, rotated(direction,
                side * 2.f * config::DRONE_SPREAD));
        }
    }

    // Le drone tire seul sur l'ennemi le plus proche, avec la moitie des
    // degats de son proprietaire.
    inline void updateDrone(Engine::Registry &reg, const Engine::Entity drone,
        const float dt)
    {
        DroneBody &body = reg.get<DroneBody>(drone);
        const int level = reg.get<DroneLevel>(body.owner).level;
        const Collider self{drone, reg.get<gameLogic::Vec2Pos>(drone),
            reg.get<gameLogic::Vec2Hitbox>(drone), Team{}};
        const gameLogic::Vec2 here = centerOfBox(self);

        followOwner(reg, drone, dt);
        body.timer = std::max(0.f, body.timer - dt);
        const auto target = nearestEnemy(reg, here);

        if (body.timer > 0.f || !target) {
            return;
        }
        reg.get<Damage>(drone).value = std::max(1,
            reg.get<Damage>(body.owner).value / config::DRONE_DAMAGE_DIVISOR);
        droneFire(reg, drone, level, {centerOfBox(*target).x - here.x,
            centerOfBox(*target).y - here.y});
        body.timer = cooldownOf(level);
    }

    inline bool hasDrone(Engine::Registry &reg, const Engine::Entity owner)
    {
        bool found = false;

        reg.each<DroneBody>([&](const Engine::Entity, const DroneBody &body) {
            found = found || body.owner == owner;
        });
        return found;
    }

    // Au shop, les drones suivent leur vaisseau sans tirer.
    inline void droneFollowSystem(Engine::Registry &reg, const float dt)
    {
        for (const Engine::Entity drone : collectWith<DroneBody>(reg)) {
            if (reg.alive(reg.get<DroneBody>(drone).owner)) {
                followOwner(reg, drone, dt);
            }
        }
    }

    // Un drone par joueur equipe, detruit avec son proprietaire.
    inline void droneSystem(Engine::Registry &reg, const float dt)
    {
        for (const Engine::Entity drone : collectWith<DroneBody>(reg)) {
            if (!reg.alive(reg.get<DroneBody>(drone).owner)) {
                reg.destroy(drone);
            }
        }
        for (const Engine::Entity owner : collectWith<DroneLevel>(reg)) {
            if (!hasDrone(reg, owner)) {
                createDrone(reg, owner);
            }
        }
        for (const Engine::Entity drone : collectWith<DroneBody>(reg)) {
            updateDrone(reg, drone, dt);
        }
    }
}
