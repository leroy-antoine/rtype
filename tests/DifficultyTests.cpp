#include <vector>
#include "logic/Collectibles.hpp"
#include "logic/Game.hpp"
#include "TestHarness.hpp"

using namespace rtype;

namespace {
    Settings with(const Difficulty difficulty, const bool friendlyFire = false)
    {
        return Settings{difficulty, friendlyFire};
    }

    void run(Game &game, const float seconds)
    {
        const int ticks = static_cast<int>(seconds * config::TICK_RATE);

        for (int i = 0; i < ticks; ++i) {
            game.update(config::FIXED_DT);
        }
    }

    void kill(Game &game, const int slot)
    {
        const auto player = findPlayerBySlot(game.registry(), slot);

        game.registry().get<Health>(*player).hp = 0;
    }

    int hpOf(Game &game, const int slot)
    {
        const auto player = findPlayerBySlot(game.registry(), slot);

        return game.registry().get<Health>(*player).hp;
    }

    void destroyAll(Engine::Registry &reg,
        const std::vector<Engine::Entity> &entities)
    {
        for (const Engine::Entity e : entities) {
            reg.destroy(e);
        }
    }

    Engine::Registry registryWith(const Settings &settings)
    {
        Engine::Registry reg;

        createSettingsHolder(reg);
        storeSettings(reg, settings);
        return reg;
    }
}

TEST(a_monster_gets_the_hit_points_of_the_difficulty_once)
{
    Game game(1);

    game.setSettings(with(Difficulty::Brutal));
    game.addPlayer();
    const Engine::Entity e = spawnMonster(game.registry(), 400.f);

    run(game, 0.1f);
    CHECK(game.registry().get<Health>(e).hp == 2 * config::MONSTER_HP);
    run(game, 0.1f);
    CHECK(game.registry().get<Health>(e).hp == 2 * config::MONSTER_HP);
}

TEST(a_monster_shoots_faster_when_the_difficulty_rises)
{
    Game game(1);

    game.setSettings(with(Difficulty::Brutal));
    game.addPlayer();
    const Engine::Entity e = spawnMonster(game.registry(), 400.f);

    run(game, 0.1f);
    CHECK(game.registry().get<Weapon>(e).cooldown
        < config::MONSTER_FIRE_COOLDOWN);
}

TEST(the_boss_life_follows_the_difficulty)
{
    Game game(1);

    game.setSettings(with(Difficulty::Hard));
    game.addPlayer();
    const Engine::Entity boss = spawnBoss(game.registry());

    run(game, 0.1f);
    CHECK(game.status().bossMaxHp == config::BOSS_HP * 3 / 2);
    CHECK(game.registry().get<Health>(boss).hp <= game.status().bossMaxHp);
}

TEST(the_players_hit_the_enemies_less_hard_in_brutal)
{
    Engine::Registry reg = registryWith(with(Difficulty::Brutal));
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity monster = spawnMonster(reg, 400.f);
    const int before = reg.get<Health>(monster).hp;

    applyDamages(monster, player, reg);
    CHECK(before - reg.get<Health>(monster).hp == 8);
}

TEST(the_enemies_hit_the_players_harder_in_brutal)
{
    Engine::Registry reg = registryWith(with(Difficulty::Brutal));
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity monster = spawnMonster(reg, 400.f);

    applyDamages(player, monster, reg);
    CHECK(config::PLAYER_HP - reg.get<Health>(player).hp == 15);
}

TEST(the_normal_difficulty_keeps_the_base_damage)
{
    Engine::Registry reg = registryWith(with(Difficulty::Normal));
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity monster = spawnMonster(reg, 400.f);

    applyDamages(player, monster, reg);
    CHECK(config::PLAYER_HP - reg.get<Health>(player).hp
        == config::MONSTER_DAMAGE);
}

TEST(the_coins_follow_the_economy_of_the_difficulty)
{
    Engine::Registry reg = registryWith(with(Difficulty::Brutal));

    CHECK(scaledCoin(reg, 12) == 9);
    storeSettings(reg, with(Difficulty::Easy));
    CHECK(scaledCoin(reg, 12) == 15);
    CHECK(scaledCoin(reg, 1) == 1);
}

