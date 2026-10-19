//
// Created by antoine on 10/16/26.
//

#pragma once
#include "Components.hpp"
#include "Config.hpp"
#include "engine/core/Entity.hpp"
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"

namespace rtype {
    inline bool isShooter(const Engine::Registry &reg, const Engine::Entity e)
    {
        return reg.has<gameLogic::Vec2Pos>(e)
            && reg.has<gameLogic::Vec2Hitbox>(e)
            && reg.has<Team>(e)
            && reg.has<Damage>(e);
    }

    inline float bulletDirection(const Team &team)
    {
        if (team.id == TeamId::Player) {
            return 1.f;
        }
        return -1.f;
    }

    inline gameLogic::Vec2Pos bulletStartPos(const gameLogic::Vec2Pos &pos,
        const gameLogic::Vec2Hitbox &box, const float direction)
    {
        float x = pos.x - config::BULLET_WIDTH;
        const float y = pos.y + box.y / 2.f - config::BULLET_HEIGHT / 2.f;

        if (direction > 0.f) {
            x = pos.x + box.x;
        }
        return gameLogic::Vec2Pos{{x, y}};
    }
}
