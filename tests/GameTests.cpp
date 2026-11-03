#include <filesystem>
#include <variant>
#include <vector>
#include "logic/Builder.hpp"
#include "logic/Game.hpp"
#include "logic/Leaderboard.hpp"
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

TEST(a_basic_monster_follows_the_basic_definition)
{
    Engine::Registry reg;
    const Engine::Entity monster = spawnMonster(reg, 300.f);

    CHECK(reg.get<MonsterType>(monster).kind == MonsterKind::Basic);
    CHECK(reg.get<Health>(monster).hp == config::MONSTER_HP);
    CHECK(reg.get<Speed>(monster).value == config::MONSTER_SPEED);
    CHECK(reg.get<Renderable>(monster).id == SpriteId::Monster);
}

TEST(a_monster_remembers_the_kind_it_was_created_with)
{
    Engine::Registry reg;
    const Engine::Entity tank = createMonster(reg, MonsterKind::Tank);

    CHECK(reg.get<MonsterType>(tank).kind == MonsterKind::Tank);
    CHECK(reg.has<MonsterTag>(tank));
}

TEST(every_boss_kind_follows_its_definition)
{
    for (std::size_t i = 0; i < static_cast<std::size_t>(BossKind::Count); ++i) {
        const auto kind = static_cast<BossKind>(i);
        Engine::Registry reg;
        const Engine::Entity boss = createBoss(reg, kind);

        CHECK(reg.get<BossType>(boss).kind == kind);
        CHECK(reg.get<Health>(boss).hp == bossDef(kind).hp);
        CHECK(reg.get<gameLogic::Vec2Hitbox>(boss).x == bossDef(kind).width);
    }
}

TEST(the_status_uses_the_health_of_the_kind_of_boss)
{
    Game game(1);

    game.addPlayer();
    spawnBoss(game.registry(), BossKind::Fortress);
    CHECK(game.status().bossHp == bossDef(BossKind::Fortress).hp);
    CHECK(game.status().bossMaxHp == bossDef(BossKind::Fortress).hp);
}

TEST(the_monsters_of_a_random_wave_do_not_share_the_same_height)
{
    Game game(1);

    game.addPlayer();
    run(game, 4.5f);
    std::vector<float> heights;
    for (const Engine::Entity m : collectMonsters(game.registry())) {
        heights.push_back(game.registry().get<gameLogic::Vec2Pos>(m).y);
    }
    CHECK(heights.size() >= 3);
    CHECK(std::ranges::any_of(heights, [&heights](const float y) {
        return y != heights.front();
    }));
}

TEST(healing_the_players_restores_their_health)
{
    Game game(1);

    game.addPlayer();
    kill(game, 0);
    healPlayers(game.registry());
    CHECK(game.registry().get<Health>(playerOf(game, 0)).hp
        == config::PLAYER_HP);
}

TEST(a_game_built_on_a_map_follows_its_levels)
{
    Game game(1, buildMap(0).value());

    game.addPlayer();
    CHECK(game.level() == 1);
    CHECK(game.status().level == 1);
    skipToBoss(game);
    CHECK(game.level() == 1);
    killBoss(game);
    game.update(config::FIXED_DT);
    CHECK(game.phase() == Phase::Playing);
    for (const Engine::Entity p : collectProjectileEntities(game.registry())) {
        game.registry().destroy(p);
    }
    run(game, 1.5f);
    CHECK(game.level() == 2);
    CHECK(game.status().level == 2);
}

TEST(every_monster_kind_has_its_own_sprite_and_valid_stats)
{
    for (std::size_t i = 0; i < static_cast<std::size_t>(MonsterKind::Count);
        ++i) {
        const MonsterDef &def = monsterDef(static_cast<MonsterKind>(i));

        CHECK(def.hp > 0);
        CHECK(def.speed > 0.f);
        CHECK(def.width > 0.f);
        CHECK(def.height > 0.f);
        for (std::size_t j = 0; j < i; ++j) {
            CHECK(def.sprite != monsterDef(static_cast<MonsterKind>(j)).sprite);
        }
    }
}

