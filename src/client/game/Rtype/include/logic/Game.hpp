//
// Created by antoine on 10/17/26.
//

#pragma once
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <map>
#include <optional>
#include <random>
#include <set>
#include <utility>
#include <vector>
#include "Augments.hpp"
#include "Boss.hpp"
#include "BossHazards.hpp"
#include "Charge.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Drone.hpp"
#include "Difficulty.hpp"
#include "Escort.hpp"
#include "Events.hpp"
#include "Formations.hpp"
#include "Factories.hpp"
#include "LevelSkip.hpp"
#include "Levels.hpp"
#include "Lives.hpp"
#include "MapList.hpp"
#include "MonsterBehavior.hpp"
#include "Monsters.hpp"
#include "Queries.hpp"
#include "Replication.hpp"
#include "Rocket.hpp"
#include "Secondary.hpp"
#include "Scaling.hpp"
#include "Settings.hpp"
#include "Shop.hpp"
#include "Status.hpp"
#include "Systems.hpp"
#include "Waves.hpp"
#include "Zones.hpp"
#include "engine/core/Registry.hpp"

namespace rtype {
    class Game {
    public:
        Game() : Game(std::random_device{}()) {}

        explicit Game(const unsigned int seed) : Game(seed, firstLevel()) {}

        Game(const unsigned int seed, Segments segments)
            : _segments(std::move(segments)), _rng(seed)
        {
            _coinCounter = createCoinCounter(_reg);
            createSettingsHolder(_reg);
            _lives.refill(activeScale(_reg).lives);
        }

        std::optional<int> addPlayer()
        {
            const std::optional<int> slot = findFreeSlot(_usedSlots);

            if (!slot) {
                return std::nullopt;
            }
            spawnPlayer(*slot);
            ++_alive;
            _usedSlots[static_cast<std::size_t>(*slot)] = true;
            _lives.grantOne(*slot);
            return slot;
        }

        void removePlayer(const int slot)
        {
            const std::optional<Engine::Entity> player =
                findPlayerBySlot(_reg, slot);

            if (player) {
                _reg.destroy(*player);
                --_alive;
            }
            if (isUsedSlot(slot)) {
                _usedSlots[static_cast<std::size_t>(slot)] = false;
            }
            _augments.erase(slot);
            _ready.erase(slot);
        }

        // Les reglages valent pour la prochaine partie (voir startMap).
        void setSettings(const Settings &settings)
        {
            storeSettings(_reg, settings);
            _lives.refill(scaleOf(settings).lives);
        }

        [[nodiscard]] Settings settings()
        {
            return activeSettings(_reg);
        }

        [[nodiscard]] int livesOf(const int slot) const
        {
            if (slot < 0 || slot >= SLOTS) {
                return 0;
            }
            return _lives.lives(slot);
        }

        // Un joueur mort (dont la vie est deja perdue) revient seul apres un
        // delai ; ceci l'accelere.
        bool respawnPlayer(const int slot)
        {
            if (_phase != Phase::Playing || !isUsedSlot(slot)
                || isPlayerAlive(slot) || !_lives.isDown(slot)
                || _lives.lives(slot) <= 0) {
                return false;
            }
            spawnPlayer(slot);
            ++_alive;
            return true;
        }

        bool isPlayerAlive(const int slot)
        {
            return findPlayerBySlot(_reg, slot).has_value();
        }

        bool addAugment(const int slot, const AugmentId id)
        {
            if (!isUsedSlot(slot) || !findAugment(id)) {
                return false;
            }
            const std::optional<Engine::Entity> player =
                findPlayerBySlot(_reg, slot);

            _augments[slot].push_back(id);
            if (player) {
                applyAugment(_reg, *player, id);
            }
            return true;
        }

        [[nodiscard]] std::vector<AugmentId> ownedAugments(const int slot) const
        {
            const auto owned = _augments.find(slot);

            if (owned == _augments.end()) {
                return {};
            }
            return owned->second;
        }

        void restockShop(const ShopTier &tier)
        {
            const std::size_t count = config::SHOP_OFFER_COUNT
                + static_cast<std::size_t>(activeScale(_reg).extraOffers);

            _shopOffers = rollOffers(_rng, count, tier, _purchased);
        }

