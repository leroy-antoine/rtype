#pragma once

namespace gameLogic {
    struct Vec2 {
        float x = 0.f;
        float y = 0.f;
    };

    struct Velocity {
        Vec2 speed; //Si j'ai x = 100 et y = 0, mon projectil va à 100 px/temps vers la droite
    };

    inline Vec2 operator+(Vec2 a, Vec2 b) {
        return {a.x + b.x, a.y + b.y};
    }

    inline Vec2 operator*(Vec2 a, float x) {
        return {a.x * x, a.y * x};
    }
}

