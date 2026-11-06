#pragma once

#include <cstdint>
#include <deque>
#include <optional>
#include <unordered_map>
#include <vector>
#include "engine/core/Registry.hpp"
#include "client/LobbyMenu.hpp"
#include "client/Prediction.hpp"
#include "logic/Events.hpp"
#include "logic/Status.hpp"
#include "rtype/Protocol.hpp"
#include "utils/Vectors.hpp"

namespace client {
    enum class Cue { PlayerShot, Hit, Explosion, BossExplosion, BossAppears,
        CoinPickup, Purchase, PurchaseRefused };

    class Replica {
    public:
        void apply(const rtype::EntitySpawned &spawn);
        void apply(const rtype::EntityDestroyed &destroy);
        void apply(const rtype::protocol::StatesSnapshot &snapshot);
        void apply(const rtype::protocol::OwnState &state);
        void apply(const rtype::GameStatus &status);
        void apply(const rtype::AllRecords &records);
        void apply(const rtype::protocol::HistoryPage &page);
        void moveMapCursor(int delta);
        void apply(const rtype::protocol::OwnedAugment &augments);
        void apply(const std::vector<rtype::AugmentId> &shopOffers);
        void apply(const rtype::protocol::BuyResult &result);
        void assignSlot(int slot);
        std::uint32_t predict(const rtype::PlayerInput &input, float dt);
        void update(float dt);

        Engine::Registry &registry();
        [[nodiscard]] const std::optional<rtype::GameStatus> &status() const;
        [[nodiscard]] std::optional<int> slot() const;
        [[nodiscard]] const rtype::AllRecords &records() const;
        [[nodiscard]] const rtype::MapRecords &runs(rtype::MapId map) const;
        [[nodiscard]] rtype::MapId mapCursor() const;
        [[nodiscard]] LobbyMenu &menu();
        [[nodiscard]] const LobbyMenu &menu() const;
        [[nodiscard]] bool isHost() const;
        [[nodiscard]] bool onLeaderboard() const;
        [[nodiscard]] std::optional<rtype::SpriteId> bossSprite();
        [[nodiscard]] const Prediction &prediction() const;
        [[nodiscard]] const rtype::protocol::OwnedAugment &augments() const;
        [[nodiscard]] const std::vector<rtype::AugmentId> &shopOffers() const;
        std::vector<Cue> takeCues();
        std::optional<rtype::protocol::BuyResult> takeBuyResult();

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
        void refreshSolids();

        Engine::Registry _reg;
        std::unordered_map<std::uint32_t, Engine::Entity> _entities;
        std::unordered_map<std::uint32_t, std::deque<Sample>> _samples;
        std::optional<double> _latestTick;
        std::optional<double> _renderTick;
        Prediction _prediction;
        std::optional<rtype::GameStatus> _status;
        std::optional<int> _slot;
        rtype::AllRecords _records{};
        std::vector<rtype::RunEntry> _history;
        rtype::RunsByMap _runs{};
        rtype::MapId _mapCursor = 0;
        LobbyMenu _menu;
        rtype::protocol::OwnedAugment _augments{};
        std::vector<rtype::AugmentId> _shopOffers;
        std::optional<rtype::protocol::BuyResult> _buyResult;
        std::vector<Cue> _cues;
    };
}
