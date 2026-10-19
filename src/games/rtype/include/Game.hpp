//
// Created by antoine on 10/17/26.
//

#pragma once
#include <cstddef>
#include <optional>
#include <random>
#include "Components.hpp"
#include "Config.hpp"
#include "Factories.hpp"
#include "Monsters.hpp"
#include "Queries.hpp"
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
            if (slot >= 0 && static_cast<std::size_t>(slot) < _usedSlots.size()) {
                _usedSlots[static_cast<std::size_t>(slot)] = false;
            }
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
        }

        Engine::Registry &registry()
        {
            return _reg;
        }

    private:
        void updateSpawn(const float dt)
        {
            _spawnTimer -= dt;
            if (_spawnTimer > 0.f) {
                return;
            }
            spawnMonster(_reg, _spawnY(_rng));
            _spawnTimer = config::MONSTER_SPAWN_INTERVAL;
        }

        Engine::Registry _reg;
        SlotTable _usedSlots{};
        float _spawnTimer = 0.f;
        std::mt19937 _rng;
        std::uniform_real_distribution<float> _spawnY{config::MAP_ORIGIN,
            MONSTER_MAX_SPAWN_Y};
    };
}
