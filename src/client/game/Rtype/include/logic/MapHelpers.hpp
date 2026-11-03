#pragma once

#include <algorithm>
#include <vector>
#include "LevelBuilder.hpp"

namespace rtype {
    using Walls = std::vector<ObstacleStep>;

    struct Gap {
        float center;
        float height;
        Motion motion{};
    };

    struct PortalMark {
        int afterGate;
        ObstacleKind kind;
    };

    struct GateStyle {
        float start;
        float spacing;
        float width;
        ObstacleKind kind;
        float speed = 1.f;
        bool pinch = false;
    };

    struct Sweeper {
        int afterGate;
        float y;
        float height;
        Motion motion;
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
            addGate(walls, {at, gap.center - gap.height / 2.f, gap.height,
                style.width, style.kind, gap.motion, style.speed,
                style.pinch});
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

    inline float portalAt(const GateStyle &style, const int afterGate)
    {
        if (afterGate < 0) {
            return style.start - 220.f;
        }
        const float gateStart = style.start
            + style.spacing * static_cast<float>(afterGate);
        const float free = style.spacing - style.width
            - config::PORTAL_WIDTH;

        return gateStart + style.width + free / 2.f;
    }

    inline void sortWalls(Walls &walls)
    {
        std::ranges::stable_sort(walls, [](const auto &a, const auto &b) {
            return a.at < b.at;
        });
    }

    // Une course de portes, avec ses portails (vitesse, touches inversees).
    inline void addRun(Walls &walls, const std::vector<Gap> &gaps,
        const GateStyle &style, const std::vector<PortalMark> &portals)
    {
        addGates(walls, gaps, style);
        for (const PortalMark &mark : portals) {
            ObstacleStep gate = portal(portalAt(style, mark.afterGate),
                mark.kind);

            gate.speed = style.speed;
            walls.push_back(gate);
        }
        sortWalls(walls);
    }

    // Des blocs a pics qui montent et descendent vite entre deux portes.
    inline void addSweepers(Walls &walls, const GateStyle &style,
        const std::vector<Sweeper> &sweepers)
    {
        for (const Sweeper &sweeper : sweepers) {
            const ObstacleStep block = wall(portalAt(style, sweeper.afterGate),
                sweeper.y, config::SWEEPER_WIDTH, sweeper.height,
                ObstacleKind::Spike);

            walls.push_back(withMotion(block, sweeper.motion, style.speed));
        }
        sortWalls(walls);
    }
}
