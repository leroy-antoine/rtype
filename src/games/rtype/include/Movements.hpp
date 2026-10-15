//
// Created by antoine on 10/14/26.
//

#pragma once
#include "utils/vectors.hpp"

namespace rtype {
    inline gameLogic::Vec2 calculatePos(gameLogic::Vec2 actualPos,
        const gameLogic::Vec2 &velocity, const float dt)
    {
        return actualPos + velocity * dt;
    }
}
