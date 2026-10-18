#pragma once
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Config.hpp"
#include "Factories.hpp"

namespace rtype {
    //Fais spawn les monstres à droite de la map, à une hauteur y donnée par le dev
    //Dans le futur, pour avoir des mobs volants, il faudra créer un tag différent pour chaque type de mob
    //Et les faire spawn à un y random

    inline Engine::Entity spawnMonster(Engine::Registry &reg, const float y,
        const int slot = 0)
    {
        const Engine::Entity e = createMonster(reg, slot);
        constexpr float x = config::MAP_WIDTH + config::SPAWN_MARGIN;

        reg.get<gameLogic::Vec2Pos>(e) = gameLogic::Vec2Pos{{x, y}};
        reg.get<gameLogic::Vec2Velocity>(e) =
            gameLogic::Vec2Velocity{{-config::MONSTER_SPEED, 0.f}};
        return e;
    }

    inline void spawnSystem(Engine::Registry &reg, float &timer,
        const float dt)
    {
        timer -= dt;
        if (timer > 0.f) {
            return;
        }
        spawnMonster(reg, 0);
        timer = config::MONSTER_SPAWN_INTERVAL;
    }
}