        [[nodiscard]] const std::vector<AugmentId> &shopOffers() const
        {
            return _shopOffers;
        }

        bool buyAugment(const int slot, const AugmentId id)
        {
            const auto offer = std::ranges::find(_shopOffers, id);
            std::size_t &coins = _reg.get<CoinCounter>(_coinCounter).total;

            if (_phase != Phase::Shop || !isUsedSlot(slot)
                || offer == _shopOffers.end() || coins < priceOf(id)) {
                return false;
            }
            coins -= priceOf(id);
            _shopOffers.erase(offer);
            _purchased.push_back(id);
            return addAugment(slot, id);
        }

        bool setReady(const int slot, const bool ready)
        {
            if (_phase != Phase::Shop || !isUsedSlot(slot)) {
                return false;
            }
            if (ready) {
                _ready.insert(slot);
            } else {
                _ready.erase(slot);
            }
            return true;
        }

        [[nodiscard]] std::size_t coins() const
        {
            return _reg.get<CoinCounter>(_coinCounter).total;
        }

        void setInput(const int slot, const PlayerInput &input)
        {
            const std::optional<Engine::Entity> player =
                findPlayerBySlot(_reg, slot);

            if (player) {
                _reg.get<PlayerInput>(*player) = input;
            }
        }

        // Le jeu attend alors qu'un joueur choisisse une map (startMap).
        void openLobby()
        {
            _lobby = true;
            reset();
        }

        // Le premier joueur connecte est celui qui choisit la map.
        [[nodiscard]] std::optional<int> host() const
        {
            for (int slot = 0; slot < SLOTS; ++slot) {
                if (isUsedSlot(slot)) {
                    return slot;
                }
            }
            return std::nullopt;
        }

        // `zones` ne sert qu'aux maps de joueur (CUSTOM_MAP) : une par niveau.
        bool startMap(Segments segments, const MapId map,
            std::vector<Zone> zones = {})
        {
            if (_phase != Phase::Lobby) {
                return false;
            }
            _segments = std::move(segments);
            _map = map;
            _customZones = std::move(zones);
            reset();
            _phase = Phase::Playing;
            return true;
        }

        [[nodiscard]] MapId map() const
        {
            return _map;
        }

        // Appele a chaque tick : mouvements, tirs, collisions, niveau...
        void update(const float dt)
        {
            if (!hasPlayers()) {
                idle();
            } else if (_paused) {
                return;
            } else if (_phase == Phase::Playing) {
                updatePlaying(dt);
            } else if (_phase == Phase::Lobby) {
                flyPlayers(dt);
            } else if (_phase == Phase::Shop) {
                updateShop(dt);
            } else {
                updateEnded(dt);
            }
            trackDestroys();
            trackSpawns();
        }

        std::vector<GameEvent> takeEvents()
        {
            return std::exchange(_events, {});
        }

        [[nodiscard]] std::vector<EntitySpawned> spawns() const
        {
            return collectSpawns(_reg, _tracked);
        }

        [[nodiscard]] std::vector<EntityState> states() const
        {
            return collectStates(_reg, _tracked);
        }

        // Phase, vie du boss et vie des joueurs : ce qu'affiche le HUD.
        GameStatus status()
        {
            GameStatus status;

            describeMatch(status);
            for (const Engine::Entity boss : collectWith<BossTag>(_reg)) {
                status.bossHp = std::max(0, _reg.get<Health>(boss).hp);
                status.bossMaxHp = bossMaxHp(_reg, boss);
            }
            status.countdown = _countdown.value_or(0.f);
            for (int slot = 0; slot < SLOTS; ++slot) {
                if (isUsedSlot(slot)) {
                    status.players.push_back(statusOf(slot));
                }
            }
            return status;
        }

        void setPaused(const bool paused)
        {
            _paused = paused && _phase == Phase::Playing;
            resetCharges(_reg);
        }

        [[nodiscard]] bool paused() const
        {
            return _paused;
        }

        [[nodiscard]] Phase phase() const
        {
            return _phase;
        }

        // Distance defilee depuis le debut de la partie.
        [[nodiscard]] float scroll() const
        {
            return _scroll;
        }

