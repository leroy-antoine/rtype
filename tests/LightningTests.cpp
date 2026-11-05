#include <vector>
#include "logic/Game.hpp"
#include "logic/Lightning.hpp"
#include "TestHarness.hpp"

using namespace rtype;

namespace {
    Engine::Entity ship(Engine::Registry &reg)
    {
        const Engine::Entity player = createPlayer(reg, 0);

        reg.get<gameLogic::Vec2Pos>(player) =
            gameLogic::Vec2Pos{{100.f, 400.f}};
        return player;
    }

    Engine::Entity tankAt(Engine::Registry &reg, const float x, const float y)
    {
        return spawnMonsterAt(reg, x, y, MonsterKind::Tank);
    }

    int hpOf(Engine::Registry &reg, const Engine::Entity e)
    {
        return reg.get<Health>(e).hp;
    }

    int segments(Engine::Registry &reg)
    {
        int count = 0;

        reg.each<Renderable>(
            [&count](const Engine::Entity, const Renderable &r) {
                if (r.id == SpriteId::PlayerBoltUp
                    || r.id == SpriteId::PlayerBoltDown) {
                    ++count;
                }
            });
        return count;
    }

    const int TANK_HP = monsterDef(MonsterKind::Tank).hp;
    const int FIRST = 15;
}

TEST(the_bolt_strikes_the_first_enemy_in_front_of_the_ship)
{
    Engine::Registry reg;
    const Engine::Entity player = ship(reg);
    const Engine::Entity near = tankAt(reg, 400.f, 400.f);
    const Engine::Entity far = tankAt(reg, 700.f, 400.f);

    CHECK(fireLightning(reg, player));
    CHECK(hpOf(reg, near) == TANK_HP - FIRST);
    CHECK(hpOf(reg, far) == TANK_HP);
}

TEST(an_enemy_off_the_line_of_the_ship_is_missed)
{
    Engine::Registry reg;
    const Engine::Entity player = ship(reg);
    const Engine::Entity aside = tankAt(reg, 400.f, 700.f);

    CHECK(!fireLightning(reg, player));
    CHECK(hpOf(reg, aside) == TANK_HP);
    CHECK(segments(reg) == 0);
}

TEST(nothing_is_fired_at_an_enemy_out_of_range_or_behind)
{
    Engine::Registry reg;
    const Engine::Entity player = ship(reg);

    tankAt(reg, 100.f + config::PLAYER_WIDTH + config::LIGHTNING_RANGE + 50.f,
        400.f);
    CHECK(!fireLightning(reg, player));
    tankAt(reg, 0.f, 400.f);
    CHECK(!fireLightning(reg, player));
}

TEST(the_bolt_jumps_to_enemies_stuck_to_the_first_one)
{
    Engine::Registry reg;
    const Engine::Entity player = ship(reg);
    const Engine::Entity first = tankAt(reg, 400.f, 400.f);
    const Engine::Entity second = tankAt(reg, 500.f, 450.f);
    const Engine::Entity third = tankAt(reg, 590.f, 500.f);

    fireLightning(reg, player);
    CHECK(hpOf(reg, first) == TANK_HP - FIRST);
    CHECK(hpOf(reg, second) == TANK_HP - 12);
    CHECK(hpOf(reg, third) == TANK_HP - 10);
}

TEST(the_bolt_does_not_jump_to_an_enemy_that_is_not_stuck)
{
    Engine::Registry reg;
    const Engine::Entity player = ship(reg);
    const Engine::Entity first = tankAt(reg, 400.f, 400.f);
    const Engine::Entity apart = tankAt(reg, 800.f, 400.f);

    fireLightning(reg, player);
    CHECK(hpOf(reg, first) == TANK_HP - FIRST);
    CHECK(hpOf(reg, apart) == TANK_HP);
}

TEST(each_enemy_is_struck_once_and_the_chain_is_limited)
{
    Engine::Registry reg;
    const Engine::Entity player = ship(reg);
    std::vector<Engine::Entity> row;

    for (int i = 0; i < 8; ++i) {
        row.push_back(tankAt(reg, 400.f + 85.f * static_cast<float>(i), 400.f));
    }
    fireLightning(reg, player);
    int struck = 0;

    for (const Engine::Entity e : row) {
        if (hpOf(reg, e) < TANK_HP) {
            ++struck;
        }
    }
    CHECK(struck == config::LIGHTNING_JUMPS + 1);
}

TEST(the_chain_goes_on_through_an_enemy_the_bolt_just_killed)
{
    Engine::Registry reg;
    const Engine::Entity player = ship(reg);
    const Engine::Entity weak = spawnMonsterAt(reg, 400.f, 400.f,
        MonsterKind::Kamikaze);
    const Engine::Entity behind = tankAt(reg, 490.f, 420.f);

    reg.get<Health>(weak).hp = 1;
    fireLightning(reg, player);
    CHECK(hpOf(reg, weak) <= 0);
    CHECK(hpOf(reg, behind) < TANK_HP);
}

TEST(the_bolt_never_hurts_a_teammate_on_its_line)
{
    Engine::Registry reg;
    const Engine::Entity player = ship(reg);
    const Engine::Entity mate = createPlayer(reg, 1);

    reg.get<gameLogic::Vec2Pos>(mate) = gameLogic::Vec2Pos{{300.f, 400.f}};
    const Engine::Entity monster = tankAt(reg, 500.f, 400.f);

    fireLightning(reg, player);
    CHECK(hpOf(reg, mate) == config::PLAYER_HP);
    CHECK(hpOf(reg, monster) == TANK_HP - FIRST);
}

TEST(every_jump_draws_a_segment_that_disappears_quickly)
{
    Engine::Registry reg;
    const Engine::Entity player = ship(reg);

    tankAt(reg, 400.f, 400.f);
    tankAt(reg, 490.f, 420.f);
    fireLightning(reg, player);
    CHECK(segments(reg) == 2);
    lifetimeSystem(reg, config::LIGHTNING_DURATION + 0.1f);
    CHECK(segments(reg) == 0);
}

TEST(a_segment_going_up_and_one_going_down_have_their_own_sprite)
{
    Engine::Registry reg;

    createBolt(reg, {0.f, 0.f}, {100.f, 50.f});
    createBolt(reg, {0.f, 50.f}, {100.f, 0.f});
    CHECK(segments(reg) == 2);
    int rising = 0;

    reg.each<Renderable>([&rising](const Engine::Entity, const Renderable &r) {
        if (r.id == SpriteId::PlayerBoltUp) {
            ++rising;
        }
    });
    CHECK(rising == 1);
}

TEST(holding_x_fires_the_bolt_at_the_pace_of_the_recharge_only_with_a_target)
{
    Engine::Registry reg;
    const Engine::Entity player = ship(reg);

    equipSecondary(reg, player, WeaponKind::Lightning);
    reg.get<PlayerInput>(player).alt = true;
    secondarySystem(reg, config::FIXED_DT);
    CHECK(reg.get<SecondaryWeapon>(player).timer == 0.f);
    const Engine::Entity monster = tankAt(reg, 400.f, 400.f);

    secondarySystem(reg, config::FIXED_DT);
    CHECK(hpOf(reg, monster) < TANK_HP);
    CHECK(reg.get<SecondaryWeapon>(player).timer > 0.f);
}

TEST(the_bolt_ignores_an_enemy_that_has_not_entered_the_map)
{
    Engine::Registry reg;
    const Engine::Entity player = ship(reg);

    tankAt(reg, config::MAP_WIDTH + 10.f, 400.f);
    CHECK(!fireLightning(reg, player));
}
