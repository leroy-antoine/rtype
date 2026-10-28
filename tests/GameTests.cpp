#include <variant>
#include <vector>
#include "logic/Game.hpp"
#include "TestHarness.hpp"

using namespace rtype;

namespace {
    void run(Game &game, const float seconds)
    {
        const int ticks = static_cast<int>(seconds * config::TICK_RATE);

        for (int i = 0; i < ticks; ++i) {
            game.update(config::FIXED_DT);
        }
    }

    Engine::Entity playerOf(Game &game, const int slot)
    {
        return *findPlayerBySlot(game.registry(), slot);
    }

    void kill(Game &game, const int slot)
    {
        game.registry().get<Health>(playerOf(game, slot)).hp = 0;
    }

    void killBoss(Game &game)
    {
        for (const Engine::Entity boss : collectWith<BossTag>(game.registry())) {
            game.registry().get<Health>(boss).hp = 0;
        }
    }

    // Fait defiler le jeu jusqu'au boss, sans que le joueur ne meure.
    void skipToBoss(Game &game)
    {
        while (game.scroll() < config::BOSS_SPAWN_SCROLL) {
            for (const Engine::Entity m : collectMonsters(game.registry())) {
                game.registry().destroy(m);
            }
            game.update(config::FIXED_DT);
        }
        game.update(config::FIXED_DT);
    }
}

TEST(waves_spawn_monsters_at_their_scroll_distance)
{
    Game game(1);

    game.addPlayer();
    run(game, 1.f);
    CHECK(collectMonsters(game.registry()).empty());
    run(game, 3.f);
    CHECK(!collectMonsters(game.registry()).empty());
}

TEST(the_level_does_not_advance_without_players)
{
    Game game(1);

    run(game, 5.f);
    CHECK(game.scroll() == 0.f);
    game.addPlayer();
    run(game, 1.f);
    CHECK(game.scroll() > 0.f);
}

TEST(the_world_resets_when_the_last_player_leaves)
{
    Game game(1);

    game.addPlayer();
    run(game, 5.f);
    game.removePlayer(0);
    run(game, 0.1f);
    CHECK(game.scroll() == 0.f);
    CHECK(collectMonsters(game.registry()).empty());
}

TEST(a_player_bullet_hurts_a_monster)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity monster = spawnMonster(reg, 300.f);
    const Engine::Entity bullet = createBullet(reg, player,
        {config::BULLET_SPEED, 0.f});

    reg.get<gameLogic::Vec2Pos>(monster) = reg.get<gameLogic::Vec2Pos>(bullet);
    collisionSystem(reg);
    CHECK(reg.get<Health>(monster).hp
        == config::MONSTER_HP - config::PLAYER_DAMAGE);
    CHECK(!reg.alive(bullet));
}

TEST(monsters_do_not_shoot_before_entering_the_map)
{
    Engine::Registry reg;
    const Engine::Entity monster = spawnMonster(reg, 300.f);

    CHECK(!wantsToShoot(reg, monster));
    reg.get<gameLogic::Vec2Pos>(monster).x = 1500.f;
    CHECK(wantsToShoot(reg, monster));
}

TEST(a_monster_that_touches_a_player_kills_him)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity monster = spawnMonster(reg, 300.f);

    reg.get<gameLogic::Vec2Pos>(monster) = reg.get<gameLogic::Vec2Pos>(player);
    contactSystem(reg);
    CHECK(reg.get<Health>(player).hp <= 0);
}

TEST(the_boss_arrives_when_the_level_has_scrolled_enough)
{
    Game game(1);

    game.addPlayer();
    CHECK(collectWith<BossTag>(game.registry()).empty());
    skipToBoss(game);
    CHECK(collectWith<BossTag>(game.registry()).size() == 1);
}

TEST(the_boss_is_bigger_and_tougher_than_a_monster)
{
    Engine::Registry reg;
    const Engine::Entity boss = spawnBoss(reg);

    CHECK(reg.get<Health>(boss).hp == config::BOSS_HP);
    CHECK(reg.get<gameLogic::Vec2Hitbox>(boss).x == config::BOSS_WIDTH);
    CHECK(reg.get<Renderable>(boss).id == SpriteId::Boss);
    CHECK(reg.has<MonsterTag>(boss));
}

