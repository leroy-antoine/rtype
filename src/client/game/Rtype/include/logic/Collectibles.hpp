//
// Created by antoine on 10/28/26.
//

#pragma once

#include <algorithm>
#include <cstddef>
#include <random>
#include <vector>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Components.hpp"
#include "Colisions.hpp"
#include "Config.hpp"
#include "Factories.hpp"
#include "Queries.hpp"

namespace rtype {
    inline bool isSummoned(const Engine::Registry &reg, const Engine::Entity e)
    {
        return reg.has<MonsterState>(e) && reg.get<MonsterState>(e).summoned;
    }

    inline Engine::Entity spawnCoin(Engine::Registry &reg,
        const Engine::Entity dead, std::mt19937 &rng)
    {
        if (isSummoned(reg, dead)) {
            return Engine::Entity{};
        }
        if (!reg.has<MonsterTag>(dead) || !reg.has<gameLogic::Vec2Pos>(dead)
            || !reg.has<gameLogic::Vec2Hitbox>(dead)) {
            return Engine::Entity{};
        }
        const gameLogic::Vec2Pos pos = reg.get<gameLogic::Vec2Pos>(dead);
        const gameLogic::Vec2Hitbox box = reg.get<gameLogic::Vec2Hitbox>(dead);
        const Engine::Entity coin = createCoin(reg, rng);

        reg.add(coin, gameLogic::Vec2Pos{{
            pos.x + box.x / 2.f - config::COIN_WIDTH / 2.f,
            pos.y + box.y / 2.f - config::COIN_HEIGHT / 2.f}});
        return coin;
    }

    inline std::vector<Collider> collectCoins(Engine::Registry &reg)
    {
        std::vector<Collider> found;

        reg.each<coins, gameLogic::Vec2Pos, gameLogic::Vec2Hitbox>(
            [&found](const Engine::Entity e, const coins &,
                const gameLogic::Vec2Pos &pos,
                const gameLogic::Vec2Hitbox &box) {
                found.push_back({e, pos, box, Team{}});
            });
        return found;
    }

    inline void addToPurse(Engine::Registry &reg, const std::size_t amount)
    {
        reg.each<CoinCounter>([amount](const Engine::Entity, CoinCounter &purse) {
            purse.total += amount;
        });
    }

    inline bool touchesAny(const Collider &coin,
        const std::vector<Collider> &players)
    {
        return std::ranges::any_of(players, [&coin](const Collider &player) {
            return overlaps(coin, player);
        });
    }

    inline void coinPickupSystem(Engine::Registry &reg)
    {
        const std::vector<Collider> players =
            collectColliders(reg, collectPlayers(reg));
        std::vector<Engine::Entity> taken;

        for (const Collider &coin : collectCoins(reg)) {
            if (touchesAny(coin, players)) {
                taken.push_back(coin.entity);
            }
        }
        for (const Engine::Entity e : taken) {
            addToPurse(reg, reg.get<coins>(e).value);
            reg.destroy(e);
        }
    }
}
