#pragma once

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>
#include "engine/core/Registry.hpp"
#include "logic/Events.hpp"
#include "logic/Status.hpp"

namespace client {
    // Ce qui vient de se passer dans le monde et qui merite un son.
    enum class Cue { PlayerShot, Hit, Explosion, BossExplosion, BossAppears,
        CoinPickup };

    class Replica {
    public:
        void apply(const rtype::EntitySpawned &spawn);
        void apply(const rtype::EntityDestroyed &destroy);
        void apply(const std::vector<rtype::EntityState> &states);
        void apply(const rtype::GameStatus &status);
        void assignSlot(int slot);
        void update(float dt);

        Engine::Registry &registry();
        [[nodiscard]] const std::optional<rtype::GameStatus> &status() const;
        [[nodiscard]] std::optional<int> slot() const;
        std::vector<Cue> takeCues();

    private:
        void apply(const rtype::EntityState &state);
        void recordSpawnCue(const rtype::EntitySpawned &spawn);

        Engine::Registry _reg;
        std::unordered_map<std::uint32_t, Engine::Entity> _entities;
        std::optional<rtype::GameStatus> _status;
        std::optional<int> _slot;
        std::vector<Cue> _cues;
    };
}
