//
// Created by antoine on 10/14/26.
//

#ifndef RTYPE_COMPONENTS_HPP
#define RTYPE_COMPONENTS_HPP
#include "Movements.hpp"
#include "utils/vectors.hpp"

namespace rtype {
    struct Player {
        int hp = 100;
        int damages = 10;
        int id = 0; //L'id dans l'ECS #sij'aibiencompris
        float speed = 100.0;
        float shootCooldown = 0.3; //on verra si on l'implémente
        gameLogic::Vec2Pos pos = {0, 0}; //une struct par component
        gameLogic::Vec2Direction direction = {0, 0}; //En gros, si fleche gauche est touché, x = -1, y = 0
        gameLogic::Vec2Velocity velocity = {direction.x * speed, direction.y * speed};
        gameLogic::Vec2Hitbox Hitbox;

        void updateVelocity()
        {
            velocity.calculateVelocity(direction, speed);
        }

        void updatePos(const float dt)
        {
            pos = calculatePos(pos, velocity.velocity, dt);
        }

        void takeDamages(const int dmg)
        {
            hp -= dmg;
        }

        // [[nodiscard]] bool getAlive() const
        // {
        //     return hp > 0;
        // }

        void updateCooldown(float dt)
        {
            shootCooldown -= dt;
        }

        [[nodiscard]] bool attackCooldown() const {
                return shootCooldown <= 0;
        }


    };

    struct Monster : Player {
    };
}

#endif //RTYPE_COMPONENTS_HPP