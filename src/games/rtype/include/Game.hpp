//
// Created by antoine on 10/17/26.
//

#pragma once
#include <optional>
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
        Game() = default;
        //mettre le random ici pour la randomness des spawns en y pour les mobs volants

        std::optional<int> addPlayer()
        {
            const std::optional<int> slot = findFreeSlot(_reg);

            if (!slot) {
                return std::nullopt;
            }
            createPlayer(_reg, *slot);
            return slot;
        }

        void removePlayer(const int slot)
        {
            const std::optional<Engine::Entity> player =
                findPlayerBySlot(_reg, slot);

            if (player) {
                _reg.destroy(*player);
            }
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
            shootingSystem(_reg);
            updateSpawn(dt);
            movementSystem(_reg, dt);
            collisionSystem(_reg);
            deleteUnitsOutOfBounds(_reg);
            deathSystem(_reg);
        }

    private:
        void updateSpawn(const float dt)
        {
            _spawnTimer -= dt;
            if (_spawnTimer > 0.f) {
                return;
            }
            spawnMonster(_reg, 0.f);
            _spawnTimer = config::MONSTER_SPAWN_INTERVAL;
        }

        Engine::Registry _reg;
        float _spawnTimer = 0.f;
    };
}
