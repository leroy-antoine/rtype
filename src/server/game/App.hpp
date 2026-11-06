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

        void tick();
        void receive();
        void broadcastEvents();
        void broadcastStates();
        void broadcastStatus();
        void recordVictory();
        void selectMap(int slot, const rtype::protocol::Payload &payload);
        void sendHistory(Engine::ClientId client);
        void changeSettings(int slot,
            const rtype::protocol::Payload &payload);
        void leaveMatch(int slot);
        void pauseMatch(int slot, const rtype::protocol::Payload &payload);
        void sendOwnStates();
        void sendWorld(Engine::ClientId client);
        void sleep() const;
        [[nodiscard]] std::optional<int> slotOf(Engine::ClientId client) const;

        void handle(const Engine::ServerEvents::Connected &event);
        void handle(const Engine::ServerEvents::Disconnected &event);
        void handle(const Engine::ServerEvents::Message &event);

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
        rtype::Phase _lastPhase = rtype::Phase::Lobby;
        rtype::protocol::OwnedAugment _sentAugments{};
        std::vector<rtype::AugmentId> _sentOffers;
    };
}
