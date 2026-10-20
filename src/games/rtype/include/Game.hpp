//
// Created by antoine on 10/17/26.
//

#pragma once
#include <cstddef>
#include <cstdint>
#include <optional>
#include <random>
#include <utility>
#include <vector>
#include "Components.hpp"
#include "Config.hpp"
#include "Events.hpp"
#include "Factories.hpp"
#include "Monsters.hpp"
#include "Queries.hpp"
#include "Replication.hpp"
#include "Systems.hpp"
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
            createPlayer(_reg, *slot);
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
        }

        bool respawnPlayer(const int slot)
        {
            if (!isUsedSlot(slot) || isPlayerAlive(slot)) {
                return false;
            }
            createPlayer(_reg, slot);
            return true;
        }

        bool isPlayerAlive(const int slot)
        {
            return findPlayerBySlot(_reg, slot).has_value();
        }

        void setInput(const int slot, const PlayerInput &input)
        {
            const std::optional<Engine::Entity> player =
                findPlayerBySlot(_reg, slot);

            if (player) {
                _reg.get<PlayerInput>(*player) = input;
            }
        }

        //Appelé à chaque tick du jeu en gros, ça checke toutes les entités et applique leurs mouvement etc
        void update(const float dt)
        {
            updateWeaponTimers(_reg, dt);
            lifetimeSystem(_reg, dt);
            playerControlSystem(_reg);
            updateSpawn(dt);
            shootingSystem(_reg);
            movementSystem(_reg, dt);
            collisionSystem(_reg);
            contactSystem(_reg);
            deleteUnitsOutOfBounds(_reg);
            deathSystem(_reg);
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

        Engine::Registry &registry()
        {
            return _reg;
        }

    private:
        [[nodiscard]] bool isUsedSlot(const int slot) const
        {
            const auto index = static_cast<std::size_t>(slot);

            return slot >= 0 && index < _usedSlots.size() && _usedSlots[index];
        }

        void updateSpawn(const float dt)
        {
            _spawnTimer -= dt;
            if (_spawnTimer > 0.f) {
                return;
            }
            spawnMonster(_reg, _spawnY(_rng));
            _spawnTimer = config::MONSTER_SPAWN_INTERVAL;
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
        float _spawnTimer = 0.f;
        TrackedTable _tracked;
        std::vector<GameEvent> _events;
        std::uint32_t _nextNetId = 1;
        std::mt19937 _rng;
        std::uniform_real_distribution<float> _spawnY{config::MAP_ORIGIN,
            config::MONSTER_MAX_SPAWN_Y};
    };
}
