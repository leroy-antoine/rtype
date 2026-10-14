//
// Created by antoine on 10/14/26.
//

#ifndef RTYPE_COMPONENTS_HPP
#define RTYPE_COMPONENTS_HPP
#include <vector>

#include "Movements.hpp"
#include "utils/vectors.hpp"

namespace rtype {


    struct Player {
        int hp = 0;
        int damages = 0;
        int id = 0;
        float speed = 100.0;
        gameLogic::Vec2 pos = {0, 0};
        gameLogic::Vec2 direction = {0, 0}; //En gros, si fleche gauche est touché, x = -1, y = 0
        gameLogic::Velocity velocity = {direction.x * speed, direction.y * speed};

        void updateVelocity()
        {
            velocity.calculateVelocity(direction, speed);
        }

        void updatePos(const float dt)
        {
            pos = calculatePos(pos, velocity.velocity, dt);
        }
    };
}

#endif //RTYPE_COMPONENTS_HPP