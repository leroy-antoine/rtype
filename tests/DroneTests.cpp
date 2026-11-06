#include <vector>
#include "logic/Drone.hpp"
#include "logic/Game.hpp"
#include "TestHarness.hpp"

using namespace rtype;

namespace {
    Engine::Entity droneShip(Engine::Registry &reg, const int level = 1)
    {
        const Engine::Entity player = createPlayer(reg, 0);

        reg.get<gameLogic::Vec2Pos>(player) =
            gameLogic::Vec2Pos{{100.f, 400.f}};
        reg.add(player, DroneLevel{level});
        return player;
    }

    void run(Engine::Registry &reg, const float seconds)
    {
        for (float t = 0.f; t < seconds; t += config::FIXED_DT) {
            droneSystem(reg, config::FIXED_DT);
        }
    }

    std::vector<Engine::Entity> drones(Engine::Registry &reg)
    {
        return collectWith<DroneBody>(reg);
    }

    Engine::Entity tankAt(Engine::Registry &reg, const float x, const float y)
    {
        return spawnMonsterAt(reg, x, y, MonsterKind::Tank);
    }

    std::size_t bullets(Engine::Registry &reg)
    {
        return collectProjectileEntities(reg).size();
    }
}

TEST(the_three_drone_items_give_the_levels_one_two_and_three)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    applyAugment(reg, player, AugmentId::Drone);
    CHECK(reg.get<DroneLevel>(player).level == 1);
    applyAugment(reg, player, AugmentId::DroneBoost);
    CHECK(reg.get<DroneLevel>(player).level == 2);
    applyAugment(reg, player, AugmentId::DroneTwin);
    CHECK(reg.get<DroneLevel>(player).level == config::DRONE_MAX_LEVEL);
}

TEST(an_expensive_drone_item_bought_alone_gives_its_own_level)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    applyAugment(reg, player, AugmentId::DroneTwin);
    CHECK(reg.get<DroneLevel>(player).level == config::DRONE_MAX_LEVEL);
}

TEST(a_cheaper_drone_item_never_lowers_the_level)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    applyAugment(reg, player, AugmentId::DroneBoost);
    applyAugment(reg, player, AugmentId::Drone);
    CHECK(reg.get<DroneLevel>(player).level == 2);
}

TEST(the_drone_never_perches_outside_the_map)
{
    Engine::Registry reg;
    const Engine::Entity player = droneShip(reg);

    reg.get<gameLogic::Vec2Pos>(player) =
        gameLogic::Vec2Pos{{config::MAP_WIDTH - 150.f, 5.f}};
    run(reg, 3.f);
    const auto &pos = reg.get<gameLogic::Vec2Pos>(drones(reg).front());

    CHECK(pos.x + config::DRONE_SIZE <= config::MAP_WIDTH + 1.f);
    CHECK(pos.y >= 0.f);
}

TEST(the_drone_ignores_an_enemy_that_has_not_entered_the_map)
{
    Engine::Registry reg;

    droneShip(reg);
    tankAt(reg, config::MAP_WIDTH + 20.f, 400.f);
    run(reg, 1.f);
    CHECK(bullets(reg) == 0);
}

TEST(a_player_with_a_drone_level_gets_exactly_one_drone)
{
    Engine::Registry reg;
    const Engine::Entity player = droneShip(reg);

    createPlayer(reg, 1);
    run(reg, 0.5f);
    CHECK(drones(reg).size() == 1);
    CHECK(reg.get<DroneBody>(drones(reg).front()).owner == player);
}

TEST(the_drone_flies_ahead_of_the_ship_and_above_it)
{
    Engine::Registry reg;
    const Engine::Entity player = droneShip(reg);

    run(reg, 2.f);
    const auto &pos = reg.get<gameLogic::Vec2Pos>(drones(reg).front());
    const auto &ship = reg.get<gameLogic::Vec2Pos>(player);

    CHECK(pos.x > ship.x + config::PLAYER_WIDTH);
    CHECK(pos.y < ship.y);
}

