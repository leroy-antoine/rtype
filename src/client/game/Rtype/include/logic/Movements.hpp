//
// Created by antoine on 10/14/26.
//

#pragma once
#include "utils/Vectors.hpp"
#include "Components.hpp"

namespace rtype {
    inline gameLogic::Vec2 inputDirection(const PlayerInput &input,
        const bool inverted = false)
    {
        gameLogic::Vec2 direction{};

        if (input.right)
            direction.x += 1.f;
        if (input.left)
            direction.x -= 1.f;
        if (input.down)
            direction.y += 1.f;
        if (input.up)
            direction.y -= 1.f;
        if (inverted) {
            direction = direction * -1.f;
        }
        return gameLogic::normalize(direction);
    }

    inline gameLogic::Vec2 calculatePos(gameLogic::Vec2 actualPos,
        const gameLogic::Vec2 &velocity, const float dt)
    {
        return actualPos + velocity * dt;
    }
}