TEST(the_kinds_of_monsters_are_coherent_with_their_names)
{
    const MonsterDef &basic = monsterDef(MonsterKind::Basic);
    const MonsterDef &tank = monsterDef(MonsterKind::Tank);
    const MonsterDef &kamikaze = monsterDef(MonsterKind::Kamikaze);
    const MonsterDef &bomber = monsterDef(MonsterKind::Bomber);

    CHECK(tank.width * tank.height > basic.width * basic.height);
    CHECK(tank.hp > basic.hp);
    CHECK(tank.speed < basic.speed);
    CHECK(kamikaze.width * kamikaze.height < basic.width * basic.height);
    CHECK(kamikaze.speed > basic.speed);
    CHECK(kamikaze.hp < basic.hp);
    CHECK(bomber.hp > tank.hp);
    CHECK(bomber.speed < basic.speed);
}

TEST(every_boss_kind_has_its_own_sprite_and_is_tougher_than_any_monster)
{
    const MonsterDef &basic = monsterDef(MonsterKind::Basic);
    const MonsterDef &bomber = monsterDef(MonsterKind::Bomber);

    for (std::size_t i = 0; i < static_cast<std::size_t>(BossKind::Count);
        ++i) {
        const MonsterDef &def = bossDef(static_cast<BossKind>(i));

        CHECK(def.hp > bomber.hp);
        CHECK(def.width * def.height > basic.width * basic.height);
        for (std::size_t j = 0; j < i; ++j) {
            CHECK(def.sprite != bossDef(static_cast<BossKind>(j)).sprite);
        }
    }
}

TEST(every_kind_of_shooter_fires_its_own_projectile)
{
    Engine::Registry reg;
    const Engine::Entity rocket = createMonster(reg, MonsterKind::Rocket);
    const Engine::Entity bomber = createMonster(reg, MonsterKind::Bomber);
    const Engine::Entity dasher = createBoss(reg, BossKind::Dasher);

    for (const Engine::Entity shooter : {rocket, bomber, dasher}) {
        reg.get<gameLogic::Vec2Pos>(shooter) =
            gameLogic::Vec2Pos{{900.f, 400.f}};
    }
    const auto sprite = [&reg](const Engine::Entity shooter) {
        const Engine::Entity bullet =
            createBullet(reg, shooter, {-100.f, 0.f});

        return reg.get<Renderable>(bullet).id;
    };

    CHECK(sprite(rocket) == SpriteId::RocketMissile);
    CHECK(sprite(bomber) == SpriteId::Bomb);
    CHECK(sprite(dasher) == SpriteId::AlienShard);
}

TEST(a_volley_has_as_many_bullets_as_the_projectile_says)
{
    Engine::Registry reg;
    const Engine::Entity dasher = createBoss(reg, BossKind::Dasher);
    const Engine::Entity basic = createMonster(reg);
    const Engine::Entity player = createPlayer(reg, 0);

    CHECK(static_cast<int>(shotVelocities(reg, dasher).size())
        == bossDef(BossKind::Dasher).projectile.shots);
    CHECK(shotVelocities(reg, basic).size() == 1);
    CHECK(shotVelocities(reg, player).size() == 1);
    CHECK(shotVelocities(reg, player)[0].x > 0.f);
}

TEST(the_dasher_roams_the_whole_right_side_of_the_map_and_stays_on_it)
{
    Game game(1);
    float minY = config::MAP_HEIGHT;
    float maxY = 0.f;

    game.addPlayer();
    const Engine::Entity boss = spawnBoss(game.registry(), BossKind::Dasher);
    for (int i = 0; i < 25 * 60; ++i) {
        game.update(config::FIXED_DT);
        if (i < 120) {
            continue;
        }
        const gameLogic::Vec2Pos pos =
            game.registry().get<gameLogic::Vec2Pos>(boss);
        const gameLogic::Vec2Hitbox box =
            game.registry().get<gameLogic::Vec2Hitbox>(boss);

        CHECK(pos.x >= 0.f && pos.x + box.x <= config::MAP_WIDTH + 10.f);
        minY = std::min(minY, pos.y);
        maxY = std::max(maxY, pos.y + box.y);
    }
    CHECK(game.registry().alive(boss));
    CHECK(maxY - minY > 600.f);
}

namespace {
    Engine::Entity blockOn(Engine::Registry &reg, const Engine::Entity target,
        const ObstacleKind kind)
    {
        const Engine::Entity block =
            createObstacle(reg, wall(0.f, 0.f, 200.f, 200.f, kind));

        reg.get<gameLogic::Vec2Pos>(block) =
            reg.get<gameLogic::Vec2Pos>(target);
        return block;
    }
}

