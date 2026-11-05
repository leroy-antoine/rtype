#include <algorithm>
#include <random>
#include <vector>
#include "logic/Collectibles.hpp"
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

    std::vector<AugmentId> rollMany(const ShopTier &tier)
    {
        std::mt19937 rng(1);
        std::vector<AugmentId> offers;

        for (int i = 0; i < 10000; ++i) {
            offers.push_back(rollOffers(rng, 1, tier, {}).front());
        }
        return offers;
    }

    Game shopWithCoins(const std::size_t coins)
    {
        Game game(1);

        game.addPlayer();
        addToPurse(game.registry(), coins);
        game.restockShop(ShopTier{});
        return game;
    }
}

TEST(every_rarity_has_a_positive_weight)
{
    const ShopTier tier{20, true};

    CHECK(rarityWeight(Rarity::Common, tier) > 0);
    CHECK(rarityWeight(Rarity::Rare, tier) > 0);
    CHECK(rarityWeight(Rarity::Epic, tier) > 0);
    CHECK(rarityWeight(Rarity::Legendary, tier) > 0);
}

TEST(rare_augments_get_more_likely_with_the_level)
{
    const ShopTier first{1, false};
    const ShopTier last{20, false};

    CHECK(rarityWeight(Rarity::Common, last)
        < rarityWeight(Rarity::Common, first));
    CHECK(rarityWeight(Rarity::Epic, last)
        > rarityWeight(Rarity::Epic, first));
    CHECK(rarityWeight(Rarity::Legendary, last)
        > rarityWeight(Rarity::Legendary, first));
}

TEST(boss_levels_offer_better_augments)
{
    const std::vector<AugmentId> normal = rollMany(ShopTier{5, false});
    const std::vector<AugmentId> boss = rollMany(ShopTier{5, true});

    CHECK(countRarity(boss, Rarity::Epic) > countRarity(normal, Rarity::Epic));
    CHECK(countRarity(boss, Rarity::Common)
        < countRarity(normal, Rarity::Common));
}

TEST(common_augments_are_offered_more_often_on_the_first_level)
{
    const std::vector<AugmentId> offers = rollMany(ShopTier{});

    CHECK(countRarity(offers, Rarity::Common)
        > countRarity(offers, Rarity::Rare));
    CHECK(countRarity(offers, Rarity::Rare)
        > countRarity(offers, Rarity::Epic));
}

TEST(offers_are_reproducible_with_a_fixed_seed)
{
    std::mt19937 first(42);
    std::mt19937 second(42);

    CHECK(rollOffers(first, 3, ShopTier{}, {})
        == rollOffers(second, 3, ShopTier{}, {}));
}

TEST(an_offer_has_the_requested_size)
{
    std::mt19937 rng(1);

    CHECK(rollOffers(rng, 3, ShopTier{}, {}).size() == 3);
    CHECK(rollOffers(rng, 0, ShopTier{}, {}).empty());
}

TEST(an_offer_has_no_duplicates)
{
    std::mt19937 rng(1);

    for (int i = 0; i < 100; ++i) {
        std::vector<AugmentId> offers =
            rollOffers(rng, AUGMENTS.size(), ShopTier{}, {});

        std::ranges::sort(offers);
        CHECK(std::ranges::adjacent_find(offers) == offers.end());
    }
}

TEST(an_offer_never_exceeds_the_number_of_augments)
{
    std::mt19937 rng(1);

    CHECK(rollOffers(rng, AUGMENTS.size() + 5, ShopTier{}, {}).size()
        == AUGMENTS.size());
}

TEST(excluded_augments_are_never_offered)
{
    std::mt19937 rng(1);
    const std::vector<AugmentId> excluded = {
        AugmentId::DamageUp, AugmentId::SpeedUp};

    for (int i = 0; i < 100; ++i) {
        const std::vector<AugmentId> offers =
            rollOffers(rng, AUGMENTS.size(), ShopTier{}, excluded);

        CHECK(offers.size() == AUGMENTS.size() - excluded.size());
        CHECK(!isExcluded(AugmentId::DamageUp, offers));
        CHECK(!isExcluded(AugmentId::SpeedUp, offers));
    }
}