TEST(the_waves_are_denser_in_brutal_but_the_mini_bosses_stay_alone)
{
    const WaveStep line{0.f, 4, -1.f, 0.f, 0.5f};
    WaveStep mini = line;

    mini.count = 1;
    mini.kind = MonsterKind::MiniTank;
    CHECK(densified(line, 1.5f).count == 6);
    CHECK(densified(line, 0.75f).count == 3);
    CHECK(densified(mini, 1.5f).count == 1);
}

TEST(a_dead_player_comes_back_alone_after_the_delay_and_loses_a_life)
{
    Game game(1);

    game.addPlayer();
    run(game, 0.1f);
    kill(game, 0);
    run(game, 0.1f);
    CHECK(!game.isPlayerAlive(0));
    CHECK(game.livesOf(0) == 2);
    run(game, config::RESPAWN_DELAY + 0.2f);
    CHECK(game.isPlayerAlive(0));
    CHECK(game.livesOf(0) == 2);
}

TEST(a_player_without_lives_is_eliminated_and_the_game_is_lost)
{
    Game game(1);

    game.setSettings(with(Difficulty::Hard));
    game.addPlayer();
    game.addPlayer();
    run(game, 0.1f);
    kill(game, 0);
    run(game, config::RESPAWN_DELAY + 0.2f);
    kill(game, 0);
    run(game, config::RESPAWN_DELAY + 0.2f);
    CHECK(game.livesOf(0) == 0);
    CHECK(!game.isPlayerAlive(0));
    CHECK(game.isPlayerAlive(1));
    CHECK(game.phase() == Phase::Playing);
}

TEST(the_game_is_lost_when_every_player_has_used_all_their_lives)
{
    Game game(1);

    game.setSettings(with(Difficulty::Hard));
    game.addPlayer();
    run(game, 0.1f);
    kill(game, 0);
    run(game, config::RESPAWN_DELAY + 0.2f);
    kill(game, 0);
    run(game, 0.1f);
    CHECK(game.phase() == Phase::Defeat);
}

TEST(a_dead_player_with_lives_left_does_not_lose_the_game)
{
    Game game(1);

    game.addPlayer();
    run(game, 0.1f);
    kill(game, 0);
    run(game, 0.1f);
    CHECK(game.phase() == Phase::Playing);
}

TEST(a_death_run_costs_no_life)
{
    Game game(1);
    Segments segments = firstLevel();

    segments.deathRuns = {1};
    game.openLobby();
    game.addPlayer();
    game.addPlayer();
    game.startMap(std::move(segments), 0);
    run(game, 2.f);
    kill(game, 0);
    run(game, 0.1f);
    CHECK(game.livesOf(0) == 3);
    CHECK(game.livesOf(1) == 3);
    CHECK(game.isPlayerAlive(0));
}

TEST(the_status_shows_the_lives_left_of_each_player)
{
    Game game(1);

    game.setSettings(with(Difficulty::Easy));
    game.addPlayer();
    run(game, 0.1f);
    CHECK(game.status().players.front().lives == 5);
    kill(game, 0);
    run(game, 0.1f);
    CHECK(game.status().players.front().lives == 4);
}

TEST(the_lives_come_back_with_a_new_match)
{
    Game game(1);

    game.setSettings(with(Difficulty::Hard));
    game.addPlayer();
    run(game, 0.1f);
    kill(game, 0);
    run(game, 0.1f);
    CHECK(game.livesOf(0) == 1);
    game.openLobby();
    CHECK(game.livesOf(0) == 2);
}

TEST(a_shot_goes_through_a_teammate_without_friendly_fire)
{
    Engine::Registry reg = registryWith(with(Difficulty::Normal, false));
    const Engine::Entity shooter = createPlayer(reg, 0);
    const Engine::Entity friend_ = createPlayer(reg, 1);
    const Engine::Entity bullet = createBullet(reg, shooter, {900.f, 0.f});

    reg.get<gameLogic::Vec2Pos>(bullet) =
        reg.get<gameLogic::Vec2Pos>(friend_);
    collisionSystem(reg);
    CHECK(reg.get<Health>(friend_).hp == config::PLAYER_HP);
    CHECK(reg.alive(bullet));
}