namespace {
    Collider colliderOf(Engine::Registry &reg, const Engine::Entity e)
    {
        return {e, reg.get<gameLogic::Vec2Pos>(e),
            reg.get<gameLogic::Vec2Hitbox>(e), Team{}};
    }
}

TEST(a_player_who_touches_a_spike_dies)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    blockOn(reg, player, ObstacleKind::Spike);
    obstacleSystem(reg);
    CHECK(reg.get<Health>(player).hp <= 0);
}

TEST(a_player_who_touches_a_rock_is_pushed_out_and_lives)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity rock = blockOn(reg, player, ObstacleKind::Rock);

    obstacleSystem(reg);
    CHECK(reg.get<Health>(player).hp == config::PLAYER_HP);
    CHECK(!overlaps(colliderOf(reg, player), colliderOf(reg, rock)));
}

TEST(a_rock_cannot_be_crossed_by_a_moving_player)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity rock = createObstacle(reg, wall(0.f, 0.f, 100.f,
        config::MAP_HEIGHT));

    reg.get<gameLogic::Vec2Pos>(rock).x = 400.f;
    reg.get<gameLogic::Vec2Velocity>(rock) = gameLogic::Vec2Velocity{};
    reg.get<PlayerInput>(player).right = true;
    for (int i = 0; i < 120; ++i) {
        playerControlSystem(reg);
        movementSystem(reg, config::FIXED_DT);
        obstacleSystem(reg);
    }
    CHECK(reg.get<gameLogic::Vec2Pos>(player).x + config::PLAYER_WIDTH
        <= 400.f + 1.f);
}

TEST(a_rock_that_pushes_a_player_off_the_map_crushes_him)
{
    Game game(1);

    game.addPlayer();
    const Engine::Entity player = *findPlayerBySlot(game.registry(), 0);
    const Engine::Entity rock = createObstacle(game.registry(),
        wall(0.f, 0.f, 400.f, config::MAP_HEIGHT));

    game.registry().get<gameLogic::Vec2Pos>(rock).x = 300.f;
    run(game, 6.f);
    CHECK(!game.isPlayerAlive(0));
    CHECK(!game.registry().alive(player));
}

TEST(a_player_away_from_the_walls_is_safe)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    createObstacle(reg, wall(0.f, 0.f, 200.f, 200.f, ObstacleKind::Spike));
    obstacleSystem(reg);
    CHECK(reg.get<Health>(player).hp == config::PLAYER_HP);
}

TEST(a_reverse_portal_inverts_the_keys_once)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity gate = blockOn(reg, player,
        ObstacleKind::ReversePortal);

    obstacleSystem(reg);
    obstacleSystem(reg);
    CHECK(reg.get<PlayerEffects>(player).inverted);
    reg.get<PlayerInput>(player).right = true;
    playerControlSystem(reg);
    CHECK(reg.get<gameLogic::Vec2Velocity>(player).x < 0.f);
    reg.get<gameLogic::Vec2Pos>(gate).x = 1500.f;
    obstacleSystem(reg);
    blockOn(reg, player, ObstacleKind::ReversePortal);
    obstacleSystem(reg);
    CHECK(!reg.get<PlayerEffects>(player).inverted);
}

TEST(speed_portals_change_the_speed_of_a_player)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    blockOn(reg, player, ObstacleKind::FastPortal);
    obstacleSystem(reg);
    reg.get<PlayerInput>(player).right = true;
    playerControlSystem(reg);
    CHECK(reg.get<gameLogic::Vec2Velocity>(player).x
        == config::PLAYER_SPEED * config::FAST_SPEED_FACTOR);
    blockOn(reg, player, ObstacleKind::NormalPortal);
    obstacleSystem(reg);
    playerControlSystem(reg);
    CHECK(reg.get<gameLogic::Vec2Velocity>(player).x
        == config::PLAYER_SPEED);
}

TEST(a_portal_stops_no_bullet_and_hurts_nobody)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity bullet = createBullet(reg, player, {900.f, 0.f});
    const Engine::Entity gate = blockOn(reg, bullet, ObstacleKind::FastPortal);

    obstacleSystem(reg);
    CHECK(reg.alive(bullet));
    CHECK(reg.alive(gate));
    CHECK(reg.get<Health>(player).hp == config::PLAYER_HP);
}

