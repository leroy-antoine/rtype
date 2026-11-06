#include <vector>
#include "logic/Game.hpp"
#include "logic/Laser.hpp"
#include "TestHarness.hpp"

using namespace rtype;

namespace {
    Engine::Entity laserShip(Engine::Registry &reg)
    {
        const Engine::Entity player = createPlayer(reg, 0);

        equipSecondary(reg, player, WeaponKind::Laser);
        reg.get<gameLogic::Vec2Pos>(player) =
            gameLogic::Vec2Pos{{100.f, 400.f}};
        return player;
    }

    void holdX(Engine::Registry &reg, const Engine::Entity player,
        const float seconds)
    {
        reg.get<PlayerInput>(player).alt = true;
        for (float t = 0.f; t < seconds; t += config::FIXED_DT) {
            secondarySystem(reg, config::FIXED_DT);
        }
    }

    void releaseX(Engine::Registry &reg, const Engine::Entity player)
    {
        reg.get<PlayerInput>(player).alt = false;
        secondarySystem(reg, config::FIXED_DT);
    }

    std::vector<Engine::Entity> beams(Engine::Registry &reg)
    {
        std::vector<Engine::Entity> found;

        reg.each<Renderable>([&](const Engine::Entity e, const Renderable &r) {
            if (r.id == SpriteId::PlayerLaser) {
                found.push_back(e);
            }
        });
        return found;
    }

    Engine::Entity tankAt(Engine::Registry &reg, const float x, const float y)
    {
        return spawnMonsterAt(reg, x, y, MonsterKind::Tank);
    }

    const int TANK_HP = monsterDef(MonsterKind::Tank).hp;
    const int BIG_HIT = config::LASER_DAMAGE_FACTOR * config::PLAYER_DAMAGE;
}

TEST(nothing_is_fired_while_the_laser_charges)
{
    Engine::Registry reg;
    const Engine::Entity player = laserShip(reg);

    holdX(reg, player, config::LASER_CHARGE_TIME + 1.f);
    CHECK(beams(reg).empty());
}

TEST(releasing_x_too_early_cancels_the_laser_without_cooldown)
{
    Engine::Registry reg;
    const Engine::Entity player = laserShip(reg);

    holdX(reg, player, config::LASER_CHARGE_TIME - 0.5f);
    releaseX(reg, player);
    CHECK(beams(reg).empty());
    CHECK(reg.get<SecondaryWeapon>(player).charge == 0.f);
    CHECK(reg.get<SecondaryWeapon>(player).timer == 0.f);
}

TEST(releasing_x_after_the_full_charge_fires_the_laser)
{
    Engine::Registry reg;
    const Engine::Entity player = laserShip(reg);

    holdX(reg, player, config::LASER_CHARGE_TIME + 0.1f);
    releaseX(reg, player);
    CHECK(beams(reg).size() == 1);
    CHECK(reg.get<SecondaryWeapon>(player).timer == config::LASER_COOLDOWN);
}

TEST(the_laser_hits_every_enemy_on_its_line_for_heavy_damage)
{
    Engine::Registry reg;
    const Engine::Entity player = laserShip(reg);
    const Engine::Entity near = tankAt(reg, 500.f, 410.f);
    const Engine::Entity far = tankAt(reg, 1500.f, 400.f);
    const Engine::Entity aside = tankAt(reg, 800.f, 700.f);

    holdX(reg, player, config::LASER_CHARGE_TIME);
    releaseX(reg, player);
    CHECK(reg.get<Health>(near).hp == TANK_HP - BIG_HIT);
    CHECK(reg.get<Health>(far).hp == TANK_HP - BIG_HIT);
    CHECK(reg.get<Health>(aside).hp == TANK_HP);
}

TEST(the_laser_hits_an_enemy_only_once)
{
    Engine::Registry reg;
    const Engine::Entity player = laserShip(reg);
    const Engine::Entity monster = tankAt(reg, 500.f, 410.f);

    holdX(reg, player, config::LASER_CHARGE_TIME);
    releaseX(reg, player);
    collisionSystem(reg);
    collisionSystem(reg);
    CHECK(reg.get<Health>(monster).hp == TANK_HP - BIG_HIT);
}

TEST(the_laser_never_hurts_a_teammate)
{
    Engine::Registry reg;
    const Engine::Entity player = laserShip(reg);
    const Engine::Entity mate = createPlayer(reg, 1);

    reg.get<gameLogic::Vec2Pos>(mate) = gameLogic::Vec2Pos{{600.f, 410.f}};
    holdX(reg, player, config::LASER_CHARGE_TIME);
    releaseX(reg, player);
    CHECK(reg.get<Health>(mate).hp == config::PLAYER_HP);
}

