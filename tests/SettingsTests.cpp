#include <cstddef>
#include "logic/Settings.hpp"
#include "TestHarness.hpp"

using namespace rtype;

TEST(the_default_settings_are_normal_without_friendly_fire)
{
    const Settings settings;

    CHECK(settings.difficulty == Difficulty::Normal);
    CHECK(!settings.friendlyFire);
}

TEST(the_normal_difficulty_changes_nothing)
{
    const DifficultyScale &scale = scaleOf(Difficulty::Normal);

    CHECK(scale.economy == 1.f);
    CHECK(scale.enemyHp == 1.f);
    CHECK(scale.enemyDamage == 1.f);
    CHECK(scale.allyDamage == 1.f);
    CHECK(scale.enemyFireRate == 1.f);
    CHECK(scale.density == 1.f);
}

TEST(the_difficulty_makes_the_enemies_stronger_step_by_step)
{
    for (std::size_t i = 1; i < DIFFICULTY_COUNT; ++i) {
        const DifficultyScale &easier = DIFFICULTY_SCALES[i - 1];
        const DifficultyScale &harder = DIFFICULTY_SCALES[i];

        CHECK(harder.enemyHp > easier.enemyHp);
        CHECK(harder.enemyDamage > easier.enemyDamage);
        CHECK(harder.enemyFireRate > easier.enemyFireRate);
        CHECK(harder.density > easier.density);
        CHECK(harder.economy < easier.economy);
        CHECK(harder.shopLevelBonus > easier.shopLevelBonus);
        CHECK(harder.allyDamage < easier.allyDamage);
        CHECK(harder.lives < easier.lives);
    }
}

TEST(the_brutal_difficulty_leaves_a_single_life)
{
    CHECK(scaleOf(Difficulty::Brutal).lives == 1);
}

TEST(only_the_four_difficulties_are_valid)
{
    CHECK(isValidDifficulty(0));
    CHECK(isValidDifficulty(3));
    CHECK(!isValidDifficulty(4));
    CHECK(!isValidDifficulty(255));
}

TEST(the_scale_of_the_settings_follows_their_difficulty)
{
    const Settings settings{Difficulty::Hard, true};

    CHECK(&scaleOf(settings) == &scaleOf(Difficulty::Hard));
}

TEST(the_difficulty_can_be_shifted_but_never_leaves_its_range)
{
    CHECK(shiftedDifficulty(Difficulty::Normal, 1) == Difficulty::Hard);
    CHECK(shiftedDifficulty(Difficulty::Normal, -1) == Difficulty::Easy);
    CHECK(shiftedDifficulty(Difficulty::Easy, -1) == Difficulty::Easy);
    CHECK(shiftedDifficulty(Difficulty::Brutal, 1) == Difficulty::Brutal);
}