TEST(a_moving_obstacle_swings_around_its_height)
{
    Engine::Registry reg;
    ObstacleStep step = wall(0.f, 500.f, 100.f, 100.f, ObstacleKind::Spike);
    float lowest = 5000.f;
    float highest = -5000.f;

    step.amplitude = 100.f;
    step.period = 2.f;
    const Engine::Entity e = createObstacle(reg, step);
    for (int i = 0; i < 240; ++i) {
        obstacleMotionSystem(reg, config::FIXED_DT);
        lowest = std::min(lowest, reg.get<gameLogic::Vec2Pos>(e).y);
        highest = std::max(highest, reg.get<gameLogic::Vec2Pos>(e).y);
    }
    CHECK(lowest < 410.f);
    CHECK(highest > 590.f);
}

TEST(the_effects_of_the_portals_end_with_the_level)
{
    Segments segments;

    segments.levels = {{1, 0.f}, {2, 240.f}};
    segments.bossSpawn = {{BossKind::Standard, 1.0e9f}};
    Game game(1, segments);

    game.addPlayer();
    const Engine::Entity player = *findPlayerBySlot(game.registry(), 0);

    run(game, 0.1f);
    game.registry().get<PlayerEffects>(player).inverted = true;
    game.registry().get<PlayerEffects>(player).speedFactor = 1.5f;
    run(game, 3.f);
    CHECK(!game.registry().get<PlayerEffects>(player).inverted);
    CHECK(game.registry().get<PlayerEffects>(player).speedFactor == 1.f);
}

TEST(a_wall_absorbs_the_bullets_that_touch_it)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity bullet = createBullet(reg, player, {900.f, 0.f});
    const Engine::Entity block = createObstacle(reg, wall(0.f, 0.f, 200.f, 200.f));

    reg.get<gameLogic::Vec2Pos>(block) =
        reg.get<gameLogic::Vec2Pos>(bullet);
    obstacleSystem(reg);
    CHECK(!reg.alive(bullet));
}

TEST(walls_come_from_the_level_scroll_and_leave_the_map)
{
    Segments segments;

    segments.levels = {{1, 0.f}};
    segments.bossSpawn = {{BossKind::Standard, 1.0e9f}};
    segments.obstacles = {wall(120.f, 900.f, 100.f, 100.f)};
    Game game(1, segments);

    game.addPlayer();
    run(game, 0.5f);
    CHECK(collectObstacles(game.registry()).empty());
    run(game, 1.5f);
    CHECK(collectObstacles(game.registry()).size() == 1);
    run(game, 20.f);
    CHECK(collectObstacles(game.registry()).empty());
}

TEST(a_game_starts_at_the_first_level)
{
    Game game(1);

    CHECK(game.level() == 1);
}

TEST(the_level_number_follows_the_distance_scrolled)
{
    Segments segments;

    segments.levels = {{1, 0.f}, {2, 1000.f}, {3, 2500.f}};
    CHECK(levelAt(segments, 0.f) == 1);
    CHECK(levelAt(segments, 999.f) == 1);
    CHECK(levelAt(segments, 1000.f) == 2);
    CHECK(levelAt(segments, 3000.f) == 3);
}

