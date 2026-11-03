#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <random>
#include "DeathRuns.hpp"
#include "Formations.hpp"
#include "LevelBuilder.hpp"
#include "Zones.hpp"

namespace rtype {
    // Les niveaux normaux : de l'ordre d'une dizaine d'obstacles, repartis sur
    // tout le niveau (asteroides qui volent, lasers qui ne touchent que les
    // joueurs), sans jamais en poser dans les 200 dernieres unites avant un
    // boss.
    inline constexpr int SCATTER_TARGET = 10;
    inline constexpr float SCATTER_FROM = 250.f;
    inline constexpr float SCATTER_CLEARANCE = 180.f;

    inline float scatterLimit(const LevelDef &level)
    {
        float limit = level.length - 300.f;

        for (const BossSpawn &boss : level.bosses) {
            limit = std::min(limit, boss.second - 200.f);
        }
        return limit;
    }

    inline bool crowded(const std::vector<ObstacleStep> &walls, const float at)
    {
        return std::ranges::any_of(walls, [at](const ObstacleStep &step) {
            return std::abs(step.at - at) < SCATTER_CLEARANCE;
        });
    }

    inline ObstacleStep asteroid(std::mt19937 &rng, const float at)
    {
        const float size = 90.f + static_cast<float>(rng() % 90);
        const float y = 80.f + static_cast<float>(rng()
            % static_cast<unsigned>(config::MAP_HEIGHT - 160.f - size));
        ObstacleStep rock = wall(at, y, size, size);

        if (rng() % 2 == 0) {
            const float speed = 1.f + static_cast<float>(rng() % 6) / 10.f;
            const Motion drift{60.f + static_cast<float>(rng() % 90),
                4.f + static_cast<float>(rng() % 3)};

            rock = withMotion(rock, drift, speed);
        }
        return rock;
    }

    inline int scatterLasers(const int number)
    {
        if (number < 3) {
            return 0;
        }
        if (zoneOf(0, static_cast<std::size_t>(number)) == Zone::Core) {
            return 5;
        }
        return std::min(3, 1 + (number - 3) / 6);
    }

    // Dans l'epave, des mines et des tourelles remplacent une partie des
    // asteroides.
    inline void wreckage(std::vector<ObstacleStep> &walls, const float at,
        const int index)
    {
        if (index % 3 == 1) {
            placeMines(walls, at, 1.f, index % 2 == 0);
        } else {
            placeTurret(walls, at, 1.f, index);
        }
    }

    inline void placeScattered(std::vector<ObstacleStep> &walls,
        const Zone zone, const float at, std::mt19937 &rng, const int index)
    {
        if (zone == Zone::Wreckage && index % 3 != 0) {
            wreckage(walls, at, index);
            return;
        }
        walls.push_back(asteroid(rng, at));
    }

    inline void scatter(LevelDef &level, const unsigned number)
    {
        std::mt19937 rng(number * 7919u);
        const float limit = scatterLimit(level);
        const int missing = SCATTER_TARGET
            - static_cast<int>(level.obstacles.size());
        const int lasers = scatterLasers(static_cast<int>(number));
        const int every = std::max(1, missing / (lasers + 1));
        const Zone zone = zoneOf(0, number);
        int placed = 0;

        for (int i = 0; i < missing && limit > SCATTER_FROM; ++i) {
            const float at = SCATTER_FROM + (static_cast<float>(i) + 0.5f)
                * (limit - SCATTER_FROM) / static_cast<float>(missing);

            if (crowded(level.obstacles, at)) {
                continue;
            }
            if (placed < lasers && (i + 1) % every == 0) {
                level.obstacles.push_back(laser(at));
                ++placed;
            } else {
                placeScattered(level.obstacles, zone, at, rng, i);
            }
        }
        sortWalls(level.obstacles);
    }

    inline bool canTakeFormation(const WaveStep &wave)
    {
        return wave.count >= 3 && wave.kind != MonsterKind::Tank
            && wave.kind != MonsterKind::Bomber
            && wave.kind < MonsterKind::MiniTank;
    }

    inline std::vector<Shape> shapesOf(const Zone zone)
    {
        if (zone == Zone::Nebula) {
            return {Shape::Sine, Shape::Wedge};
        }
        if (zone == Zone::Wreckage) {
            return {Shape::Column, Shape::Wedge};
        }
        if (zone == Zone::Core) {
            return {Shape::Wedge, Shape::Column, Shape::Sine};
        }
        return {};
    }

    // Les vagues des niveaux avances prennent des formes propres a la zone.
    inline void shapeWaves(LevelDef &level, const unsigned number)
    {
        const std::vector<Shape> shapes = shapesOf(zoneOf(0, number));

        for (std::size_t i = 0; i < level.waves.size() && !shapes.empty();
            ++i) {
            if (canTakeFormation(level.waves[i])) {
                level.waves[i].shape = shapes[(i + number) % shapes.size()];
            }
        }
    }
}