TEST(the_laser_stops_at_the_first_wall_on_its_line)
{
    Engine::Registry reg;
    const Engine::Entity player = laserShip(reg);
    const Engine::Entity behind = tankAt(reg, 1200.f, 410.f);
    ObstacleStep step{};

    step.kind = ObstacleKind::Rock;
    step.width = 64.f;
    step.height = 64.f;
    const Engine::Entity wall = createObstacle(reg, step);

    reg.get<gameLogic::Vec2Pos>(wall) = gameLogic::Vec2Pos{{800.f, 380.f}};
    holdX(reg, player, config::LASER_CHARGE_TIME);
    releaseX(reg, player);
    CHECK(reg.get<Health>(behind).hp == TANK_HP);
    CHECK(reg.get<gameLogic::Vec2Hitbox>(beams(reg).front()).x < 800.f);
}

TEST(a_wall_on_another_line_does_not_stop_the_laser)
{
    Engine::Registry reg;
    const Engine::Entity player = laserShip(reg);
    const Engine::Entity far = tankAt(reg, 1500.f, 410.f);
    ObstacleStep step{};

    step.kind = ObstacleKind::Rock;
    step.width = 64.f;
    step.height = 64.f;
    const Engine::Entity wall = createObstacle(reg, step);

    reg.get<gameLogic::Vec2Pos>(wall) = gameLogic::Vec2Pos{{800.f, 100.f}};
    holdX(reg, player, config::LASER_CHARGE_TIME);
    releaseX(reg, player);
    CHECK(reg.get<Health>(far).hp == TANK_HP - BIG_HIT);
}

TEST(the_laser_does_not_charge_during_its_cooldown)
{
    Engine::Registry reg;
    const Engine::Entity player = laserShip(reg);

    holdX(reg, player, config::LASER_CHARGE_TIME);
    releaseX(reg, player);
    holdX(reg, player, 1.f);
    CHECK(reg.get<SecondaryWeapon>(player).charge == 0.f);
    releaseX(reg, player);
    CHECK(beams(reg).size() == 1);
}

TEST(the_laser_can_fire_again_after_its_cooldown)
{
    Engine::Registry reg;
    const Engine::Entity player = laserShip(reg);

    holdX(reg, player, config::LASER_CHARGE_TIME);
    releaseX(reg, player);
    secondarySystem(reg, config::LASER_COOLDOWN);
    holdX(reg, player, config::LASER_CHARGE_TIME);
    releaseX(reg, player);
    CHECK(beams(reg).size() == 2);
}

TEST(the_beam_lasts_a_short_time_and_then_disappears)
{
    Engine::Registry reg;
    const Engine::Entity player = laserShip(reg);

    holdX(reg, player, config::LASER_CHARGE_TIME);
    releaseX(reg, player);
    lifetimeSystem(reg, config::LASER_DURATION / 2.f);
    CHECK(beams(reg).size() == 1);
    lifetimeSystem(reg, config::LASER_DURATION);
    CHECK(beams(reg).empty());
}

TEST(the_laser_hurts_a_boss)
{
    Engine::Registry reg;
    const Engine::Entity player = laserShip(reg);
    const Engine::Entity boss = spawnBoss(reg);

    reg.get<gameLogic::Vec2Pos>(boss) = gameLogic::Vec2Pos{{900.f, 250.f}};
    const int before = reg.get<Health>(boss).hp;

    holdX(reg, player, config::LASER_CHARGE_TIME);
    releaseX(reg, player);
    CHECK(reg.get<Health>(boss).hp == before - BIG_HIT);
}

TEST(pausing_the_game_loses_the_laser_charge)
{
    Game game(1);

    game.addPlayer();
    game.addAugment(0, AugmentId::Laser);
    const Engine::Entity player = *findPlayerBySlot(game.registry(), 0);

    game.registry().get<SecondaryWeapon>(player).charge = 1.f;
    game.setPaused(true);
    CHECK(game.registry().get<SecondaryWeapon>(player).charge == 0.f);
}

TEST(the_equipped_weapon_is_the_last_one_bought)
{
    CHECK(!equippedWeapon({AugmentId::DamageUp}).has_value());
    CHECK(equippedWeapon({AugmentId::Rocket, AugmentId::DamageUp,
        AugmentId::Laser}) == WeaponKind::Laser);
    CHECK(equippedWeapon({AugmentId::Laser, AugmentId::Rocket})
        == WeaponKind::Rocket);
}