TEST(the_distance_stops_while_a_boss_is_alive)
{
    Game game(1);

    game.addPlayer();
    skipToBoss(game);
    const float stopped = game.scroll();

    CHECK(!collectWith<BossTag>(game.registry()).empty());
    run(game, 0.5f);
    CHECK(game.phase() == Phase::Playing);
    CHECK(game.scroll() == stopped);
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

TEST(an_augment_is_applied_at_once_to_a_living_player)
{
    Game game(1);

    game.addPlayer();
    CHECK(game.addAugment(0, AugmentId::DamageUp));
    CHECK(game.registry().get<Damage>(playerOf(game, 0)).value
        == config::PLAYER_DAMAGE + config::AUGMENT_DAMAGE_BONUS);
}

TEST(an_augment_survives_a_respawn)
{
    Game game(1);

    game.addPlayer();
    game.addPlayer();
    game.addAugment(0, AugmentId::DamageUp);
    kill(game, 0);
    game.update(config::FIXED_DT);
    CHECK(game.respawnPlayer(0));
    CHECK(game.registry().get<Damage>(playerOf(game, 0)).value
        == config::PLAYER_DAMAGE + config::AUGMENT_DAMAGE_BONUS);
}

TEST(an_augment_bought_while_dead_is_applied_on_respawn)
{
    Game game(1);

    game.addPlayer();
    game.addPlayer();
    kill(game, 0);
    game.update(config::FIXED_DT);
    CHECK(game.addAugment(0, AugmentId::SpeedUp));
    game.respawnPlayer(0);
    CHECK(game.registry().get<Speed>(playerOf(game, 0)).value
        == config::PLAYER_SPEED + config::AUGMENT_SPEED_BONUS);
}

TEST(augments_are_reapplied_when_the_game_restarts)
{
    Game game(1);

    game.addPlayer();
    game.addAugment(0, AugmentId::SpeedUp);
    kill(game, 0);
    run(game, config::RESTART_DELAY + 1.f);
    CHECK(game.phase() == Phase::Playing);
    CHECK(game.registry().get<Speed>(playerOf(game, 0)).value
        == config::PLAYER_SPEED + config::AUGMENT_SPEED_BONUS);
}

TEST(augments_are_kept_per_player)
{
    Game game(1);

    game.addPlayer();
    game.addPlayer();
    game.addAugment(0, AugmentId::DamageUp);
    game.addAugment(0, AugmentId::DamageUp);
    game.addAugment(1, AugmentId::MaxHpUp);
    CHECK(game.ownedAugments(0).size() == 2);
    CHECK(game.ownedAugments(1).size() == 1);
    CHECK(game.registry().get<Damage>(playerOf(game, 1)).value
        == config::PLAYER_DAMAGE);
}

TEST(a_player_who_leaves_loses_the_augments)
{
    Game game(1);

    game.addPlayer();
    game.addAugment(0, AugmentId::DamageUp);
    game.removePlayer(0);
    CHECK(game.ownedAugments(0).empty());
    game.addPlayer();
    CHECK(game.registry().get<Damage>(playerOf(game, 0)).value
        == config::PLAYER_DAMAGE);
}

TEST(an_augment_needs_a_connected_player_and_a_valid_id)
{
    Game game(1);

    game.addPlayer();
    CHECK(!game.addAugment(1, AugmentId::DamageUp));
    CHECK(!game.addAugment(0, AugmentId::Count));
    CHECK(game.ownedAugments(0).empty());
}

TEST(the_timer_counts_the_time_of_the_match)
{
    Game game(1);

    game.addPlayer();
    CHECK(game.status().elapsedMs == 0);
    run(game, 2.f);
    CHECK(game.status().elapsedMs >= 1990);
    CHECK(game.status().elapsedMs <= 2010);
}

TEST(the_timer_restarts_with_a_new_game)
{
    Game game(1);

    game.addPlayer();
    run(game, 1.f);
    game.removePlayer(0);
    run(game, 0.1f);
    CHECK(game.status().elapsedMs == 0);
}

TEST(the_timer_stops_when_the_match_is_over)
{
    Game game(1);

    game.addPlayer();
    run(game, 1.f);
    kill(game, 0);
    run(game, 0.1f);
    const std::uint32_t atTheEnd = game.status().elapsedMs;

    run(game, 1.f);
    CHECK(game.phase() != Phase::Playing);
    CHECK(game.status().elapsedMs == atTheEnd);
}

TEST(a_dying_boss_makes_a_boss_sized_explosion)
{
    Engine::Registry reg;
    const Engine::Entity boss = spawnBoss(reg, BossKind::Dasher);

    CHECK(explosionSize(reg, boss) >= config::BOSS_EXPLOSION_MIN_SIZE);
}

namespace {
    Engine::Entity placed(Engine::Registry &reg, const MonsterKind kind,
        const float y)
    {
        const Engine::Entity e = spawnMonster(reg, y, kind);

        reg.get<gameLogic::Vec2Pos>(e).x = 1500.f;
        return e;
    }
}

TEST(a_kamikaze_dives_toward_the_player_once_close)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity kamikaze = placed(reg, MonsterKind::Kamikaze, 100.f);

    reg.get<gameLogic::Vec2Pos>(player) = gameLogic::Vec2Pos{{600.f, 800.f}};
    monsterSystem(reg, config::FIXED_DT);
    CHECK(reg.get<MonsterState>(kamikaze).locked);
    CHECK(reg.get<gameLogic::Vec2Velocity>(kamikaze).y > 0.f);
    CHECK(reg.get<gameLogic::Vec2Velocity>(kamikaze).x < 0.f);
}