        [[nodiscard]] std::size_t level() const
        {
            return levelAt(_segments, _scroll);
        }

        Engine::Registry &registry()
        {
            return _reg;
        }

    private:
        static constexpr int SLOTS = static_cast<int>(config::MAX_PLAYERS);

        struct PendingSpawn {
            float delay;
            float y;
            MonsterKind kind;
        };

        [[nodiscard]] bool isUsedSlot(const int slot) const
        {
            const auto index = static_cast<std::size_t>(slot);

            return slot >= 0 && index < _usedSlots.size() && _usedSlots[index];
        }

        [[nodiscard]] bool hasPlayers() const
        {
            return std::ranges::any_of(_usedSlots, [](const bool used) {
                return used;
            });
        }

        void describeMatch(GameStatus &status)
        {
            status.phase = _phase;
            status.paused = _paused;
            status.deaths = _deaths;
            status.settings = settings();
            status.map = _map;
            status.coins = static_cast<std::uint32_t>(
                _reg.get<CoinCounter>(_coinCounter).total);
            status.elapsedMs = static_cast<std::uint32_t>(
                _elapsed * config::MS_PER_SECOND);
            status.level = static_cast<std::uint8_t>(std::min<std::size_t>(
                level(), std::numeric_limits<std::uint8_t>::max()));
        }

        PlayerStatus statusOf(const int slot)
        {
            const std::optional<Engine::Entity> player =
                findPlayerBySlot(_reg, slot);
            PlayerStatus status{slot, 0, false, _ready.contains(slot),
                _lives.lives(slot)};

            if (player) {
                status.alive = true;
                status.hp = std::max(0, _reg.get<Health>(*player).hp);
            }
            return status;
        }

        // Personne n'est connecte : le niveau n'avance pas, et repart de zero.
        void idle()
        {
            if (_scroll > 0.f || _nextBoss > 0 || _phase != restingPhase()
                || !_pending.empty()) {
                reset();
            }
        }

        void updatePlaying(const float dt)
        {
            _elapsed += dt;
            updateWeaponTimers(_reg, dt);
            updateShieldTimers(_reg, dt);
            regenerationSystem(_reg, dt);
            lifetimeSystem(_reg, dt);
            playerControlSystem(_reg);
            advanceLevel(dt);
            bossHazards(dt);
            monsterSystem(_reg, dt);
            obstacleMotionSystem(_reg, dt);
            turretSystem(_reg, dt);
            laserSystem(_reg, dt);
            strikeSystem(_reg, dt);
            bossSystem(_reg, dt);
            chargeSystem(_reg, dt);
            secondarySystem(_reg, dt);
            scaleNewEnemies(_reg);
            shootingSystem(_reg);
            movementSystem(_reg, dt);
            droneSystem(_reg, dt);
            collisionSystem(_reg);
            rocketSystem(_reg);
            contactSystem(_reg);
            obstacleSystem(_reg);
            coinPickupSystem(_reg);
            deleteUnitsOutOfBounds(_reg);
            protectPlayers();
            skipLevel(deathSystem(_reg, _rng));
            trackLives(dt);
            failRun(countDeaths());
            updatePhase();
        }

        void bossHazards(const float dt)
        {
            if (collectWith<BossTag>(_reg).empty()) {
                clearBossHazards();
                storm(dt);
                return;
            }
            bossHazardSystem(_reg, _hazards, _rng, bossIntensity(), dt);
        }

        [[nodiscard]] Zone currentZone() const
        {
            if (_map == CUSTOM_MAP) {
                return customZone(_customZones, level());
            }
            return zoneOf(_map, level());
        }

        // La meteo de la zone : une meteorite de temps en temps.
        void storm(const float dt)
        {
            const float every = stormInterval(currentZone());

            if (every <= 0.f) {
                return;
            }
            _storm -= dt;
            if (_storm <= 0.f) {
                spawnMeteor(_reg, _rng);
                _storm = every;
            }
        }

        void clearBossHazards()
        {
            if (_hazards.time > 0.f) {
                destroyAll(collectObstacles(_reg));
            }
            _hazards = HazardClock{};
        }

