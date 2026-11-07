#include <vector>
#include "logic/Charge.hpp"
#include "logic/Game.hpp"
#include "TestHarness.hpp"

using namespace rtype;

namespace {
    void hold(Engine::Registry &reg, const Engine::Entity player,
        const float seconds)
    {
        reg.get<PlayerInput>(player).fire = true;
        for (float t = 0.f; t < seconds; t += config::FIXED_DT) {
            chargeSystem(reg, config::FIXED_DT);
        }
    }

    void release(Engine::Registry &reg, const Engine::Entity player)
    {
        reg.get<PlayerInput>(player).fire = false;
        chargeSystem(reg, config::FIXED_DT);
    }

    Engine::Entity theBullet(Engine::Registry &reg)
    {
        return collectProjectileEntities(reg).front();
    }
}

TEST(nothing_is_fired_while_the_key_is_held)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    hold(reg, player, 1.f);
    CHECK(collectProjectileEntities(reg).empty());
}

TEST(a_short_press_fires_one_normal_shot_on_release)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    hold(reg, player, 0.1f);
    release(reg, player);
    CHECK(collectProjectileEntities(reg).size() == 1);
    CHECK(!reg.has<Pierce>(theBullet(reg)));
    CHECK(reg.get<Damage>(theBullet(reg)).value == config::PLAYER_DAMAGE);
}

TEST(a_tap_during_the_cooldown_fires_nothing)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    hold(reg, player, 0.05f);
    release(reg, player);
    hold(reg, player, 0.05f);
    release(reg, player);
    CHECK(collectProjectileEntities(reg).size() == 1);
}

TEST(a_long_hold_fires_one_piercing_shot_stronger_than_a_tap)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    hold(reg, player, 0.6f);
    release(reg, player);
    const Engine::Entity bullet = theBullet(reg);

    CHECK(collectProjectileEntities(reg).size() == 1);
    CHECK(reg.has<Pierce>(bullet));
    CHECK(reg.get<Damage>(bullet).value > 2 * config::PLAYER_DAMAGE);
    CHECK(reg.get<Damage>(bullet).value < 6 * config::PLAYER_DAMAGE);
    CHECK(reg.get<gameLogic::Vec2Hitbox>(bullet).x
        > 2.f * config::BULLET_WIDTH);
}

TEST(a_full_charge_does_six_times_the_damage_and_four_times_the_size)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    hold(reg, player, 3.f);
    release(reg, player);
    const Engine::Entity bullet = theBullet(reg);

    CHECK(reg.get<Damage>(bullet).value == 6 * config::PLAYER_DAMAGE);
    CHECK(reg.get<gameLogic::Vec2Hitbox>(bullet).x
        == 4.f * config::BULLET_WIDTH);
    CHECK(reg.get<gameLogic::Vec2Hitbox>(bullet).y
        == 4.f * config::BULLET_HEIGHT);
}

TEST(the_charged_shot_stays_centered_on_the_ship)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    hold(reg, player, 3.f);
    release(reg, player);
    const Engine::Entity bullet = theBullet(reg);
    const auto &ship = reg.get<gameLogic::Vec2Pos>(player);
    const auto &pos = reg.get<gameLogic::Vec2Pos>(bullet);
    const auto &box = reg.get<gameLogic::Vec2Hitbox>(bullet);

    CHECK(pos.y + box.y / 2.f
        == ship.y + config::PLAYER_HEIGHT / 2.f);
}

TEST(the_charge_starts_again_from_zero_after_a_shot)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    hold(reg, player, 1.f);
    release(reg, player);
    CHECK(reg.get<Charge>(player).time == 0.f);
}

TEST(the_charge_ratio_goes_from_nothing_to_full)
{
    CHECK(chargeRatio(0.f) == 0.f);
    CHECK(chargeRatio(config::CHARGE_TAP_LIMIT) == 0.f);
    CHECK(chargeRatio(config::CHARGE_FULL_TIME) == 1.f);
    CHECK(chargeRatio(10.f) == 1.f);
    CHECK(!isCharging(0.2f));
    CHECK(isCharging(0.3f));
}

TEST(a_charged_shot_pierces_every_enemy_on_its_way_once)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity first = spawnMonsterAt(reg, 300.f, 200.f,
        MonsterKind::Tank);
    const Engine::Entity second = spawnMonsterAt(reg, 330.f, 200.f,
        MonsterKind::Tank);

    reg.get<gameLogic::Vec2Pos>(player).y = 210.f;
    hold(reg, player, 3.f);
    release(reg, player);
    const Engine::Entity bullet = theBullet(reg);
    const int tank = monsterDef(MonsterKind::Tank).hp;

    reg.get<gameLogic::Vec2Pos>(bullet) = gameLogic::Vec2Pos{{310.f, 200.f}};
    collisionSystem(reg);
    collisionSystem(reg);
    CHECK(reg.alive(bullet));
    CHECK(reg.get<Health>(first).hp == tank - 6 * config::PLAYER_DAMAGE);
    CHECK(reg.get<Health>(second).hp == tank - 6 * config::PLAYER_DAMAGE);
}

TEST(a_normal_shot_stops_at_the_first_enemy)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity monster = spawnMonsterAt(reg, 300.f, 200.f,
        MonsterKind::Tank);

    reg.get<gameLogic::Vec2Pos>(player).y = 210.f;
    hold(reg, player, 0.05f);
    release(reg, player);
    const Engine::Entity bullet = theBullet(reg);

    reg.get<gameLogic::Vec2Pos>(bullet) = gameLogic::Vec2Pos{{310.f, 200.f}};
    collisionSystem(reg);
    CHECK(!reg.alive(bullet));
    CHECK(reg.get<Health>(monster).hp
        == monsterDef(MonsterKind::Tank).hp - config::PLAYER_DAMAGE);
}

TEST(pausing_the_game_loses_the_charge)
{
    Game game(1);

    game.addPlayer();
    const Engine::Entity player = *findPlayerBySlot(game.registry(), 0);

    game.registry().get<Charge>(player).time = 1.f;
    game.setPaused(true);
    CHECK(game.registry().get<Charge>(player).time == 0.f);
}

TEST(the_game_fires_a_tap_and_a_charged_shot_from_the_keys)
{
    Game game(1);
    PlayerInput keys;

    game.addPlayer();
    keys.fire = true;
    game.setInput(0, keys);
    for (int i = 0; i < 10; ++i) {
        game.update(config::FIXED_DT);
    }
    CHECK(collectProjectileEntities(game.registry()).empty());
    keys.fire = false;
    game.setInput(0, keys);
    game.update(config::FIXED_DT);
    CHECK(collectProjectileEntities(game.registry()).size() == 1);
}

TEST(a_charged_shot_hurts_more_than_a_burst_of_taps_over_the_same_time)
{
    const float full = chargeFactor(config::CHARGE_FULL_TIME);
    const float tapsInTheSameTime = config::CHARGE_FULL_TIME
        / config::PLAYER_FIRE_COOLDOWN;

    const auto damage = static_cast<float>(config::PLAYER_DAMAGE);

    CHECK(full * damage >= tapsInTheSameTime * damage * 1.2f);
}