TEST(a_kamikaze_far_from_the_player_keeps_flying_straight)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity kamikaze = placed(reg, MonsterKind::Kamikaze, 100.f);

    reg.get<gameLogic::Vec2Pos>(player) = gameLogic::Vec2Pos{{100.f, 800.f}};
    monsterSystem(reg, config::FIXED_DT);
    CHECK(!reg.get<MonsterState>(kamikaze).locked);
    CHECK(reg.get<gameLogic::Vec2Velocity>(kamikaze).y == 0.f);
}

TEST(an_elite_zigzags_and_stays_on_the_map)
{
    Engine::Registry reg;
    const Engine::Entity elite = placed(reg, MonsterKind::Elite, 20.f);
    float minY = 5000.f;
    float maxY = -5000.f;

    for (int i = 0; i < 60 * 6; ++i) {
        monsterSystem(reg, config::FIXED_DT);
        movementSystem(reg, config::FIXED_DT);
        reg.get<gameLogic::Vec2Pos>(elite).x = 1500.f;
        minY = std::min(minY, reg.get<gameLogic::Vec2Pos>(elite).y);
        maxY = std::max(maxY, reg.get<gameLogic::Vec2Pos>(elite).y);
    }
    CHECK(maxY - minY > 100.f);
    CHECK(minY >= -1.f);
}

TEST(a_rocket_halts_to_charge_before_firing_then_moves_again)
{
    Engine::Registry reg;
    const Engine::Entity rocket = placed(reg, MonsterKind::Rocket, 300.f);

    reg.get<Weapon>(rocket).timer = 2.f;
    monsterSystem(reg, config::FIXED_DT);
    CHECK(reg.get<gameLogic::Vec2Velocity>(rocket).x < 0.f);
    reg.get<Weapon>(rocket).timer = config::ROCKET_CHARGE_TIME / 2.f;
    monsterSystem(reg, config::FIXED_DT);
    CHECK(reg.get<gameLogic::Vec2Velocity>(rocket).x == 0.f);
    reg.get<Weapon>(rocket).timer = 3.f;
    monsterSystem(reg, config::FIXED_DT);
    CHECK(reg.get<gameLogic::Vec2Velocity>(rocket).x < 0.f);
}

TEST(a_bomber_drops_its_bomb_on_the_side_of_the_player)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity bomber = placed(reg, MonsterKind::Bomber, 400.f);

    reg.get<gameLogic::Vec2Pos>(player) = gameLogic::Vec2Pos{{300.f, 900.f}};
    CHECK(shotVelocities(reg, bomber)[0].y > 0.f);
    reg.get<gameLogic::Vec2Pos>(player) = gameLogic::Vec2Pos{{300.f, 20.f}};
    CHECK(shotVelocities(reg, bomber)[0].y < 0.f);
}

TEST(a_boss_is_not_moved_by_the_monster_behaviours)
{
    Engine::Registry reg;
    const Engine::Entity boss = spawnBoss(reg, BossKind::Standard);

    reg.get<gameLogic::Vec2Pos>(boss).x = 1500.f;
    monsterSystem(reg, config::FIXED_DT);
    CHECK(reg.get<MonsterState>(boss).age == 0.f);
}

namespace {
    Engine::Entity enteredBoss(Engine::Registry &reg, const BossKind kind)
    {
        const Engine::Entity boss = spawnBoss(reg, kind);

        reg.get<gameLogic::Vec2Pos>(boss).x = 1200.f;
        return boss;
    }

    void runBoss(Engine::Registry &reg, const float seconds)
    {
        for (float t = 0.f; t < seconds; t += config::FIXED_DT) {
            bossSystem(reg, config::FIXED_DT);
            movementSystem(reg, config::FIXED_DT);
        }
    }
}

TEST(the_summoner_calls_kamikazes_at_regular_intervals)
{
    Engine::Registry reg;

    enteredBoss(reg, BossKind::Summoner);
    runBoss(reg, config::SUMMON_INTERVAL - 1.f);
    CHECK(countMinions(reg) == 0);
    runBoss(reg, 1.5f);
    CHECK(countMinions(reg) == config::SUMMON_COUNT);
}

TEST(the_summoner_never_has_more_minions_than_the_cap)
{
    Engine::Registry reg;

    enteredBoss(reg, BossKind::Summoner);
    runBoss(reg, config::SUMMON_INTERVAL * 6.f);
    CHECK(countMinions(reg) <= config::SUMMON_MAX_ALIVE);
    CHECK(countMinions(reg) > 0);
}