        // Plus on avance dans les boss de la map, plus le combat est dense.
        [[nodiscard]] float bossIntensity() const
        {
            const std::size_t total = _segments.bossSpawn.size();

            if (total == 0) {
                return 1.f;
            }
            return static_cast<float>(_nextBoss) / static_cast<float>(total);
        }

        // Partie finie : les joueurs volent encore, puis tout recommence.
        void flyPlayers(const float dt)
        {
            lifetimeSystem(_reg, dt);
            playerControlSystem(_reg);
            movementSystem(_reg, dt);
        }

        [[nodiscard]] Phase restingPhase() const
        {
            if (_lobby) {
                return Phase::Lobby;
            }
            return Phase::Playing;
        }

        void leaveEndScreen()
        {
            const bool retry = _phase == Phase::Defeat;

            reset();
            if (retry) {
                _phase = Phase::Playing;
            }
        }

        int aliveCount()
        {
            int count = 0;

            for (int slot = 0; slot < SLOTS; ++slot) {
                if (isUsedSlot(slot) && isPlayerAlive(slot)) {
                    ++count;
                }
            }
            return count;
        }

        int countDeaths()
        {
            const int now = aliveCount();
            const int lost = std::max(0, _alive - now);

            _deaths = static_cast<std::uint16_t>(_deaths + lost);
            _alive = now;
            return lost;
        }

        [[nodiscard]] bool inDeathRun()
        {
            return isDeathRun(_segments, _scroll)
                && collectWith<BossTag>(_reg).empty();
        }

        // Une mort en death run : toute l equipe repart du debut du niveau.
        void restartDeathRun()
        {
            const float start = levelStart(_segments, _scroll);

            destroyAll(collectMonsters(_reg));
            destroyAll(collectProjectileEntities(_reg));
            destroyAll(collectObstacles(_reg));
            _pending.clear();
            _nextWave = countBefore(_segments.waves, start);
            _nextObstacle = countBefore(_segments.obstacles, start);
            _scroll = restartScroll(start);
            _effectLevel = level();
            respawnEveryone();
        }

        // Le premier obstacle repart deja pres du joueur : pas d attente.
        [[nodiscard]] float restartScroll(const float start) const
        {
            if (_nextObstacle >= _segments.obstacles.size()) {
                return start;
            }
            const ObstacleStep &first = _segments.obstacles[_nextObstacle];
            const float travel = (config::MAP_WIDTH - config::RESTART_LEAD)
                / first.speed;

            return std::clamp(first.at + travel, start,
                levelEnd(_segments, start));
        }

        template <typename Step>
        static std::size_t countBefore(const std::vector<Step> &steps,
            const float start)
        {
            return static_cast<std::size_t>(std::ranges::count_if(steps,
                [start](const Step &step) { return step.at < start; }));
        }

        void respawnEveryone()
        {
            for (int slot = 0; slot < SLOTS; ++slot) {
                if (!isUsedSlot(slot)) {
                    continue;
                }
                if (const auto player = findPlayerBySlot(_reg, slot)) {
                    _reg.destroy(*player);
                }
                spawnPlayer(slot);
            }
            _alive = aliveCount();
        }

        void failRun(const int lost)
        {
            if (lost > 0 && inDeathRun()) {
                restartDeathRun();
            }
        }

        void updateEnded(const float dt)
        {
            flyPlayers(dt);
            _phaseTimer -= dt;
            if (_phaseTimer > 0.f) {
                return;
            }
            if (_phase == Phase::Victory) {
                enterShop(level(), true);
            } else {
                leaveEndScreen();
            }
        }

        void endPortalEffects()
        {
            const std::size_t now = level();

            if (now == _effectLevel) {
                return;
            }
            const std::size_t finished = _effectLevel;

            _effectLevel = now;
            _reg.each<PlayerEffects>(
                [](const Engine::Entity, PlayerEffects &effects) {
                    effects = PlayerEffects{};
                });
            if (finished >= 1 && now > finished) {
                enterShop(finished, false);
            }
        }

        [[nodiscard]] bool levelHadBoss(const std::size_t number) const
        {
            const float start = levelStartOf(_segments, number);
            const float end = levelEnd(_segments, start);

            return std::ranges::any_of(_segments.bossSpawn,
                [start, end](const BossSpawn &boss) {
                    return boss.second >= start && boss.second < end;
                });
        }


