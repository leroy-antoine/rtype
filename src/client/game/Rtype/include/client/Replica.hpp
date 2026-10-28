#pragma once

#include <cstdint>
#include <deque>
#include <optional>
#include <unordered_map>
#include <vector>
#include "engine/core/Registry.hpp"
#include "client/Prediction.hpp"
#include "logic/Events.hpp"
#include "logic/Status.hpp"
#include "rtype/Protocol.hpp"
#include "utils/Vectors.hpp"

namespace client {
    enum class Cue { PlayerShot, Hit, Explosion, BossExplosion, BossAppears,
        CoinPickup };

    class Replica {
    public:
        void apply(const rtype::EntitySpawned &spawn);
        void apply(const rtype::EntityDestroyed &destroy);
        void apply(const rtype::protocol::StatesSnapshot &snapshot);
        void apply(const rtype::protocol::OwnState &state);
        void apply(const rtype::GameStatus &status);
        void assignSlot(int slot);
        std::uint32_t predict(const rtype::PlayerInput &input, float dt);
        void update(float dt);

        Engine::Registry &registry();
        [[nodiscard]] const std::optional<rtype::GameStatus> &status() const;
        [[nodiscard]] std::optional<int> slot() const;
        std::vector<Cue> takeCues();

    private:
        static constexpr double INTERPOLATION_DELAY = 3.0;
        static constexpr double MAX_CLOCK_ERROR = 10.0;
        static constexpr double CLOCK_CORRECTION_RATE = 5.0;

        struct Sample {
            double tick;
            gameLogic::Vec2Pos pos;
        };

        void record(double tick, const rtype::EntityState &state);
        void advanceClock(float dt);
        void interpolate();
        void interpolate(Engine::Entity e, std::deque<Sample> &samples);
        void applyPrediction();
        std::optional<Engine::Entity> localPlayer();
        void recordSpawnCue(const rtype::EntitySpawned &spawn);

        Engine::Registry _reg;
        std::unordered_map<std::uint32_t, Engine::Entity> _entities;
        std::unordered_map<std::uint32_t, std::deque<Sample>> _samples;
        std::optional<double> _latestTick;
        std::optional<double> _renderTick;
        Prediction _prediction;
        std::optional<rtype::GameStatus> _status;
        std::optional<int> _slot;
        std::vector<Cue> _cues;
    };
}
