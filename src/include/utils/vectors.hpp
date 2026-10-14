#pragma once

namespace gameLogic {
    struct Vec2 {
        float x = 0.f;
        float y = 0.f;
    };

    inline Vec2 operator+(Vec2 a, Vec2 b) {
        return {a.x + b.x, a.y + b.y};
    }

    inline Vec2 operator*(Vec2 a, float x) {
        return {a.x * x, a.y * x};
    }

    struct Vec2Pos : Vec2 {};       // position dans le monde
    struct Vec2Direction : Vec2 {}; // direction voulue, ex. {-1, 0} = gauche
    struct Vec2Velocity : Vec2 {
        Vec2 velocity;

        void calculateVelocity(const Vec2 direction, const float speed) {
            velocity = direction * speed;
        }
    };  // direction * vitesse, en unités par seconde
    struct Vec2Hitbox : Vec2 {};    // taille de la boite de collision (largeur, hauteur)
}