        void skipLevel(const int kills)
        {
            if (kills > 0 && collectWith<BossTag>(_reg).empty()
                && !inDeathRun()) {
                _scroll += grantSkip(_segments, _skip, _scroll, kills);
            }
        }

        // Le shop s'ouvre a la fin de chaque niveau et apres la victoire :
        // le jeu attend que tous les joueurs soient prets.
        void enterShop(const std::size_t finished, const bool last)
        {
            _phase = Phase::Shop;
            _shopLast = last;
            _ready.clear();
            _countdown.reset();
            if (!last) {
                clearField();
            }
            resetCharges(_reg);
            reviveEveryone();
            restockShop(ShopTier{static_cast<int>(finished)
                + activeScale(_reg).shopLevelBonus,
                last || levelHadBoss(finished)});
        }

        void clearField()
        {
            _pending.clear();
            destroyAll(collectMonsters(_reg));
            destroyAll(collectProjectileEntities(_reg));
            destroyAll(collectWith<Missile>(_reg));
            destroyAll(collectObstacles(_reg));
        }

        // Apres le dernier shop on retourne au lobby, sinon le niveau suivant
        // continue la partie.
        void leaveShop()
        {
            _ready.clear();
            _countdown.reset();
            _shopOffers.clear();
            if (_shopLast) {
                reset();
                return;
            }
            _phase = Phase::Playing;
        }

        void updateShop(const float dt)
        {
            lifetimeSystem(_reg, dt);
            playerControlSystem(_reg);
            movementSystem(_reg, dt);
            updateCountdown(dt);
        }

        void updateCountdown(const float dt)
        {
            if (!allReady()) {
                _countdown.reset();
                return;
            }
            if (!_countdown) {
                _countdown = config::SHOP_COUNTDOWN;
            }
            *_countdown -= dt;
            if (*_countdown <= 0.f) {
                leaveShop();
            }
        }

        [[nodiscard]] bool allReady() const
        {
            for (int slot = 0; slot < SLOTS; ++slot) {
                if (isUsedSlot(slot) && !_ready.contains(slot)) {
                    return false;
                }
            }
            return true;
        }

        void advanceLevel(const float dt)
        {
            if (collectWith<BossTag>(_reg).empty()) {
                _scroll += config::SCROLL_SPEED * dt;
                endPortalEffects();
                queueDueWaves();
                spawnDueBosses();
                spawnDueObstacles();
            }
            spawnPending(dt);
        }

        void spawnDueBosses()
        {
            const auto &bosses = _segments.bossSpawn;

            while (_nextBoss < bosses.size()
                && bosses[_nextBoss].second <= _scroll) {
                spawnBoss(_reg, bosses[_nextBoss].first);
                queueEscort(_nextBoss);
                ++_nextBoss;
            }
        }

        // En Brutal, des mini-boss arrivent avec le boss.
        void queueEscort(const std::size_t bossIndex)
        {
            const Difficulty difficulty = activeSettings(_reg).difficulty;

            for (const EscortSpawn &spawn : escortFor(difficulty, bossIndex)) {
                _pending.push_back({spawn.delay, spawn.y, spawn.kind});
            }
        }

        void spawnDueObstacles()
        {
            const auto &obstacles = _segments.obstacles;

            while (_nextObstacle < obstacles.size()
                && obstacles[_nextObstacle].at <= _scroll) {
                createObstacle(_reg, obstacles[_nextObstacle],
                    _scroll - obstacles[_nextObstacle].at);
                ++_nextObstacle;
            }
        }

        [[nodiscard]] bool allBossesSpawned() const
        {
            return _nextBoss >= _segments.bossSpawn.size();
        }

        void queueDueWaves()
        {
            const auto &waves = _segments.waves;

            while (_nextWave < waves.size() && waves[_nextWave].at <= _scroll) {
                queueWave(densified(waves[_nextWave],
                    activeScale(_reg).density));
                ++_nextWave;
            }
        }

        float spawnHeight(const WaveStep &wave, const float index)
        {
            std::uniform_real_distribution<float> randomY(
                config::WAVE_MIN_Y, config::WAVE_MAX_Y);

            if (wave.y < 0.f) {
                return randomY(_rng);
            }
            return wave.y + index * wave.dy;
        }

