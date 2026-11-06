#pragma once

#include <algorithm>
#include <cmath>
#include <vector>
#include "utils/Vectors.hpp"
#include "Colisions.hpp"

namespace rtype {
    inline float shortest(const float a, const float b)
    {
        if (std::abs(a) <= std::abs(b)) {
            return a;
        }
        return b;
    }

    // Plus petit deplacement qui sort le joueur du bloc, ou rien s'il ne le
    // touche pas.
    inline gameLogic::Vec2 pushOutOf(const Collider &player,
        const Collider &block)
    {
        if (!overlaps(player, block)) {
            return {};
        }
        const float toRight = block.pos.x + block.box.x - player.pos.x;
        const float toLeft = block.pos.x - (player.pos.x + player.box.x);
        const float toBottom = block.pos.y + block.box.y - player.pos.y;
        const float toTop = block.pos.y - (player.pos.y + player.box.y);
        const float x = shortest(toRight, toLeft);
        const float y = shortest(toBottom, toTop);

        if (std::abs(x) < std::abs(y)) {
            return {x, 0.f};
        }
        return {0.f, y};
    }

    inline gameLogic::Vec2Pos resolveSolids(const Collider &player,
        const std::vector<Collider> &blocks)
    {
        Collider moved = player;

        for (const Collider &block : blocks) {
            moved.pos = gameLogic::Vec2Pos{moved.pos
                + pushOutOf(moved, block)};
        }
        return moved.pos;
    }
}
