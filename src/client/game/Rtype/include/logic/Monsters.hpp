#pragma once
#include <algorithm>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Config.hpp"
#include "Factories.hpp"

namespace rtype {
    // Fait apparaitre un monstre a droite de la carte, a la hauteur y donnee,
    // et le lance vers la gauche.
    inline Engine::Entity spawnMonster(Engine::Registry &reg, const float y,
        const MonsterKind kind = MonsterKind::Basic)
    {
        const Engine::Entity e = createMonster(reg, kind);
        constexpr float x = config::MAP_WIDTH + config::SPAWN_MARGIN;
        const float maxY = config::MAP_HEIGHT
            - reg.get<gameLogic::Vec2Hitbox>(e).y;

        reg.get<gameLogic::Vec2Pos>(e) =
            gameLogic::Vec2Pos{{x, std::clamp(y, config::MAP_ORIGIN, maxY)}};
        reg.get<gameLogic::Vec2Velocity>(e) =
            gameLogic::Vec2Velocity{{-reg.get<Speed>(e).value, 0.f}};
        return e;
    }

    // Le boss entre par la droite, centre sur la hauteur de la carte.
    inline Engine::Entity spawnBoss(Engine::Registry &reg)
    {
        const Engine::Entity e = createBoss(reg);
        constexpr float x = config::MAP_WIDTH + config::SPAWN_MARGIN;

        reg.get<gameLogic::Vec2Pos>(e) = gameLogic::Vec2Pos{{x,
            (config::MAP_HEIGHT - config::BOSS_HEIGHT) / 2.f}};
        reg.get<gameLogic::Vec2Velocity>(e) =
            gameLogic::Vec2Velocity{{-config::BOSS_SPEED, 0.f}};
        return e;
    }
}
