#pragma once

#include <algorithm>
#include <cmath>
#include <optional>
#include <vector>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Colisions.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Factories.hpp"
#include "Queries.hpp"

namespace rtype {
    // L'ennemi le plus proche sur la ligne du nez, a portee.
    inline std::optional<Collider> laneTarget(Engine::Registry &reg,
        const gameLogic::Vec2 &nose)
    {
        std::optional<Collider> best;

        for (const Collider &enemy : livingEnemies(reg)) {
            const bool inLane = enemy.pos.y < nose.y + config::LIGHTNING_LANE
                && enemy.pos.y + enemy.box.y > nose.y - config::LIGHTNING_LANE;
            const bool inRange = enemy.pos.x + enemy.box.x > nose.x
                && enemy.pos.x <= nose.x + config::LIGHTNING_RANGE;

            if (inLane && inRange && (!best || enemy.pos.x < best->pos.x)) {
                best = enemy;
            }
        }
        return best;
    }

    inline bool alreadyHit(const std::vector<Collider> &hit,
        const Collider &enemy)
    {
        return std::ranges::any_of(hit, [&enemy](const Collider &other) {
            return other.entity == enemy.entity;
        });
    }

    // L'ennemi colle a `from` le plus proche, qui n'a pas deja ete touche.
    inline std::optional<Collider> nextLink(Engine::Registry &reg,
        const Collider &from, const std::vector<Collider> &hit)
    {
        std::optional<Collider> best;
        float bestGap = config::LIGHTNING_JUMP;

        for (const Collider &enemy : livingEnemies(reg)) {
            const float gap = gapBetween(from, enemy);

            if (!alreadyHit(hit, enemy) && gap <= bestGap) {
                best = enemy;
                bestGap = gap;
            }
        }
        return best;
    }

    inline SpriteId boltSprite(const bool rising)
    {
        if (rising) {
            return SpriteId::PlayerBoltUp;
        }
        return SpriteId::PlayerBoltDown;
    }

    // Un segment de l'eclair, dessine par le client : la boite l'englobe et
    // le sprite dit dans quel sens il va.
    inline void createBolt(Engine::Registry &reg, const gameLogic::Vec2 &from,
        const gameLogic::Vec2 &to)
    {
        const float width = std::max(std::fabs(to.x - from.x),
            config::LIGHTNING_MIN_SIZE);
        const float height = std::max(std::fabs(to.y - from.y),
            config::LIGHTNING_MIN_SIZE);
        const bool rising = (to.x - from.x) * (to.y - from.y) < 0.f;
        const Engine::Entity bolt = reg.create();

        reg.add(bolt, gameLogic::Vec2Pos{{std::min(from.x, to.x),
            std::min(from.y, to.y)}});
        reg.add(bolt, gameLogic::Vec2Hitbox{{width, height}});
        reg.add(bolt, Renderable{boltSprite(rising)});
        reg.add(bolt, Lifetime{config::LIGHTNING_DURATION,
            config::LIGHTNING_DURATION});
    }

    inline int weakened(const int damage)
    {
        const long next = std::lround(static_cast<float>(damage)
            * config::LIGHTNING_FALLOFF);

        return std::max(1, static_cast<int>(next));
    }

    // L'eclair saute d'un ennemi a l'autre, de plus en plus faiblement.
    inline void chain(Engine::Registry &reg, const Engine::Entity source,
        const Collider &first, const gameLogic::Vec2 &nose)
    {
        std::vector<Collider> hit;
        std::optional<Collider> link = first;
        gameLogic::Vec2 from = nose;
        int damage = reg.get<Damage>(source).value;

        for (int jump = 0; link && jump <= config::LIGHTNING_JUMPS; ++jump) {
            reg.get<Damage>(source).value = damage;
            applyDamages(link->entity, source, reg);
            createBolt(reg, from, centerOfBox(*link));
            hit.push_back(*link);
            from = centerOfBox(*link);
            damage = weakened(damage);
            link = nextLink(reg, *link, hit);
        }
    }

    // Frappe le premier ennemi devant le vaisseau, puis les ennemis colles.
    inline bool fireLightning(Engine::Registry &reg,
        const Engine::Entity shooter)
    {
        if (!isShooter(reg, shooter)) {
            return false;
        }
        const Collider ship{shooter, reg.get<gameLogic::Vec2Pos>(shooter),
            reg.get<gameLogic::Vec2Hitbox>(shooter), Team{}};
        const gameLogic::Vec2 nose{ship.pos.x + ship.box.x,
            centerOfBox(ship).y};
        const std::optional<Collider> first = laneTarget(reg, nose);

        if (!first) {
            return false;
        }
        const Engine::Entity source = reg.create();

        reg.add(source, Team{TeamId::Player});
        reg.add(source, Damage{static_cast<int>(std::lround(
            static_cast<float>(reg.get<Damage>(shooter).value)
            * config::LIGHTNING_DAMAGE_FACTOR))});
        chain(reg, source, *first, nose);
        reg.destroy(source);
        return true;
    }
}
