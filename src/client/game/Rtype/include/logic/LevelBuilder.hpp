#pragma once

#include <cmath>
#include <utility>
#include <vector>
#include "Config.hpp"
#include "Kinds.hpp"
#include "Levels.hpp"
#include "Waves.hpp"

namespace rtype {
    using BossSpawn = std::pair<BossKind, float>;

    struct LevelDef {
        float length;
        std::vector<WaveStep> waves;
        std::vector<BossSpawn> bosses;
        std::vector<ObstacleStep> obstacles;
        bool deathRun = false;
    };

    inline WaveStep wave(const float at, const MonsterKind kind,
        const int count, const float y = RANDOM_Y, const float dy = 0.f,
        const float interval = 0.5f)
    {
        return WaveStep{at, count, y, dy, interval, kind};
    }

    inline WaveStep shifted(const WaveStep &step, const float offset)
    {
        WaveStep moved = step;

        moved.at += offset;
        return moved;
    }

    inline ObstacleStep wall(const float at, const float y, const float width,
        const float height, const ObstacleKind kind = ObstacleKind::Rock)
    {
        return ObstacleStep{at, y, width, height, kind};
    }

    // Un portail occupe toute la hauteur : impossible de l'eviter.
    inline ObstacleStep portal(const float at, const ObstacleKind kind)
    {
        return wall(at, 0.f, config::PORTAL_WIDTH, config::MAP_HEIGHT, kind);
    }

    struct Motion {
        float amplitude = 0.f;
        float period = 0.f;
    };

    struct GateSpec {
        float at;
        float gapY;
        float gapHeight;
        float width;
        ObstacleKind kind = ObstacleKind::Rock;
        Motion motion{};
        float speed = 1.f;
        bool pinch = false;
    };

    inline ObstacleStep withMotion(ObstacleStep step, const Motion &motion,
        const float speed = 1.f)
    {
        step.amplitude = motion.amplitude;
        step.period = motion.period;
        step.speed = speed;
        return step;
    }

    inline ObstacleStep gateWall(const GateSpec &gate, const float y,
        const float height, const float amplitude)
    {
        return withMotion(wall(gate.at, y, gate.width, height, gate.kind),
            {amplitude, gate.motion.period}, gate.speed);
    }

    // Deux murs autour d'un passage. Si la porte bouge, les murs depassent de
    // la carte pour que le passage reste le seul trou pendant le mouvement.
    // Une porte qui se ferme (pinch) fait bouger ses murs en sens opposes.
    inline void addGate(std::vector<ObstacleStep> &walls, const GateSpec &gate)
    {
        const float amplitude = gate.motion.amplitude;
        const float reach = std::abs(amplitude);
        const float gapEnd = gate.gapY + gate.gapHeight;
        float bottomAmplitude = amplitude;

        if (gate.pinch) {
            bottomAmplitude = -amplitude;
        }
        if (gate.gapY > 0.f || reach > 0.f) {
            walls.push_back(gateWall(gate, -reach, gate.gapY + reach,
                amplitude));
        }
        if (gapEnd < config::MAP_HEIGHT || reach > 0.f) {
            walls.push_back(gateWall(gate, gapEnd,
                config::MAP_HEIGHT - gapEnd + reach, bottomAmplitude));
        }
    }

    inline ObstacleStep shifted(const ObstacleStep &step, const float offset)
    {
        ObstacleStep moved = step;

        moved.at += offset;
        return moved;
    }

    inline BossSpawn shifted(const BossSpawn &boss, const float offset)
    {
        return {boss.first, boss.second + offset};
    }
}
