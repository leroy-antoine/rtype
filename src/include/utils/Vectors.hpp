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

    struct Vec2Pos : Vec2 {};
    struct Vec2Direction : Vec2 {};
    struct Vec2Velocity : Vec2 {};
    struct Vec2Hitbox : Vec2 {};
}

