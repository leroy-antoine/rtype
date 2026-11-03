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
#include <utility>
#include <vector>
#include "Augments.hpp"
#include "Boss.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Events.hpp"
#include "Factories.hpp"
#include "LevelSkip.hpp"
#include "Levels.hpp"
#include "MapList.hpp"
#include "MonsterBehavior.hpp"
#include "Monsters.hpp"
#include "Queries.hpp"
#include "Replication.hpp"
#include "Status.hpp"
#include "Systems.hpp"
#include "Waves.hpp"
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
        }

        std::optional<int> addPlayer()
        {
            const std::optional<int> slot = findFreeSlot(_usedSlots);

            if (!slot) {
                return std::nullopt;
            }
            spawnPlayer(*slot);
            _usedSlots[static_cast<std::size_t>(*slot)] = true;
            return slot;
        }

        void removePlayer(const int slot)
        {
            const std::optional<Engine::Entity> player =
                findPlayerBySlot(_reg, slot);

            if (player) {
                _reg.destroy(*player);
            }
            if (isUsedSlot(slot)) {
                _usedSlots[static_cast<std::size_t>(slot)] = false;
            }
            _augments.erase(slot);
        }

        // Un joueur mort peut revenir tant que la partie n'est pas finie.
        bool respawnPlayer(const int slot)
        {
            if (_phase != Phase::Playing || !isUsedSlot(slot)
                || isPlayerAlive(slot)) {
                return false;
            }
            spawnPlayer(slot);
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

        bool startMap(Segments segments, const MapId map)
        {
            if (_phase != Phase::Lobby) {
                return false;
            }
            _segments = std::move(segments);
            _map = map;
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
                status.bossMaxHp =
                    bossDef(_reg.get<BossType>(boss).kind).hp;
            }
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
            PlayerStatus status{slot, 0, false};

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
            lifetimeSystem(_reg, dt);
            playerControlSystem(_reg);
            advanceLevel(dt);
            monsterSystem(_reg, dt);
            obstacleMotionSystem(_reg, dt);
            bossSystem(_reg, dt);
            shootingSystem(_reg);
            movementSystem(_reg, dt);
            collisionSystem(_reg);
            contactSystem(_reg);
            obstacleSystem(_reg);
            coinPickupSystem(_reg);
            deleteUnitsOutOfBounds(_reg);
            protectPlayers();
            skipLevel(deathSystem(_reg, _rng));
            updatePhase();
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

        void updateEnded(const float dt)
        {
            flyPlayers(dt);
            _phaseTimer -= dt;
            if (_phaseTimer <= 0.f) {
                reset();
            }
        }

        void endPortalEffects()
        {
            const std::size_t now = level();

            if (now == _effectLevel) {
                return;
            }
            _effectLevel = now;
            _reg.each<PlayerEffects>(
                [](const Engine::Entity, PlayerEffects &effects) {
                    effects = PlayerEffects{};
                });
        }

        void skipLevel(const int kills)
        {
            if (kills > 0 && collectWith<BossTag>(_reg).empty()) {
                _scroll += grantSkip(_segments, _skip, _scroll, kills);
            }
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
                ++_nextBoss;
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
                queueWave(waves[_nextWave]);
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

        void queueWave(const WaveStep &wave)
        {
            for (int i = 0; i < wave.count; ++i) {
                const auto n = static_cast<float>(i);

                _pending.push_back({n * wave.interval,
                    spawnHeight(wave, n), wave.kind});
            }
        }

        // Les monstres d'une vague arrivent les uns apres les autres.
        void spawnPending(const float dt)
        {
            std::vector<PendingSpawn> waiting;

            for (PendingSpawn &spawn : _pending) {
                spawn.delay -= dt;
                if (spawn.delay <= 0.f) {
                    spawnMonster(_reg, spawn.y, spawn.kind);
                } else {
                    waiting.push_back(spawn);
                }
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
            } else if (allPlayersDead()) {
                endGame(Phase::Defeat);
            }
        }

        bool allPlayersDead()
        {
            for (int slot = 0; slot < SLOTS; ++slot) {
                if (isUsedSlot(slot) && isPlayerAlive(slot)) {
                    return false;
                }
            }
            return true;
        }

        // Fin de partie : on retire les ennemis et leurs balles.
        void endGame(const Phase phase)
        {
            _phase = phase;
            _phaseTimer = config::RESTART_DELAY;
            _pending.clear();
            destroyAll(collectMonsters(_reg));
            destroyAll(collectProjectileEntities(_reg));
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
            _phase = restingPhase();
            _phaseTimer = 0.f;
            _elapsed = 0.f;
            _reg.get<CoinCounter>(_coinCounter).total = 0;
            for (int slot = 0; slot < SLOTS; ++slot) {
                if (isUsedSlot(slot)) {
                    spawnPlayer(slot);
                }
            }
        }

        void spawnPlayer(const int slot)
        {
            const Engine::Entity player = createPlayer(_reg, slot);

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
        Phase _phase = Phase::Playing;
        bool _lobby = false;
        bool _paused = false;
        MapId _map = 0;
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
    };
}