TEST(the_shop_is_empty_until_restocked)
{
    Game game(1);

    game.addPlayer();
    CHECK(game.shopOffers().empty());
    game.restockShop(ShopTier{});
    CHECK(game.shopOffers().size() == config::SHOP_OFFER_COUNT);
}

TEST(a_purchase_is_refused_without_enough_coins)
{
    Game game = shopWithCoins(0);
    const AugmentId offer = game.shopOffers().front();

    CHECK(!game.buyAugment(0, offer));
    CHECK(game.coins() == 0);
    CHECK(game.ownedAugments(0).empty());
    CHECK(game.shopOffers().size() == config::SHOP_OFFER_COUNT);
}

TEST(a_purchase_spends_the_coins_and_gives_the_augment)
{
    Game game = shopWithCoins(1000);
    const AugmentId offer = game.shopOffers().front();

    CHECK(game.buyAugment(0, offer));
    CHECK(game.coins() == 1000 - priceOf(offer));
    CHECK(game.ownedAugments(0) == std::vector<AugmentId>{offer});
    CHECK(game.shopOffers().size() == config::SHOP_OFFER_COUNT - 1);
}

TEST(a_bought_augment_is_never_offered_again)
{
    Game game = shopWithCoins(1000);
    const AugmentId bought = game.shopOffers().front();

    game.buyAugment(0, bought);
    for (int i = 0; i < 100; ++i) {
        game.restockShop(ShopTier{});
        CHECK(!isExcluded(bought, game.shopOffers()));
    }
}

TEST(only_offered_augments_can_be_bought)
{
    Game game = shopWithCoins(1000);
    const AugmentId missing = shopPool(game.shopOffers()).front().id;

    CHECK(!game.buyAugment(0, missing));
    CHECK(!game.buyAugment(1, game.shopOffers().front()));
    CHECK(!game.buyAugment(0, AugmentId::Count));
    CHECK(game.coins() == 1000);
}

TEST(coins_are_kept_when_the_game_restarts)
{
    Game game = shopWithCoins(100);

    game.registry().get<Health>(*findPlayerBySlot(game.registry(), 0)).hp = 0;
    for (float t = 0.f; t < config::RESTART_DELAY + 1.f;
        t += config::FIXED_DT) {
        game.update(config::FIXED_DT);
    }
    CHECK(game.phase() == Phase::Playing);
    CHECK(game.coins() == 100);
}

namespace {
    Segments twoLevels()
    {
        Segments segments;

        segments.levels.emplace_back(1, 0.f);
        segments.levels.emplace_back(2, 300.f);
        segments.levels.emplace_back(3, 600.f);
        segments.bossSpawn.emplace_back(BossKind::Standard, 99999.f);
        return segments;
    }

    void runFor(Game &game, const float seconds)
    {
        for (float t = 0.f; t < seconds; t += config::FIXED_DT) {
            game.update(config::FIXED_DT);
        }
    }
}

TEST(the_shop_opens_at_the_end_of_each_level_and_freezes_the_game)
{
    Game game(1, twoLevels());

    game.addPlayer();
    CHECK(!game.shopOpen());
    runFor(game, 1.f);
    CHECK(!game.shopOpen());
    runFor(game, 4.f);
    CHECK(game.shopOpen());
    CHECK(game.shopOffers().size() == config::SHOP_OFFER_COUNT);

    const float frozen = game.scroll();

    runFor(game, 2.f);
    CHECK(game.scroll() == frozen);
}

TEST(closing_the_shop_resumes_the_game_and_the_next_level_has_its_own_shop)
{
    Game game(1, twoLevels());

    game.addPlayer();
    runFor(game, 5.f);
    CHECK(game.shopOpen());
    game.closeShop();
    CHECK(!game.shopOpen());
    CHECK(game.shopOffers().empty());
    runFor(game, 1.f);
    CHECK(game.scroll() > 0.f);
    runFor(game, 5.f);
    CHECK(game.shopOpen());
}

TEST(a_new_match_closes_the_shop)
{
    Game game(1, twoLevels());

    game.addPlayer();
    runFor(game, 5.f);
    CHECK(game.shopOpen());
    game.openLobby();
    CHECK(!game.shopOpen());
}