        void queueFormation(const WaveStep &wave)
        {
            std::uniform_real_distribution<float> randomY(
                config::WAVE_MIN_Y + 300.f, config::WAVE_MAX_Y - 300.f);
            float base = wave.y;

            if (base < 0.f) {
                base = randomY(_rng);
            }
            for (int i = 0; i < wave.count; ++i) {
                const Slot slot = formationSlot(wave, i, base);

                _pending.push_back({slot.delay, slot.y, wave.kind});
            }
        }

        void queueWave(const WaveStep &wave)
        {
            if (wave.shape != Shape::Line) {
                queueFormation(wave);
                return;
            }
            for (int i = 0; i < wave.count; ++i) {
                const auto n = static_cast<float>(i);

                _pending.push_back({n * wave.interval,
                    spawnHeight(wave, n), wave.kind});
            }
        }

        // Hauteur testee au rang `step` autour de y : haut, bas, haut...
        static float probeTop(const float y, const int step,
            const float maxY)
        {
            float side = 1.f;

            if (step % 2 == 0) {
                side = -1.f;
            }
            return std::clamp(y + side * config::SPAWN_STEP * ((step + 1) / 2),
                config::MAP_ORIGIN, maxY);
        }

        // Cherche une hauteur libre pres de y : on ne nait pas dans un mur.
        static std::optional<float> clearSpawnY(const float y,
            const MonsterDef &def, const std::vector<Collider> &walls)
        {
            const float x = config::MAP_WIDTH + config::SPAWN_MARGIN;
            const float maxY = config::MAP_HEIGHT - def.height;

            for (int step = 0; step <= 2 * config::SPAWN_SEARCH; ++step) {
                const float top = probeTop(y, step, maxY);
                const Collider body{Engine::Entity{}, {{x, top}},
                    {{def.width, def.height}}, Team{}};

                if (!touchesAny(body, walls)) {
                    return top;
                }
            }
            return std::nullopt;
        }

        bool spawnClear(const PendingSpawn &spawn,
            const std::vector<Collider> &walls)
        {
            const std::optional<float> y = clearSpawnY(spawn.y,
                monsterDef(spawn.kind), walls);

            if (y) {
                spawnMonster(_reg, *y, spawn.kind);
            }
            return y.has_value();
        }

        // Les monstres d'une vague arrivent les uns apres les autres.
        void spawnPending(const float dt)
        {
            std::vector<PendingSpawn> waiting;
            std::vector<Collider> walls;

            if (!_pending.empty()) {
                walls = obstacleColliders(_reg, isWall);
            }
            for (PendingSpawn &spawn : _pending) {
                spawn.delay -= dt;
                if (spawn.delay <= 0.f) {
                    if (spawnClear(spawn, walls)) {
                        continue;
                    }
                    spawn.delay = config::SPAWN_RETRY;
                }
                waiting.push_back(spawn);
            }
            _pending = std::move(waiting);
        }

        void protectPlayers()
        {
            if (config::PLAYERS_INVINCIBLE) {
                healPlayers(_reg);
            }
        }

        void updatePhase()
        {
            if (allBossesSpawned() && collectWith<BossTag>(_reg).empty()) {
                endGame(Phase::Victory);
            } else if (nobodyCanFight()) {
                endGame(Phase::Defeat);
            }
        }

        // Defait : plus personne en vie, et plus personne qui puisse revenir.
        bool nobodyCanFight()
        {
            for (int slot = 0; slot < SLOTS; ++slot) {
                if (isUsedSlot(slot)
                    && (isPlayerAlive(slot) || _lives.lives(slot) > 0)) {
                    return false;
                }
            }
            return true;
        }

        // Une mort coute une vie, sauf en death run (toute l'equipe repart).
        void trackLives(const float dt)
        {
            for (int slot = 0; slot < SLOTS && !inDeathRun(); ++slot) {
                if (isUsedSlot(slot) && !isPlayerAlive(slot)
                    && !_lives.isDown(slot)) {
                    _lives.lose(slot);
                }
            }
            for (const int slot : _lives.tick(dt)) {
                respawnPlayer(slot);
            }
        }

