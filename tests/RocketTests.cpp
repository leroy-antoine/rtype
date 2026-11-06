#include <vector>
#include "logic/Game.hpp"
#include "logic/Rocket.hpp"
#include "TestHarness.hpp"

using namespace rtype;

namespace {
    std::vector<Engine::Entity> missiles(Engine::Registry &reg)
    {
        return collectWith<Missile>(reg);
    }

    Engine::Entity monsterAt(Engine::Registry &reg, const float x,
        const float y)
    {
        return spawnMonsterAt(reg, x, y, MonsterKind::Tank);
    }

    Engine::Entity rocketAt(Engine::Registry &reg, const Engine::Entity player,
        const float x, const float y)
    {
        fireRocket(reg, player);
        const Engine::Entity rocket = missiles(reg).front();

        reg.get<gameLogic::Vec2Pos>(rocket) = gameLogic::Vec2Pos{{x, y}};
        return rocket;
    }

    int hpOf(Engine::Registry &reg, const Engine::Entity e)
    {
        return reg.get<Health>(e).hp;
    }

    const int TANK_HP = monsterDef(MonsterKind::Tank).hp;
}

TEST(a_rocket_leaves_the_nose_of_the_ship_with_triple_damage)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    CHECK(fireRocket(reg, player));
    const Engine::Entity rocket = missiles(reg).front();
    const auto &pos = reg.get<gameLogic::Vec2Pos>(rocket);
    const auto &ship = reg.get<gameLogic::Vec2Pos>(player);

    CHECK(missiles(reg).size() == 1);
    CHECK(pos.x == ship.x + config::PLAYER_WIDTH);
    CHECK(reg.get<Damage>(rocket).value
        == config::ROCKET_DAMAGE_FACTOR * config::PLAYER_DAMAGE);
    CHECK(reg.get<gameLogic::Vec2Velocity>(rocket).x == config::ROCKET_SPEED);
    CHECK(reg.get<Team>(rocket).id == TeamId::Player);
}

TEST(a_rocket_is_slower_than_a_normal_bullet)
{
    CHECK(config::ROCKET_SPEED < config::BULLET_SPEED);
}

TEST(the_rocket_damage_follows_the_damage_of_the_ship)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    reg.get<Damage>(player).value += 10;
    fireRocket(reg, player);
    CHECK(reg.get<Damage>(missiles(reg).front()).value
        == config::ROCKET_DAMAGE_FACTOR * (config::PLAYER_DAMAGE + 10));
}

TEST(a_rocket_explodes_on_the_enemy_it_touches_and_vanishes)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity monster = monsterAt(reg, 600.f, 400.f);

    rocketAt(reg, player, 590.f, 430.f);
    rocketSystem(reg);
    CHECK(missiles(reg).empty());
    CHECK(hpOf(reg, monster) == TANK_HP
        - config::ROCKET_DAMAGE_FACTOR * config::PLAYER_DAMAGE);
}

TEST(the_blast_hurts_every_enemy_in_its_radius_and_only_them)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity hit = monsterAt(reg, 600.f, 400.f);
    const Engine::Entity near = monsterAt(reg, 600.f, 520.f);
    const Engine::Entity far = monsterAt(reg, 900.f, 400.f);

    rocketAt(reg, player, 590.f, 430.f);
    rocketSystem(reg);
    CHECK(hpOf(reg, hit) < TANK_HP);
    CHECK(hpOf(reg, near) < TANK_HP);
    CHECK(hpOf(reg, far) == TANK_HP);
}

TEST(the_blast_never_hurts_the_players)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity other = createPlayer(reg, 1);

    monsterAt(reg, 600.f, 400.f);
    reg.get<gameLogic::Vec2Pos>(other) = gameLogic::Vec2Pos{{650.f, 420.f}};
    rocketAt(reg, player, 590.f, 430.f);
    rocketSystem(reg);
    CHECK(hpOf(reg, other) == config::PLAYER_HP);
    CHECK(hpOf(reg, player) == config::PLAYER_HP);
}

TEST(a_rocket_in_the_open_flies_on)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    monsterAt(reg, 1500.f, 400.f);
    rocketAt(reg, player, 400.f, 700.f);
    rocketSystem(reg);
    CHECK(missiles(reg).size() == 1);
}

TEST(a_rocket_explodes_on_a_wall)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity monster = monsterAt(reg, 700.f, 700.f);
    ObstacleStep step{};

    step.kind = ObstacleKind::Rock;
    step.y = 300.f;
    step.width = 64.f;
    step.height = 64.f;
    const Engine::Entity wall = createObstacle(reg, step);

    reg.get<gameLogic::Vec2Pos>(wall) = gameLogic::Vec2Pos{{600.f, 300.f}};
    rocketAt(reg, player, 590.f, 320.f);
    rocketSystem(reg);
    CHECK(missiles(reg).empty());
    CHECK(hpOf(reg, monster) == TANK_HP);
}

TEST(a_rocket_explodes_at_the_edge_of_the_map)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    rocketAt(reg, player, config::MAP_WIDTH + 1.f, 300.f);
    rocketSystem(reg);
    CHECK(missiles(reg).empty());
}

TEST(an_explosion_is_drawn_where_the_rocket_blew_up)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    monsterAt(reg, 600.f, 400.f);
    rocketAt(reg, player, 590.f, 430.f);
    rocketSystem(reg);
    CHECK(!collectWith<Lifetime>(reg).empty());
}

TEST(holding_x_launches_rockets_at_the_pace_of_the_recharge)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    equipSecondary(reg, player, WeaponKind::Rocket);
    reg.get<PlayerInput>(player).alt = true;
    secondarySystem(reg, config::FIXED_DT);
    secondarySystem(reg, config::FIXED_DT);
    CHECK(missiles(reg).size() == 1);
    secondarySystem(reg, config::ROCKET_COOLDOWN);
    CHECK(missiles(reg).size() == 2);
}

TEST(a_player_without_a_secondary_weapon_fires_nothing_with_x)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    reg.get<PlayerInput>(player).alt = true;
    secondarySystem(reg, 1.f);
    CHECK(missiles(reg).empty());
}

TEST(a_bought_launcher_fires_in_the_game_and_kills_a_group)
{
    Game game(1);
    PlayerInput keys;

    game.addPlayer();
    game.addAugment(0, AugmentId::Rocket);
    keys.alt = true;
    game.setInput(0, keys);
    game.update(config::FIXED_DT);
    CHECK(missiles(game.registry()).size() == 1);
}

TEST(a_rocket_passing_close_to_an_enemy_explodes_next_to_it)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity monster = monsterAt(reg, 600.f, 400.f);

    rocketAt(reg, player, 590.f, 400.f + 80.f + 40.f);
    rocketSystem(reg);
    CHECK(missiles(reg).empty());
    CHECK(hpOf(reg, monster) < TANK_HP);
}

TEST(a_rocket_passing_far_from_an_enemy_flies_on)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity monster = monsterAt(reg, 600.f, 400.f);

    rocketAt(reg, player, 590.f, 400.f + 80.f + 120.f);
    rocketSystem(reg);
    CHECK(missiles(reg).size() == 1);
    CHECK(hpOf(reg, monster) == TANK_HP);
}

TEST(a_rocket_ignores_an_enemy_that_has_not_entered_the_map)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    monsterAt(reg, config::MAP_WIDTH + 30.f, 400.f);
    rocketAt(reg, player, config::MAP_WIDTH - 100.f, 420.f);
    rocketSystem(reg);
    CHECK(missiles(reg).size() == 1);
}
