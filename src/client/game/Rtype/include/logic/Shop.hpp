#pragma once

#include <algorithm>
#include <cstddef>
#include <random>
#include <vector>
#include "Augments.hpp"
#include "Config.hpp"

namespace rtype {
    struct ShopTier {
        int level = 1;
        bool bossLevel = false;
    };

    struct RarityOdds {
        int weight;
        int perLevel;
        int bossBonus;
    };

    inline RarityOdds rarityOdds(const Rarity rarity)
    {
        switch (rarity) {
            case Rarity::Common:
                return {config::COMMON_WEIGHT, config::COMMON_WEIGHT_PER_LEVEL,
                    config::COMMON_BOSS_BONUS};
            case Rarity::Rare:
                return {config::RARE_WEIGHT, config::RARE_WEIGHT_PER_LEVEL,
                    config::RARE_BOSS_BONUS};
            case Rarity::Epic:
                return {config::EPIC_WEIGHT, config::EPIC_WEIGHT_PER_LEVEL,
                    config::EPIC_BOSS_BONUS};
            case Rarity::Legendary:
                return {config::LEGENDARY_WEIGHT,
                    config::LEGENDARY_WEIGHT_PER_LEVEL,
                    config::LEGENDARY_BOSS_BONUS};
        }
        return {config::MIN_RARITY_WEIGHT, 0, 0};
    }

    inline int rarityWeight(const Rarity rarity, const ShopTier &tier)
    {
        const RarityOdds odds = rarityOdds(rarity);
        int weight = odds.weight + odds.perLevel * tier.level;

        if (tier.bossLevel) {
            weight += odds.bossBonus;
        }
        return std::max(config::MIN_RARITY_WEIGHT, weight);
    }

    inline bool isExcluded(const AugmentId id,
        const std::vector<AugmentId> &excluded)
    {
        return std::ranges::find(excluded, id) != excluded.end();
    }

    inline std::vector<AugmentDef> shopPool(
        const std::vector<AugmentId> &excluded)
    {
        std::vector<AugmentDef> pool;

        for (const AugmentDef &def : AUGMENTS) {
            if (!isExcluded(def.id, excluded)) {
                pool.push_back(def);
            }
        }
        return pool;
    }

    inline std::size_t pickWeighted(const std::vector<AugmentDef> &pool,
        const ShopTier &tier, std::mt19937 &rng)
    {
        std::vector<int> weights;

        for (const AugmentDef &def : pool) {
            weights.push_back(rarityWeight(def.rarity, tier));
        }
        std::discrete_distribution<std::size_t> pick(weights.begin(),
            weights.end());

        return pick(rng);
    }

    inline std::vector<AugmentId> rollOffers(std::mt19937 &rng,
        const std::size_t count, const ShopTier &tier,
        const std::vector<AugmentId> &excluded)
    {
        std::vector<AugmentDef> pool = shopPool(excluded);
        std::vector<AugmentId> offers;

        while (offers.size() < count && !pool.empty()) {
            const std::size_t picked = pickWeighted(pool, tier, rng);

            offers.push_back(pool[picked].id);
            pool.erase(pool.begin() + picked);
        }
        return offers;
    }
}
