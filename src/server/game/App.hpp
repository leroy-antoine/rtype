#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>
#include "engine/core/FixedTimestep.hpp"
#include "engine/network/Event.hpp"
#include "engine/network/Server.hpp"
#include "CustomMaps.hpp"
#include "logic/Config.hpp"
#include "logic/Game.hpp"
#include "logic/Leaderboard.hpp"
#include "rtype/Protocol.hpp"

namespace server {
    class App {
    public:
        explicit App(std::uint16_t port);

        int run();

    private:
        using Clock = std::chrono::steady_clock;

        // Le statut (HUD) est envoye 10 fois par seconde, pas a chaque tick.
        static constexpr std::size_t STATUS_EVERY_TICKS = 6;
        static constexpr const char *LEADERBOARD_FILE = "leaderboard.txt";
        static constexpr const char *CUSTOM_LEADERBOARD_FILE =
            "custom_leaderboard.txt";

        void tick();
        void receive();
        void broadcastEvents();
        void broadcastStates();
        void broadcastStatus();
        void recordVictory();
        void recordCustomVictory(const rtype::RunRecord &record);
        void selectMap(int slot, const rtype::protocol::Payload &payload);
        void leaveMatch(int slot);
        void pauseMatch(int slot, const rtype::protocol::Payload &payload);
        void sendOwnStates();
        void sendWorld(Engine::ClientId client);
        void sleep() const;
        [[nodiscard]] std::optional<int> slotOf(Engine::ClientId client) const;

        void handle(const Engine::ServerEvents::Connected &event);
        void handle(const Engine::ServerEvents::Disconnected &event);
        void handle(const Engine::ServerEvents::Message &event);

        // The maps written by the players (see docs/custom-maps.md).
        bool handleCustom(const Engine::ServerEvents::Message &event,
            int slot);
        void uploadBegin(Engine::ClientId client, int slot,
            const rtype::protocol::Payload &payload);
        void uploadEnd(Engine::ClientId client, int slot);
        void startCustom(Engine::ClientId client, const LoadedMap &loaded);
        void sendUploadResult(Engine::ClientId client,
            const rtype::protocol::UploadResult &result);
        void sendCustomRecords(Engine::ClientId client,
            const rtype::protocol::Payload &payload);

        void syncAugments();
        void syncShop();
        void handleBuy(Engine::ClientId client, int slot, const rtype::protocol::Payload &payload);
        [[nodiscard]] rtype::protocol::OwnedAugment ownedAugments() const;

        Engine::Server _server;
        rtype::Game _game;
        Engine::FixedTimestep _timestep{rtype::config::FIXED_DT};
        std::unordered_map<Engine::ClientId, int> _slots;
        std::unordered_map<Engine::ClientId, std::uint32_t> _lastInputs;
        std::size_t _ticks = 0;
        rtype::Leaderboard _leaderboard;
        CustomMaps _custom{CUSTOM_LEADERBOARD_FILE};
        rtype::Phase _lastPhase = rtype::Phase::Lobby;
        rtype::protocol::OwnedAugment _sentAugments{};
        std::vector<rtype::AugmentId> _sentOffers;
    };
}
