#pragma once

#include <cstddef>
#include <random>
#include <vector>
#include "Augments.hpp"
#include "Config.hpp"

namespace rtype {
    inline int rarityWeight(const Rarity rarity)
    {
        switch (rarity) {
            case Rarity::Common:
                return config::RARITY_COMMON_WEIGHT;
            case Rarity::Rare:
                return config::RARITY_RARE_WEIGHT;
            case Rarity::Epic:
                return config::RARITY_EPIC_WEIGHT;
            case Rarity::Legendary:
                return config::RARITY_LEGENDARY_WEIGHT;
        }
        return 0;
    }

    inline std::size_t pickWeighted(const std::vector<AugmentDef> &pool,
        std::mt19937 &rng)
    {
        std::vector<int> weights;

        for (const AugmentDef &def : pool) {
            weights.push_back(rarityWeight(def.rarity));
        }
        std::discrete_distribution<std::size_t> pick(weights.begin(),
            weights.end());

        return pick(rng);
    }

    inline std::vector<AugmentId> rollOffers(std::mt19937 &rng,
        const std::size_t count)
    {
        std::vector<AugmentDef> pool(AUGMENTS.begin(), AUGMENTS.end());
        std::vector<AugmentId> offers;

        while (offers.size() < count && !pool.empty()) {
            const std::size_t picked = pickWeighted(pool, rng);

            offers.push_back(pool[picked].id);
            pool.erase(pool.begin() + picked);
        }
        return offers;
    }
}