TEST(summoned_minions_drop_no_coin)
{
    Engine::Registry reg;
    std::mt19937 rng(1);
    const Engine::Entity boss = enteredBoss(reg, BossKind::Summoner);

    summonMinions(reg, boss);
    for (const Engine::Entity e : collectMonsters(reg)) {
        if (reg.get<MonsterState>(e).summoned) {
            CHECK(spawnCoin(reg, e, rng) == Engine::Entity{});
        }
    }
}

TEST(the_fortress_fires_faster_during_its_barrage)
{
    Engine::Registry reg;
    const Engine::Entity boss = enteredBoss(reg, BossKind::Fortress);

    reg.get<BossTag>(boss).age = 0.1f;
    reg.get<Weapon>(boss).timer = 1.6f;
    bossSystem(reg, config::FIXED_DT);
    CHECK(reg.get<Weapon>(boss).timer > 1.f);
    reg.get<BossTag>(boss).age = config::FORTRESS_BURST_PERIOD - 1.f;
    bossSystem(reg, config::FIXED_DT);
    CHECK(reg.get<Weapon>(boss).timer <= config::FORTRESS_BURST_COOLDOWN);
}

TEST(the_fortress_stays_on_the_map_while_it_sways)
{
    Engine::Registry reg;
    const Engine::Entity boss = enteredBoss(reg, BossKind::Fortress);
    const float maxY = config::MAP_HEIGHT
        - reg.get<gameLogic::Vec2Hitbox>(boss).y;

    for (int i = 0; i < 60 * 30; ++i) {
        bossSystem(reg, config::FIXED_DT);
        movementSystem(reg, config::FIXED_DT);
        CHECK(reg.get<gameLogic::Vec2Pos>(boss).y >= -1.f);
        CHECK(reg.get<gameLogic::Vec2Pos>(boss).y <= maxY + 1.f);
    }
}

TEST(each_kill_gives_half_a_second_of_scrolling)
{
    const Segments segments = firstLevel();
    SkipBank bank;

    CHECK(grantSkip(segments, bank, 0.f, 2)
        == 2.f * 0.5f * config::SCROLL_SPEED);
}

TEST(the_skip_of_a_level_is_capped)
{
    const Segments segments = firstLevel();
    const float cap = config::KILL_SKIP_MAX_FRACTION
        * levelSpan(segments, 0.f);
    SkipBank bank;
    const float first = grantSkip(segments, bank, 0.f, 1000);

    CHECK(first == cap);
    CHECK(grantSkip(segments, bank, first, 5) == 0.f);
}

TEST(the_skip_bank_starts_over_on_a_new_level)
{
    Segments segments = firstLevel();
    SkipBank bank;

    segments.levels.push_back({2, 1000.f});
    grantSkip(segments, bank, 0.f, 1000);
    CHECK(grantSkip(segments, bank, 1000.f, 1) > 0.f);
}

TEST(only_killed_monsters_count_as_kills)
{
    Engine::Registry reg;
    std::mt19937 rng(1);
    const Engine::Entity basic = spawnMonster(reg, 300.f);
    const Engine::Entity minion = spawnMonster(reg, 500.f);
    const Engine::Entity boss = spawnBoss(reg);

    reg.get<MonsterState>(minion).summoned = true;
    reg.get<Health>(basic).hp = 0;
    reg.get<Health>(minion).hp = 0;
    reg.get<Health>(boss).hp = 0;
    CHECK(deathSystem(reg, rng) == 1);
}

TEST(a_wall_that_spawns_late_is_placed_back_by_the_overshoot)
{
    Engine::Registry reg;
    const Engine::Entity block =
        createObstacle(reg, wall(0.f, 0.f, 64.f, 64.f), 50.f);

    CHECK(reg.get<gameLogic::Vec2Pos>(block).x == config::MAP_WIDTH - 50.f);
}

TEST(the_summoner_retries_as_soon_as_there_is_room)
{
    Engine::Registry reg;
    const Engine::Entity boss = enteredBoss(reg, BossKind::Summoner);

    for (int i = 0; i < 3; ++i) {
        summonMinions(reg, boss);
    }
    runBoss(reg, config::SUMMON_INTERVAL + 0.5f);
    const int before = countMinions(reg);

    for (const Engine::Entity e : collectMonsters(reg)) {
        if (reg.get<MonsterState>(e).summoned) {
            reg.destroy(e);
        }
    }
    runBoss(reg, 0.2f);
    CHECK(before > 0);
    CHECK(countMinions(reg) == config::SUMMON_COUNT);
}