TEST(the_drone_follows_the_ship_with_a_delay)
{
    Engine::Registry reg;
    const Engine::Entity player = droneShip(reg);

    run(reg, 0.1f);
    const Engine::Entity drone = drones(reg).front();

    reg.get<gameLogic::Vec2Pos>(player) = gameLogic::Vec2Pos{{100.f, 800.f}};
    run(reg, 0.05f);
    CHECK(reg.get<gameLogic::Vec2Pos>(drone).y
        < 800.f - config::DRONE_OFFSET_Y);
    run(reg, 3.f);
    CHECK(std::fabs(reg.get<gameLogic::Vec2Pos>(drone).y
        - (800.f - config::DRONE_OFFSET_Y)) < 2.f);
}

TEST(the_drone_cannot_be_hit_by_anything)
{
    Engine::Registry reg;

    droneShip(reg);
    run(reg, 0.2f);
    CHECK(!reg.has<Health>(drones(reg).front()));
}

TEST(the_drone_shoots_the_nearest_enemy_in_range_with_half_the_damage)
{
    Engine::Registry reg;

    droneShip(reg);
    tankAt(reg, 600.f, 400.f);
    tankAt(reg, 900.f, 400.f);
    run(reg, 0.1f);
    CHECK(bullets(reg) == 1);
    CHECK(reg.get<Damage>(collectProjectileEntities(reg).front()).value
        == config::PLAYER_DAMAGE / config::DRONE_DAMAGE_DIVISOR);
    CHECK(reg.get<gameLogic::Vec2Velocity>(
        collectProjectileEntities(reg).front()).x > 0.f);
}

TEST(the_drone_does_not_shoot_at_an_enemy_out_of_range)
{
    Engine::Registry reg;

    droneShip(reg);
    tankAt(reg, 100.f + config::DRONE_RANGE + 400.f, 400.f);
    run(reg, 1.f);
    CHECK(bullets(reg) == 0);
}

TEST(the_drone_shoots_at_the_rate_of_its_level)
{
    Engine::Registry reg;

    droneShip(reg, 1);
    tankAt(reg, 600.f, 400.f);
    run(reg, 1.3f);
    CHECK(bullets(reg) == 3);
}

TEST(a_higher_level_drone_shoots_faster)
{
    Engine::Registry slow;
    Engine::Registry fast;

    droneShip(slow, 1);
    droneShip(fast, 2);
    tankAt(slow, 600.f, 400.f);
    tankAt(fast, 600.f, 400.f);
    run(slow, 2.f);
    run(fast, 2.f);
    CHECK(bullets(fast) > bullets(slow));
}

TEST(a_level_three_drone_fires_two_bullets_at_once)
{
    Engine::Registry reg;

    droneShip(reg, 3);
    tankAt(reg, 600.f, 400.f);
    run(reg, 0.1f);
    CHECK(bullets(reg) == 2);
}

TEST(the_drone_disappears_with_its_owner)
{
    Engine::Registry reg;
    const Engine::Entity player = droneShip(reg);

    run(reg, 0.2f);
    reg.destroy(player);
    run(reg, 0.1f);
    CHECK(drones(reg).empty());
}

TEST(a_bought_drone_comes_back_with_its_level_after_a_respawn)
{
    Game game(1);

    game.addPlayer();
    game.addPlayer();
    game.addAugment(0, AugmentId::Drone);
    game.addAugment(0, AugmentId::DroneBoost);
    game.update(config::FIXED_DT);
    CHECK(collectWith<DroneBody>(game.registry()).size() == 1);
    game.registry().get<Health>(*findPlayerBySlot(game.registry(), 0)).hp = 0;
    game.update(config::FIXED_DT);
    game.update(config::FIXED_DT);
    CHECK(collectWith<DroneBody>(game.registry()).empty());
    game.respawnPlayer(0);
    game.update(config::FIXED_DT);
    CHECK(collectWith<DroneBody>(game.registry()).size() == 1);
    CHECK(game.registry().get<DroneLevel>(
        *findPlayerBySlot(game.registry(), 0)).level == 2);
}

TEST(the_drone_augments_are_sold_in_the_shop)
{
    for (const AugmentId id : {AugmentId::Drone, AugmentId::DroneBoost,
        AugmentId::DroneTwin}) {
        const auto def = findAugment(id);

        CHECK(def.has_value());
        CHECK(def->price > 0);
    }
}
