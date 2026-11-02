#pragma once

#include <vector>
#include "LevelBuilder.hpp"

namespace rtype {
    using Walls = std::vector<ObstacleStep>;

    struct Gap {
        float center;
        float height;
    };

    struct GateStyle {
        float start;
        float spacing;
        float width;
        ObstacleKind kind;
    };

    struct CavePiece {
        float top;
        float bottom;
    };

    inline void addGates(Walls &walls, const std::vector<Gap> &gaps,
        const GateStyle &style)
    {
        float at = style.start;

        for (const Gap &gap : gaps) {
            addGate(walls, at, gap.center - gap.height / 2.f, gap.height,
                style.width, style.kind);
            at += style.spacing;
        }
    }

    inline void addCave(Walls &walls, const std::vector<CavePiece> &pieces,
        const float start, const float width)
    {
        float at = start;

        for (const CavePiece &piece : pieces) {
            walls.push_back(wall(at, 0.f, width, piece.top));
            walls.push_back(wall(at, config::MAP_HEIGHT - piece.bottom,
                width, piece.bottom));
            at += width;
        }
    }
}
