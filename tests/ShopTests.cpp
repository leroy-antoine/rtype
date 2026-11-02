#include <algorithm>
#include <random>
#include <vector>
#include "logic/Game.hpp"
#include "logic/Shop.hpp"
#include "TestHarness.hpp"

using namespace rtype;

namespace {
    int countRarity(const std::vector<AugmentId> &offers, const Rarity rarity)
    {
        return std::ranges::count_if(offers, [rarity](const AugmentId id) {
            return findAugment(id)->rarity == rarity;
        });
    }
}

TEST(every_rarity_has_a_positive_weight)
{
    CHECK(rarityWeight(Rarity::Common) > 0);
    CHECK(rarityWeight(Rarity::Rare) > 0);
    CHECK(rarityWeight(Rarity::Epic) > 0);
    CHECK(rarityWeight(Rarity::Legendary) > 0);
}

TEST(offers_are_reproducible_with_a_fixed_seed)
{
    std::mt19937 first(42);
    std::mt19937 second(42);

    CHECK(rollOffers(first, 3) == rollOffers(second, 3));
}

TEST(an_offer_has_the_requested_size)
{
    std::mt19937 rng(1);

    CHECK(rollOffers(rng, 3).size() == 3);
    CHECK(rollOffers(rng, 0).empty());
}

TEST(an_offer_has_no_duplicates)
{
    std::mt19937 rng(1);

    for (int i = 0; i < 100; ++i) {
        std::vector<AugmentId> offers = rollOffers(rng, AUGMENTS.size());

        std::ranges::sort(offers);
        CHECK(std::ranges::adjacent_find(offers) == offers.end());
    }
}

TEST(an_offer_never_exceeds_the_number_of_augments)
{
    std::mt19937 rng(1);

    CHECK(rollOffers(rng, AUGMENTS.size() + 5).size() == AUGMENTS.size());
}

TEST(common_augments_are_offered_more_often_than_rare_ones)
{
    std::mt19937 rng(1);
    std::vector<AugmentId> offers;

    for (int i = 0; i < 10000; ++i) {
        offers.push_back(rollOffers(rng, 1).front());
    }
    CHECK(countRarity(offers, Rarity::Common)
        > countRarity(offers, Rarity::Rare));
    CHECK(countRarity(offers, Rarity::Rare)
        > countRarity(offers, Rarity::Epic));
}

TEST(the_shop_is_empty_until_restocked)
{
    Game game(1);

    game.addPlayer();
    CHECK(game.shopOffers().empty());
    game.restockShop();
    CHECK(game.shopOffers().size() == config::SHOP_OFFER_COUNT);
}