TEST(the_boss_stops_on_the_right_and_sways)
{
    Engine::Registry reg;
    const Engine::Entity boss = spawnBoss(reg);
    const float startY = reg.get<gameLogic::Vec2Pos>(boss).y;

    for (int i = 0; i < 60 * 12; ++i) {
        bossSystem(reg, config::FIXED_DT);
        movementSystem(reg, config::FIXED_DT);
    }
    CHECK(reg.get<gameLogic::Vec2Pos>(boss).x <= config::BOSS_STOP_X + 1.f);
    CHECK(reg.get<gameLogic::Vec2Pos>(boss).x >= config::BOSS_STOP_X - 5.f);
    CHECK(reg.get<gameLogic::Vec2Pos>(boss).y != startY);
}

TEST(the_boss_fires_a_fan_of_bullets_to_the_left)
{
    Engine::Registry reg;
    const Engine::Entity boss = spawnBoss(reg);
    const std::vector<gameLogic::Vec2> shots = shotVelocities(reg, boss);

    CHECK(static_cast<int>(shots.size()) == config::BOSS_FAN_BULLETS);
    for (const gameLogic::Vec2 &shot : shots) {
        CHECK(shot.x < 0.f);
    }
    CHECK(shots.front().y * shots.back().y < 0.f);
}

TEST(the_boss_bullets_are_bigger_than_the_others)
{
    Engine::Registry reg;
    const Engine::Entity boss = spawnBoss(reg);
    const Engine::Entity bullet = createBullet(reg, boss, {-100.f, 0.f});

    CHECK(reg.get<gameLogic::Vec2Hitbox>(bullet).x == config::BOSS_BULLET_SIZE);
}

TEST(a_player_shoots_one_bullet_to_the_right)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const std::vector<gameLogic::Vec2> shots = shotVelocities(reg, player);

    CHECK(shots.size() == 1);
    CHECK(shots[0].x > 0.f);
}

TEST(a_dead_player_can_respawn_while_a_teammate_keeps_the_game_on)
{
    Game game(1);

    game.addPlayer();
    game.addPlayer();
    kill(game, 0);
    game.update(config::FIXED_DT);
    CHECK(!game.isPlayerAlive(0));
    CHECK(game.respawnPlayer(0));
    CHECK(game.isPlayerAlive(0));
}

TEST(a_living_player_cannot_respawn)
{
    Game game(1);

    game.addPlayer();
    CHECK(!game.respawnPlayer(0));
}

TEST(killing_the_boss_wins_the_game)
{
    Game game(1);

    game.addPlayer();
    skipToBoss(game);
    killBoss(game);
    game.update(config::FIXED_DT);
    CHECK(game.phase() == Phase::Victory);
}

TEST(the_game_is_lost_when_every_player_is_dead)
{
    Game game(1);

    game.addPlayer();
    kill(game, 0);
    game.update(config::FIXED_DT);
    CHECK(game.phase() == Phase::Defeat);
    CHECK(!game.respawnPlayer(0));
}

TEST(the_game_is_not_lost_while_a_teammate_is_alive)
{
    Game game(1);

    game.addPlayer();
    game.addPlayer();
    kill(game, 0);
    run(game, 0.1f);
    CHECK(game.phase() == Phase::Playing);
}

TEST(a_finished_game_restarts_after_the_delay)
{
    Game game(1);

    game.addPlayer();
    skipToBoss(game);
    killBoss(game);
    game.update(config::FIXED_DT);
    CHECK(game.phase() == Phase::Victory);
    run(game, config::RESTART_DELAY + 1.f);
    CHECK(game.phase() == Phase::Playing);
    CHECK(game.scroll() < config::BOSS_SPAWN_SCROLL);
    CHECK(game.isPlayerAlive(0));
    CHECK(collectWith<BossTag>(game.registry()).empty());
}

TEST(the_status_describes_players_and_the_boss)
{
    Game game(1);

    game.addPlayer();
    game.addPlayer();
    GameStatus status = game.status();

    CHECK(status.phase == Phase::Playing);
    CHECK(status.bossMaxHp == 0);
    CHECK(status.players.size() == 2);
    CHECK(status.players[1].slot == 1);
    CHECK(status.players[1].hp == config::PLAYER_HP);
    CHECK(status.players[1].alive);
    skipToBoss(game);
    status = game.status();
    CHECK(status.bossHp == config::BOSS_HP);
    CHECK(status.bossMaxHp == config::BOSS_HP);
}

TEST(every_spawned_entity_is_announced_to_the_clients)
{
    Game game(1);

    game.addPlayer();
    run(game, 5.f);
    std::size_t spawned = 0;

    for (const GameEvent &event : game.takeEvents()) {
        if (std::holds_alternative<EntitySpawned>(event)) {
            ++spawned;
        }
    }
    CHECK(spawned >= 5);
    CHECK(game.spawns().size() >= 5);
}