TEST(a_game_with_a_lobby_waits_for_a_map)
{
    Game game(1);

    game.openLobby();
    game.addPlayer();
    for (int i = 0; i < 120; ++i) {
        game.update(config::FIXED_DT);
    }
    CHECK(game.phase() == Phase::Lobby);
    CHECK(game.scroll() == 0.f);
    CHECK(game.status().elapsedMs == 0);
}

TEST(a_map_can_only_be_started_from_the_lobby)
{
    Game game(1);

    game.addPlayer();
    CHECK(!game.startMap(firstLevel(), 0));
    game.openLobby();
    CHECK(game.startMap(*buildMap(2), 2));
    CHECK(game.phase() == Phase::Playing);
    CHECK(game.map() == 2);
    CHECK(game.status().map == 2);
    CHECK(!game.startMap(firstLevel(), 0));
}

TEST(the_host_is_the_first_connected_player)
{
    Game game(1);

    CHECK(!game.host().has_value());
    game.addPlayer();
    game.addPlayer();
    CHECK(game.host() == 0);
    game.removePlayer(0);
    CHECK(game.host() == 1);
}

TEST(the_game_goes_back_to_the_lobby_after_the_end_screen)
{
    Game game(1);

    game.openLobby();
    game.addPlayer();
    game.startMap(firstLevel(), 0);
    kill(game, 0);
    game.update(config::FIXED_DT);
    CHECK(game.phase() == Phase::Defeat);
    for (float t = 0.f; t < config::RESTART_DELAY + 1.f; t += config::FIXED_DT) {
        game.update(config::FIXED_DT);
    }
    CHECK(game.phase() == Phase::Lobby);
}

TEST(the_lobby_is_back_when_everybody_leaves)
{
    Game game(1);

    game.openLobby();
    game.addPlayer();
    game.startMap(firstLevel(), 0);
    game.removePlayer(0);
    game.update(config::FIXED_DT);
    CHECK(game.phase() == Phase::Lobby);
}

TEST(the_best_times_are_sorted_capped_and_saved)
{
    Leaderboard board;
    const auto file = std::filesystem::temp_directory_path()
        / "rtype_leaderboard_test.txt";

    for (std::uint32_t i = 0; i < 8; ++i) {
        board.add(1, {(8 - i) * 1000, 1});
    }
    CHECK(!board.add(static_cast<MapId>(MAP_COUNT), {1, 1}));
    CHECK(board.all()[1].size() == MAX_RECORDS);
    CHECK(board.all()[1].front().timeMs == 1000);
    CHECK(board.save(file));
    Leaderboard loaded;

    loaded.load(file);
    CHECK(loaded.all()[1].size() == MAX_RECORDS);
    CHECK(loaded.all()[1].back().timeMs == 5000);
    std::filesystem::remove(file);
}

TEST(a_fast_wall_travels_faster_than_the_scrolling)
{
    Engine::Registry reg;
    ObstacleStep step = wall(0.f, 0.f, 100.f, 100.f, ObstacleKind::Spike);

    step.speed = 2.f;
    const Engine::Entity e = createObstacle(reg, step);

    CHECK(reg.get<gameLogic::Vec2Velocity>(e).x
        == -config::OBSTACLE_SPEED * 2.f);
}

TEST(the_lobby_can_be_reopened_during_a_match)
{
    Game game(1);

    game.addPlayer();
    game.openLobby();
    game.startMap(*buildMap(1), 1);
    game.update(1.f);
    game.openLobby();
    CHECK(game.phase() == Phase::Lobby);
    CHECK(game.startMap(*buildMap(0), 0));
    CHECK(game.map() == 0);
}

TEST(a_paused_match_does_not_advance)
{
    Game game(1);

    game.addPlayer();
    game.openLobby();
    game.startMap(*buildMap(0), 0);
    game.update(0.1f);
    game.setPaused(true);
    const auto before = game.status().elapsedMs;
    game.update(1.f);
    CHECK(game.status().paused);
    CHECK(game.status().elapsedMs == before);
    game.setPaused(false);
    game.update(1.f);
    CHECK(game.status().elapsedMs > before);
}

TEST(the_lobby_cannot_be_paused)
{
    Game game(1);

    game.addPlayer();
    game.openLobby();
    game.setPaused(true);
    CHECK(!game.paused());
}
