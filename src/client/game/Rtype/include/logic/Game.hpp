//
// Created by antoine on 10/17/26.
//

#pragma once
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
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
        Game() : _rng(std::random_device{}()) {}

        explicit Game(const unsigned int seed) : _rng(seed) {}

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

        // Appele a chaque tick : mouvements, tirs, collisions, niveau...
        void update(const float dt)
        {
            if (!hasPlayers()) {
                idle();
            } else if (_phase == Phase::Playing) {
                updatePlaying(dt);
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

            status.phase = _phase;
            for (const Engine::Entity boss : collectWith<BossTag>(_reg)) {
                status.bossHp = std::max(0, _reg.get<Health>(boss).hp);
                status.bossMaxHp = config::BOSS_HP;
            }
            for (int slot = 0; slot < SLOTS; ++slot) {
                if (isUsedSlot(slot)) {
                    status.players.push_back(statusOf(slot));
                }
            }
            return status;
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

        Engine::Registry &registry()
        {
            return _reg;
        }

    private:
        static constexpr int SLOTS = static_cast<int>(config::MAX_PLAYERS);

        struct PendingSpawn {
            float delay;
            float y;
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
            if (_scroll > 0.f || _bossSpawned || _phase != Phase::Playing
                || !_pending.empty()) {
                reset();
            }
        }

        void updatePlaying(const float dt)
        {
            updateWeaponTimers(_reg, dt);
            lifetimeSystem(_reg, dt);
            playerControlSystem(_reg);
            advanceLevel(dt);
            bossSystem(_reg, dt);
            shootingSystem(_reg);
            movementSystem(_reg, dt);
            collisionSystem(_reg);
            contactSystem(_reg);
            deleteUnitsOutOfBounds(_reg);
            deathSystem(_reg);
            updatePhase();
        }

        // Partie finie : les joueurs volent encore, puis tout recommence.
        void updateEnded(const float dt)
        {
            lifetimeSystem(_reg, dt);
            playerControlSystem(_reg);
            movementSystem(_reg, dt);
            _phaseTimer -= dt;
            if (_phaseTimer <= 0.f) {
                reset();
            }
        }

        void advanceLevel(const float dt)
        {
            if (!_bossSpawned) {
                _scroll += config::SCROLL_SPEED * dt;
                queueDueWaves();
                if (_scroll >= config::BOSS_SPAWN_SCROLL) {
                    spawnBoss(_reg);
                    _bossSpawned = true;
                }
            }
            spawnPending(dt);
        }

        void queueDueWaves()
        {
            while (_nextWave < WAVES.size() && WAVES[_nextWave].at <= _scroll) {
                queueWave(WAVES[_nextWave]);
                ++_nextWave;
            }
        }

        void queueWave(const WaveStep &wave)
        {
            std::uniform_real_distribution<float> randomY(
                config::WAVE_MIN_Y, config::WAVE_MAX_Y);
            float y = wave.y;

            if (y < 0.f) {
                y = randomY(_rng);
            }
            for (int i = 0; i < wave.count; ++i) {
                const auto n = static_cast<float>(i);

                _pending.push_back({n * wave.interval, y + n * wave.dy});
            }
        }

        // Les monstres d'une vague arrivent les uns apres les autres.
        void spawnPending(const float dt)
        {
            std::vector<PendingSpawn> waiting;

            for (PendingSpawn &spawn : _pending) {
                spawn.delay -= dt;
                if (spawn.delay <= 0.f) {
                    spawnMonster(_reg, spawn.y);
                } else {
                    waiting.push_back(spawn);
                }
            }
            _pending = std::move(waiting);
        }

        void updatePhase()
        {
            if (_bossSpawned && collectWith<BossTag>(_reg).empty()) {
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
            _bossSpawned = false;
            _phase = Phase::Playing;
            _phaseTimer = 0.f;
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
        float _phaseTimer = 0.f;
        float _scroll = 0.f;
        std::size_t _nextWave = 0;
        bool _bossSpawned = false;
        std::vector<PendingSpawn> _pending;
        TrackedTable _tracked;
        std::vector<GameEvent> _events;
        std::uint32_t _nextNetId = 1;
        std::mt19937 _rng;
    };
}
