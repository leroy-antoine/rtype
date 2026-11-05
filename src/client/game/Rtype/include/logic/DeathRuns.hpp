#pragma once

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>
#include "MapHelpers.hpp"

namespace rtype {
    // Un laser dont la phase est calee sur le temps du niveau : tous les
    // lasers d'un niveau s'allument et s'eteignent ensemble, au rythme de
    // la musique (un cycle = 4 temps).
    inline ObstacleStep laser(const float at, const float shift = 0.f)
    {
        const float phase = std::fmod(at / config::SCROLL_SPEED + shift,
            config::LASER_PERIOD);
        ObstacleStep step = wall(at, 0.f, config::LASER_WIDTH,
            config::MAP_HEIGHT, ObstacleKind::Laser);

        step.period = phase;
        return step;
    }

    // Un parcours de death run : une suite de pieges de types melanges, ecrite
    // en toutes lettres (G porte, B laser, M mines, T tourelle, F portail
    // rapide, R touches inversees, N vitesse normale). Tout le parcours avance
    // a la meme vitesse.
    struct Course {
        float start;
        float speed;
        float after;
        const char *pattern;
        float gapFrom;
        float gapTo;
        Motion motion;
        bool pinch;
    };

    inline constexpr float COURSE_LANES[6] = {320.f, 700.f, 480.f, 760.f,
        300.f, 620.f};
    inline constexpr float MINE_ROWS[3] = {200.f, 500.f, 800.f};
    inline constexpr float MINE_STAGGER = 150.f;
    inline constexpr Motion MINE_DRIFT{100.f, 3.f};

    inline void placeMines(Walls &walls, const float at, const float speed,
        const bool shifted)
    {
        float shift = 0.f;

        if (shifted) {
            shift = MINE_STAGGER;
        }
        for (const float row : MINE_ROWS) {
            walls.push_back(withMotion(wall(at, row + shift,
                config::MINE_SIZE, config::MINE_SIZE, ObstacleKind::Mine),
                MINE_DRIFT, speed));
        }
    }

    inline void placeGate(Walls &walls, const Course &course, const float at,
        const int gate)
    {
        const std::string pattern = course.pattern;
        const float span = static_cast<float>(pattern.size());
        const float ratio = static_cast<float>(gate) / span;
        const float gap = course.gapFrom
            + (course.gapTo - course.gapFrom) * ratio;
        const float center = COURSE_LANES[gate % 6];

        addGate(walls, {at, center - gap / 2.f, gap, 80.f,
            ObstacleKind::Spike, course.motion, course.speed, course.pinch});
    }

    inline void placeTurret(Walls &walls, const float at, const float speed,
        const int count)
    {
        float y = 0.f;

        if (count % 2 == 1) {
            y = config::MAP_HEIGHT - config::OBSTACLE_TILE;
        }
        ObstacleStep step = wall(at, y, config::OBSTACLE_TILE,
            config::OBSTACLE_TILE, ObstacleKind::Turret);

        step.speed = speed;
        walls.push_back(step);
    }

    inline ObstacleKind portalKind(const char code)
    {
        if (code == 'R') {
            return ObstacleKind::ReversePortal;
        }
        if (code == 'F') {
            return ObstacleKind::FastPortal;
        }
        return ObstacleKind::NormalPortal;
    }

    inline void addCoursePortal(Walls &walls, const Course &course,
        const float at, const ObstacleKind kind)
    {
        ObstacleStep gate = portal(at, kind);

        gate.speed = course.speed;
        walls.push_back(gate);
    }

    inline void placePiece(Walls &walls, const Course &course,
        const char kind, const float at, const int index)
    {
        const int gates = index / 2;

        if (kind == 'G') {
            placeGate(walls, course, at, index);
        } else if (kind == 'B') {
            ObstacleStep beam = laser(at);

            beam.speed = course.speed;
            walls.push_back(beam);
        } else if (kind == 'M') {
            placeMines(walls, at, course.speed, gates % 2 == 1);
        } else if (kind == 'T') {
            placeTurret(walls, at, course.speed, gates);
        } else {
            addCoursePortal(walls, course, at, portalKind(kind));
        }
    }

    inline void checkPattern(const std::string &pattern)
    {
        const std::string known = "GBMTFRN";

        if (pattern.find_first_not_of(known) != std::string::npos) {
            throw std::invalid_argument("unknown course piece: " + pattern);
        }
    }

    inline float addCourse(Walls &walls, const Course &course)
    {
        const std::string pattern = course.pattern;

        checkPattern(pattern);
        float at = course.start;

        for (std::size_t i = 0; i < pattern.size(); ++i) {
            const char kind = pattern[i];

            placePiece(walls, course, kind, at, static_cast<int>(i));
            at += course.after;
            if (kind == 'T') {
                at -= course.after / 2.f;
            }
        }
        sortWalls(walls);
        return at - course.after;
    }
}