        // Au shop, les joueurs elimines reviennent avec une vie.
        void reviveEveryone()
        {
            for (int slot = 0; slot < SLOTS; ++slot) {
                if (isUsedSlot(slot) && !isPlayerAlive(slot)) {
                    _lives.grantOne(slot);
                    spawnPlayer(slot);
                }
            }
            _alive = aliveCount();
        }

        // Fin de partie : on retire les ennemis et leurs balles.
        void endGame(const Phase phase)
        {
            _phase = phase;
            _phaseTimer = config::RESTART_DELAY;
            _pending.clear();
            destroyAll(collectMonsters(_reg));
            destroyAll(collectProjectileEntities(_reg));
            destroyAll(collectWith<Missile>(_reg));
            destroyAll(collectObstacles(_reg));
        }

        void destroyAll(const std::vector<Engine::Entity> &entities)
        {
            for (const Engine::Entity e : entities) {
                _reg.destroy(e);
            }
        }

        // Nouvelle partie : monde vide, les joueurs connectes repartent a neuf.
        void reset()
        {
            destroyAll(collectWith<Renderable>(_reg));
            _pending.clear();
            _scroll = 0.f;
            _nextWave = 0;
            _nextBoss = 0;
            _nextObstacle = 0;
            _skip = SkipBank{};
            _effectLevel = 0;
            _paused = false;
            _hazards = HazardClock{};
            _storm = 2.f;
            _deaths = 0;
            _reg.get<CoinCounter>(_coinCounter).total = 0;
            _phase = restingPhase();
            _phaseTimer = 0.f;
            _elapsed = 0.f;
            _ready.clear();
            _countdown.reset();
            _shopOffers.clear();
            _purchased.clear();
            _augments.clear();
            _shopLast = false;
            _lives.refill(activeScale(_reg).lives);
            for (int slot = 0; slot < SLOTS; ++slot) {
                if (isUsedSlot(slot)) {
                    spawnPlayer(slot);
                }
            }
            _alive = aliveCount();
        }

        void spawnPlayer(const int slot)
        {
            const Engine::Entity player = createPlayer(_reg, slot);

            _lives.revive(slot);
            for (const AugmentId id : ownedAugments(slot)) {
                applyAugment(_reg, player, id);
            }
        }

        void trackSpawns()
        {
            for (const Engine::Entity e : collectUntracked(_reg)) {
                _reg.add(e, NetId{_nextNetId});
                _tracked[_nextNetId] = e;
                ++_nextNetId;
                if (const std::optional<EntitySpawned> spawn =
                    describeSpawn(_reg, e)) {
                    _events.emplace_back(*spawn);
                }
            }
        }

        void trackDestroys()
        {
            for (auto it = _tracked.begin(); it != _tracked.end();) {
                if (_reg.alive(it->second)) {
                    ++it;
                    continue;
                }
                _events.emplace_back(EntityDestroyed{it->first});
                it = _tracked.erase(it);
            }
        }

        Engine::Registry _reg;
        SlotTable _usedSlots{};
        std::map<int, std::vector<AugmentId>> _augments;
        std::vector<AugmentId> _shopOffers;
        std::vector<AugmentId> _purchased;
        std::set<int> _ready;
        std::optional<float> _countdown;
        bool _shopLast = false;
        Phase _phase = Phase::Playing;
        bool _lobby = false;
        bool _paused = false;
        std::uint16_t _deaths = 0;
        HazardClock _hazards;
        float _storm = 2.f;
        int _alive = 0;
        MapId _map = 0;
        std::vector<Zone> _customZones;
        float _phaseTimer = 0.f;
        float _elapsed = 0.f;
        float _scroll = 0.f;
        std::size_t _nextWave = 0;
        std::size_t _nextBoss = 0;
        std::size_t _nextObstacle = 0;
        SkipBank _skip;
        std::size_t _effectLevel = 0;
        Segments _segments;
        std::vector<PendingSpawn> _pending;
        TrackedTable _tracked;
        std::vector<GameEvent> _events;
        std::uint32_t _nextNetId = 1;
        std::mt19937 _rng;
        Engine::Entity _coinCounter;
        LifeBook _lives;
    };
}
