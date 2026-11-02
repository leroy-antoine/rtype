#pragma once
#include <algorithm>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Config.hpp"
#include "Factories.hpp"

namespace rtype {
    // Place un monstre en (x, y), la hauteur etant ramenee dans la carte.
    inline Engine::Entity spawnMonsterAt(Engine::Registry &reg, const float x,
        const float y, const MonsterKind kind)
    {
        const Engine::Entity e = createMonster(reg, kind);
        const float maxY = config::MAP_HEIGHT
            - reg.get<gameLogic::Vec2Hitbox>(e).y;

        reg.get<gameLogic::Vec2Pos>(e) =
            gameLogic::Vec2Pos{{x, std::clamp(y, config::MAP_ORIGIN, maxY)}};
        reg.get<gameLogic::Vec2Velocity>(e) =
            gameLogic::Vec2Velocity{{-reg.get<Speed>(e).value, 0.f}};
        return e;
    }

    // Fait apparaitre un monstre a droite de la carte, a la hauteur y donnee,
    // et le lance vers la gauche.
    inline Engine::Entity spawnMonster(Engine::Registry &reg, const float y,
        const MonsterKind kind = MonsterKind::Basic)
    {
        constexpr float x = config::MAP_WIDTH + config::SPAWN_MARGIN;

        return spawnMonsterAt(reg, x, y, kind);
    }

    // Le boss entre par la droite, centre sur la hauteur de la carte.
    inline Engine::Entity spawnBoss(Engine::Registry &reg,
        const BossKind kind = BossKind::Standard)
    {
        const Engine::Entity e = createBoss(reg, kind);
        constexpr float x = config::MAP_WIDTH + config::SPAWN_MARGIN;
        const float height = reg.get<gameLogic::Vec2Hitbox>(e).y;

        reg.get<gameLogic::Vec2Pos>(e) = gameLogic::Vec2Pos{{x,
            (config::MAP_HEIGHT - height) / 2.f}};
        reg.get<gameLogic::Vec2Velocity>(e) =
            gameLogic::Vec2Velocity{{-reg.get<Speed>(e).value, 0.f}};
        return e;
    }
}