TEST(a_shot_hurts_a_teammate_with_friendly_fire)
{
    Engine::Registry reg = registryWith(with(Difficulty::Brutal, true));
    const Engine::Entity shooter = createPlayer(reg, 0);
    const Engine::Entity friend_ = createPlayer(reg, 1);
    const Engine::Entity bullet = createBullet(reg, shooter, {900.f, 0.f});

    reg.get<gameLogic::Vec2Pos>(bullet) =
        reg.get<gameLogic::Vec2Pos>(friend_);
    collisionSystem(reg);
    CHECK(reg.get<Health>(friend_).hp
        == config::PLAYER_HP - config::PLAYER_DAMAGE);
    CHECK(!reg.alive(bullet));
}

TEST(a_shot_never_hurts_its_own_shooter)
{
    Engine::Registry reg = registryWith(with(Difficulty::Normal, true));
    const Engine::Entity shooter = createPlayer(reg, 0);
    const Engine::Entity bullet = createBullet(reg, shooter, {900.f, 0.f});

    reg.get<gameLogic::Vec2Pos>(bullet) =
        reg.get<gameLogic::Vec2Pos>(shooter);
    collisionSystem(reg);
    CHECK(reg.get<Health>(shooter).hp == config::PLAYER_HP);
}

TEST(the_escort_grows_with_each_boss_in_brutal_only)
{
    CHECK(escortSize(Difficulty::Brutal, 0) == 1);
    CHECK(escortSize(Difficulty::Brutal, 1) == 2);
    CHECK(escortSize(Difficulty::Brutal, 2) == 3);
    CHECK(escortSize(Difficulty::Brutal, 3) == 3);
    CHECK(escortSize(Difficulty::Hard, 2) == 0);
    CHECK(escortSize(Difficulty::Normal, 0) == 0);
}

TEST(the_escort_alternates_the_mini_bosses_one_second_apart)
{
    const std::vector<EscortSpawn> escort = escortFor(Difficulty::Brutal, 2);

    CHECK(escort.size() == 3);
    CHECK(escort[0].kind == MonsterKind::MiniTank);
    CHECK(escort[1].kind == MonsterKind::MiniElite);
    CHECK(escort[2].kind == MonsterKind::MiniBomber);
    CHECK(escort[1].delay - escort[0].delay == config::ESCORT_DELAY);
    CHECK(escort[0].y < escort[1].y && escort[1].y < escort[2].y);
}

TEST(a_brutal_boss_comes_with_its_escort)
{
    Game game(1);
    Segments segments = firstLevel();

    game.setSettings(with(Difficulty::Brutal));
    game.openLobby();
    game.addPlayer();
    game.startMap(std::move(segments), 0);
    while (game.scroll() < config::BOSS_SPAWN_SCROLL) {
        destroyAll(game.registry(), collectMonsters(game.registry()));
        healPlayers(game.registry());
        game.update(config::FIXED_DT);
    }
    game.update(config::FIXED_DT);
    CHECK(!collectWith<BossTag>(game.registry()).empty());
    run(game, 4.f);
    bool escorted = false;

    for (const Engine::Entity e : collectMonsters(game.registry())) {
        const MonsterKind kind = game.registry().get<MonsterType>(e).kind;

        escorted = escorted || kind == MonsterKind::MiniTank;
    }
    CHECK(escorted);
}

TEST(a_manual_respawn_only_works_once_the_life_is_lost)
{
    Game game(1);

    game.addPlayer();
    run(game, 0.1f);
    kill(game, 0);
    game.registry().destroy(*findPlayerBySlot(game.registry(), 0));
    CHECK(!game.respawnPlayer(0));
    run(game, 0.1f);
    CHECK(game.livesOf(0) == 2);
    CHECK(game.respawnPlayer(0));
    CHECK(game.livesOf(0) == 2);
}

TEST(leaving_and_coming_back_does_not_give_the_lives_back)
{
    Game game(1);

    game.setSettings(with(Difficulty::Hard));
    game.addPlayer();
    run(game, 0.1f);
    kill(game, 0);
    run(game, 0.1f);
    CHECK(game.livesOf(0) == 1);
    game.removePlayer(0);
    game.addPlayer();
    CHECK(game.livesOf(0) == 1);
}
