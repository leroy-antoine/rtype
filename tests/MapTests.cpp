#include <cstddef>
#include "logic/Builder.hpp"
#include "logic/Levels.hpp"
#include "TestHarness.hpp"

using namespace rtype;

TEST(the_first_level_of_the_first_map_is_the_original_level)
{
    const std::optional<Segments> run = buildMap(0);
    const Segments original = firstLevel();

    CHECK(run.has_value());
    CHECK(run->waves.size() >= original.waves.size());
    for (std::size_t i = 0; i < original.waves.size(); ++i) {
        CHECK(run->waves[i].at == original.waves[i].at);
        CHECK(run->waves[i].count == original.waves[i].count);
        CHECK(run->waves[i].y == original.waves[i].y);
        CHECK(run->waves[i].dy == original.waves[i].dy);
        CHECK(run->waves[i].interval == original.waves[i].interval);
    }
    CHECK(run->bossSpawn[0] == original.bossSpawn[0]);
}

TEST(the_levels_of_a_map_follow_each_other)
{
    const std::vector<LevelDef> levels = map1();
    const std::optional<Segments> run = buildMap(0);

    CHECK(run.has_value());
    CHECK(run->levels.size() == levels.size());
    CHECK(run->levels[0].first == 1);
    CHECK(run->levels[0].second == 0.f);
    CHECK(run->levels[1].first == 2);
    CHECK(run->levels[1].second == levels[0].length);
    CHECK(levelAt(*run, levels[0].length - 1.f) == 1);
    CHECK(levelAt(*run, levels[0].length) == 2);
}

TEST(the_waves_and_bosses_of_a_map_are_sorted_by_distance)
{
    const std::optional<Segments> run = buildMap(0);

    CHECK(run.has_value());
    for (std::size_t i = 1; i < run->waves.size(); ++i) {
        CHECK(run->waves[i - 1].at <= run->waves[i].at);
    }
    for (std::size_t i = 1; i < run->bossSpawn.size(); ++i) {
        CHECK(run->bossSpawn[i - 1].second <= run->bossSpawn[i].second);
    }
}

TEST(the_last_level_of_a_map_has_a_boss)
{
    const std::optional<Segments> run = buildMap(0);

    CHECK(run.has_value());
    CHECK(!map1().back().bosses.empty());
}

TEST(an_unknown_map_id_builds_nothing)
{
    CHECK(!buildMap(MAPS.size()).has_value());
    CHECK(!buildMap(9999).has_value());
}
