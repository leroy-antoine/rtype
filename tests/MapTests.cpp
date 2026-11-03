#include <cstddef>
#include "logic/Builder.hpp"
#include "logic/Levels.hpp"
#include "logic/Zones.hpp"
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

TEST(the_map_has_twenty_levels)
{
    const std::optional<Segments> run = buildMap(0);

    CHECK(map1().size() == 20);
    CHECK(run.has_value());
    CHECK(run->levels.size() == 20);
    CHECK(run->levels.back().first == 20);
}

TEST(the_walls_of_a_map_are_sorted_by_distance)
{
    const std::optional<Segments> run = buildMap(0);

    CHECK(run.has_value());
    CHECK(!run->obstacles.empty());
    for (std::size_t i = 1; i < run->obstacles.size(); ++i) {
        CHECK(run->obstacles[i - 1].at <= run->obstacles[i].at);
    }
}

TEST(everything_happens_before_the_end_of_its_level)
{
    for (const LevelDef &level : map1()) {
        for (const WaveStep &step : level.waves) {
            CHECK(step.at < level.length);
        }
        for (const BossSpawn &boss : level.bosses) {
            CHECK(boss.second < level.length);
        }
        for (const ObstacleStep &step : level.obstacles) {
            CHECK(step.at < level.length);
        }
    }
}

TEST(the_death_runs_have_walls_and_no_enemies)
{
    const std::vector<LevelDef> levels = map1();

    for (const std::size_t index : {9u, 14u}) {
        CHECK(levels[index].waves.empty());
        CHECK(levels[index].bosses.empty());
        CHECK(levels[index].obstacles.size() >= 20);
    }
}

TEST(a_gate_always_leaves_room_for_a_ship)
{
    for (const LevelDef &level : map1()) {
        for (std::size_t i = 1; i < level.obstacles.size(); ++i) {
            const ObstacleStep &top = level.obstacles[i - 1];
            const ObstacleStep &bottom = level.obstacles[i];

            if (top.at == bottom.at && !isPortal(top.kind)
                && top.amplitude == bottom.amplitude) {
                CHECK(bottom.y - (top.y + top.height)
                    >= 3.f * config::PLAYER_HEIGHT);
            }
        }
    }
}

TEST(the_last_levels_have_more_bosses_than_the_first_ones)
{
    const std::vector<LevelDef> levels = map1();
    std::size_t first = 0;
    std::size_t last = 0;

    for (std::size_t i = 0; i < 5; ++i) {
        first += levels[i].bosses.size();
        last += levels[levels.size() - 1 - i].bosses.size();
    }
    CHECK(last > first);
}

TEST(a_gate_has_a_wall_above_and_below_its_gap)
{
    std::vector<ObstacleStep> walls;

    addGate(walls, {100.f, 400.f, 200.f, 80.f});
    CHECK(walls.size() == 2);
    CHECK(walls[0].y == 0.f);
    CHECK(walls[0].height == 400.f);
    CHECK(walls[1].y == 600.f);
    CHECK(walls[1].height == config::MAP_HEIGHT - 600.f);
    CHECK(walls[1].at == 100.f);
}

TEST(a_shifted_wall_moves_along_the_distance_only)
{
    const ObstacleStep moved = shifted(wall(100.f, 50.f, 80.f, 60.f), 2500.f);

    CHECK(moved.at == 2600.f);
    CHECK(moved.y == 50.f);
    CHECK(moved.width == 80.f);
}

TEST(no_wall_is_still_on_screen_when_a_boss_appears)
{
    const std::optional<Segments> run = buildMap(0);

    CHECK(run.has_value());
    for (const auto &boss : run->bossSpawn) {
        for (const ObstacleStep &step : run->obstacles) {
            const bool before = step.at < boss.second;
            const float gone = step.at + config::MAP_WIDTH + step.width;

            CHECK(!before || gone <= boss.second);
        }
    }
}

TEST(every_map_builds_and_is_sorted)
{
    CHECK(MAP_NAMES.size() == MAPS.size());
    for (std::size_t id = 0; id < MAPS.size(); ++id) {
        const std::optional<Segments> run = buildMap(id);

        CHECK(run.has_value());
        CHECK(!run->levels.empty());
        for (std::size_t i = 1; i < run->waves.size(); ++i) {
            CHECK(run->waves[i - 1].at <= run->waves[i].at);
        }
        for (std::size_t i = 1; i < run->bossSpawn.size(); ++i) {
            CHECK(run->bossSpawn[i - 1].second <= run->bossSpawn[i].second);
        }
        for (std::size_t i = 1; i < run->obstacles.size(); ++i) {
            CHECK(run->obstacles[i - 1].at <= run->obstacles[i].at);
        }
    }
}

TEST(everything_in_every_map_happens_before_the_end_of_its_level)
{
    for (const MapFn map : MAPS) {
        for (const LevelDef &level : map()) {
            for (const WaveStep &step : level.waves) {
                CHECK(step.at < level.length);
            }
            for (const BossSpawn &boss : level.bosses) {
                CHECK(boss.second < level.length);
            }
            for (const ObstacleStep &step : level.obstacles) {
                CHECK(step.at < level.length);
            }
        }
    }
}

