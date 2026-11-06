#include <array>
#include "Names.hpp"
#include "Parsers.hpp"

namespace rtype::mapjson {
    namespace {
        enum class Piece {
            Rock, Spike, Laser, Turret, Mine, Gate,
            ReversePortal, FastPortal, NormalPortal
        };

        using OK = ObstacleKind;

        inline constexpr std::array<Name<Piece>, 9> PIECE_NAMES = {{
            {"Rock", Piece::Rock}, {"Spike", Piece::Spike},
            {"Laser", Piece::Laser}, {"Turret", Piece::Turret},
            {"Mine", Piece::Mine}, {"Gate", Piece::Gate},
            {"ReversePortal", Piece::ReversePortal},
            {"FastPortal", Piece::FastPortal},
            {"NormalPortal", Piece::NormalPortal}}};

        ObstacleKind kindOf(const Piece piece)
        {
            switch (piece) {
                case Piece::Spike: return OK::Spike;
                case Piece::Laser: return OK::Laser;
                case Piece::Turret: return OK::Turret;
                case Piece::Mine: return OK::Mine;
                case Piece::ReversePortal: return OK::ReversePortal;
                case Piece::FastPortal: return OK::FastPortal;
                case Piece::NormalPortal: return OK::NormalPortal;
                default: return OK::Rock;
            }
        }

        bool isPortalPiece(const Piece piece)
        {
            return isPortal(kindOf(piece));
        }

        // A moving obstacle needs a period, or it would never move.
        Motion readMotion(const Reader &obstacle)
        {
            Motion motion{};

            motion.amplitude = obstacle.numberOr("amplitude", AMPLITUDE, 0.f);
            motion.period = obstacle.numberOr("period", PERIOD, 0.f);
            if (motion.amplitude != 0.f && motion.period <= 0.f) {
                obstacle.fail("amplitude needs a period greater than 0");
            }
            return motion;
        }

        ObstacleStep readSolid(const Reader &obstacle, const float length,
            const Piece piece)
        {
            ObstacleStep step{};

            obstacle.allow({"type", "at", "y", "width", "height",
                "amplitude", "period", "speed"});
            step.at = obstacle.number("at", {0.f, length});
            step.y = obstacle.number("y", OBSTACLE_Y);
            step.width = obstacle.number("width", OBSTACLE_WIDTH);
            step.height = obstacle.number("height", OBSTACLE_HEIGHT);
            step.kind = kindOf(piece);
            return withMotion(step, readMotion(obstacle),
                obstacle.numberOr("speed", SPEED, 1.f));
        }

        ObstacleStep readPortal(const Reader &obstacle, const float length,
            const Piece piece)
        {
            obstacle.allow({"type", "at", "speed"});
            ObstacleStep step = portal(obstacle.number("at", {0.f, length}),
                kindOf(piece));

            step.speed = obstacle.numberOr("speed", SPEED, 1.f);
            return step;
        }

        void readGap(const Reader &obstacle, GateSpec &gate)
        {
            gate.gapY = obstacle.number("gapY", GAP_Y);
            gate.gapHeight = obstacle.number("gapHeight", GAP_HEIGHT);
            if (gate.gapY + gate.gapHeight > config::MAP_HEIGHT) {
                obstacle.fail("the gap goes below the map");
            }
        }

        GateSpec readGate(const Reader &obstacle, const float length)
        {
            GateSpec gate{};

            obstacle.allow({"type", "at", "gapY", "gapHeight", "width",
                "wall", "amplitude", "period", "speed", "pinch"});
            gate.at = obstacle.number("at", {0.f, length});
            readGap(obstacle, gate);
            gate.width = obstacle.number("width", OBSTACLE_WIDTH);
            gate.kind = obstacle.choiceOr("wall", WALL_NAMES, OK::Rock);
            gate.motion = readMotion(obstacle);
            gate.speed = obstacle.numberOr("speed", SPEED, 1.f);
            gate.pinch = obstacle.flagOr("pinch", false);
            return gate;
        }
    }

    // A gate is two walls around a gap, so one element can add two steps.
    void parseObstacle(const Reader &obstacle, const float length,
        std::vector<ObstacleStep> &out)
    {
        const Piece piece = obstacle.choice("type", PIECE_NAMES);

        if (piece == Piece::Gate) {
            addGate(out, readGate(obstacle, length));
            return;
        }
        if (isPortalPiece(piece)) {
            out.push_back(readPortal(obstacle, length, piece));
            return;
        }
        out.push_back(readSolid(obstacle, length, piece));
    }
}