TEST(every_map_ends_with_a_boss_and_keeps_its_gates_passable)
{
    for (const MapFn map : MAPS) {
        const std::vector<LevelDef> levels = map();

        CHECK(!levels.back().bosses.empty());
        for (const LevelDef &level : levels) {
            for (std::size_t i = 1; i < level.obstacles.size(); ++i) {
                const ObstacleStep &top = level.obstacles[i - 1];
                const ObstacleStep &bottom = level.obstacles[i];

                if (top.at == bottom.at && !isPortal(top.kind)
                    && top.amplitude == bottom.amplitude) {
                    CHECK(bottom.y - (top.y + top.height)
                        >= 3.f * config::PLAYER_HEIGHT);
                }
            }
        }
    }
}

TEST(no_wall_is_on_screen_when_a_boss_appears_in_any_map)
{
    for (std::size_t id = 0; id < MAPS.size(); ++id) {
        const std::optional<Segments> run = buildMap(id);

        for (const auto &boss : run->bossSpawn) {
            for (const ObstacleStep &step : run->obstacles) {
                const float gone = step.at + config::MAP_WIDTH + step.width;

                CHECK(step.at >= boss.second || gone <= boss.second);
            }
        }
    }
}

TEST(the_death_runs_map_has_only_walls_before_its_boss_level)
{
    const std::vector<LevelDef> levels = map3();

    for (std::size_t i = 0; i + 1 < levels.size(); ++i) {
        CHECK(levels[i].waves.empty());
        CHECK(levels[i].bosses.empty());
        CHECK(levels[i].obstacles.size() >= 12);
    }
}

TEST(every_level_gives_back_the_normal_controls_and_speed)
{
    for (const MapFn map : MAPS) {
        for (const LevelDef &level : map()) {
            int reverses = 0;
            bool fast = false;

            for (const ObstacleStep &step : level.obstacles) {
                if (step.kind == ObstacleKind::ReversePortal) {
                    ++reverses;
                }
                if (step.kind == ObstacleKind::FastPortal) {
                    fast = true;
                }
                if (step.kind == ObstacleKind::NormalPortal) {
                    fast = false;
                }
            }
            CHECK(reverses % 2 == 0);
            CHECK(!fast);
        }
    }
}

TEST(the_death_runs_have_portals_and_moving_gates)
{
    const std::vector<LevelDef> levels = map3();
    int moving = 0;
    int portals = 0;

    for (const LevelDef &level : levels) {
        for (const ObstacleStep &step : level.obstacles) {
            moving += step.amplitude > 0.f;
            portals += isPortal(step.kind);
        }
    }
    CHECK(moving > 0);
    CHECK(portals >= 12);
}

TEST(a_moving_gate_keeps_a_wall_above_and_below_during_its_swing)
{
    std::vector<ObstacleStep> walls;

    addGate(walls, {100.f, 400.f, 300.f, 80.f, ObstacleKind::Spike,
        {90.f, 3.f}});
    CHECK(walls.size() == 2);
    CHECK(walls[0].y + walls[0].amplitude <= 0.f + 1.f);
    CHECK(walls[1].y + walls[1].height - walls[1].amplitude
        >= config::MAP_HEIGHT - 1.f);
}

TEST(a_portal_crosses_the_whole_height_of_the_map)
{
    const ObstacleStep gate = portal(300.f, ObstacleKind::FastPortal);

    CHECK(gate.y == 0.f);
    CHECK(gate.height == config::MAP_HEIGHT);
    CHECK(gate.width == config::PORTAL_WIDTH);
}

TEST(each_zone_has_a_theme_and_the_death_runs_are_in_the_warp)
{
    CHECK(zoneOf(0, 1) == Zone::DeepSpace);
    CHECK(zoneOf(0, 10) == Zone::Warp);
    CHECK(zoneOf(0, 15) == Zone::Warp);
    CHECK(zoneOf(0, 20) == Zone::Core);
    CHECK(zoneOf(0, 99) == Zone::Core);
    CHECK(zoneOf(1, 3) == Zone::Arena);
    CHECK(zoneOf(2, 1) == Zone::Warp);
    CHECK(zoneOf(2, 5) == Zone::Core);
}

TEST(a_pinching_gate_never_closes_below_two_and_a_half_ships)
{
    for (const MapFn map : MAPS) {
        for (const LevelDef &level : map()) {
            for (std::size_t i = 1; i < level.obstacles.size(); ++i) {
                const ObstacleStep &top = level.obstacles[i - 1];
                const ObstacleStep &bottom = level.obstacles[i];

                if (top.at == bottom.at && !isPortal(top.kind)
                    && top.amplitude == -bottom.amplitude
                    && top.amplitude != 0.f) {
                    CHECK(bottom.y - (top.y + top.height)
                        - 2.f * std::abs(top.amplitude)
                        >= 2.5f * config::PLAYER_HEIGHT);
                }
            }
        }
    }
}

TEST(the_walls_of_the_death_runs_come_faster_than_the_scrolling)
{
    int fast = 0;

    for (const LevelDef &level : map3()) {
        for (const ObstacleStep &step : level.obstacles) {
            fast += step.speed > 1.f;
        }
    }
    CHECK(fast > 100);
}

TEST(a_gate_can_pinch_and_come_faster)
{
    std::vector<ObstacleStep> walls;

    addGate(walls, {100.f, 400.f, 300.f, 80.f, ObstacleKind::Spike,
        {50.f, 2.f}, 1.5f, true});
    CHECK(walls.size() == 2);
    CHECK(walls[0].amplitude == 50.f);
    CHECK(walls[1].amplitude == -50.f);
    CHECK(walls[0].speed == 1.5f);
}
