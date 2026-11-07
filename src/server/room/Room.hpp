#pragma once

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <optional>
#include <stop_token>
#include <thread>
#include <unordered_map>
#include <vector>
#include "engine/core/FixedTimestep.hpp"
#include "engine/network/Event.hpp"
#include "engine/network/Server.hpp"
#include "game/CustomMaps.hpp"
#include "logic/Config.hpp"
#include "logic/CustomLeaderboard.hpp"
#include "logic/Game.hpp"
#include "logic/Leaderboard.hpp"
#include "rtype/Protocol.hpp"


namespace server {
    struct Records {
        static constexpr const char *FILE = "leaderboard.txt";
        static constexpr const char *CUSTOM_FILE = "custom_leaderboard.txt";
        std::mutex mutex;
        rtype::Leaderboard board;
        rtype::CustomLeaderboard custom;
    };

    class Room {
        public:
            Room(rtype::protocol::RoomId id, Engine::Server &server, Records &records);
            Room(const Room &) = delete;
            Room &operator=(const Room &) = delete;

            void post(Engine::ServerEvent event);
            [[nodiscard]] rtype::protocol::RoomId id() const;
            [[nodiscard]] rtype::Phase phase() const;

        private:
            using Clock = std::chrono::steady_clock;

            static constexpr std::size_t STATUS_EVERY_TICKS = 6;

            void run(const std::stop_token &stop);
            void tick();
            void receive();
            void sendToMembers(Engine::MessageType type, const rtype::protocol::Payload &payload,
                Engine::Delivery delivery = Engine::Delivery::Unreliable);
            void broadcastEvents();
            void broadcastStates();
            void broadcastStatus();
            void recordVictory();
            void recordCustomVictory(const rtype::RunRecord &record);
            void sendHistory(Engine::ClientId client);
            void changeSettings(int slot, const rtype::protocol::Payload &payload);
            void sendLeaderboard(std::optional<Engine::ClientId> client);
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

            bool handleCustom(const Engine::ServerEvents::Message &event, int slot);
            void uploadBegin(Engine::ClientId client, int slot, const rtype::protocol::Payload &payload);
            void uploadEnd(Engine::ClientId client, int slot);
            void startCustom(Engine::ClientId client, const LoadedMap &loaded);
            void sendUploadResult(Engine::ClientId client, const rtype::protocol::UploadResult &result);
            void sendCustomRecords(Engine::ClientId client, const rtype::protocol::Payload &payload);

            void syncAugments();
            void syncShop();
            void handleBuy(Engine::ClientId client, int slot, const rtype::protocol::Payload &payload);
            [[nodiscard]] rtype::protocol::OwnedAugment ownedAugments() const;

            rtype::protocol::RoomId _id;
            Engine::Server &_server;
            Records &_records;
            rtype::Game _game;
            CustomMaps _custom;
            Engine::FixedTimestep _timestep{rtype::config::FIXED_DT};
            std::unordered_map<Engine::ClientId, int> _slots;
            std::unordered_map<Engine::ClientId, std::uint32_t> _lastInputs;
            std::size_t _ticks = 0;
            rtype::Phase _lastPhase = rtype::Phase::Lobby;
            rtype::protocol::OwnedAugment _sentAugments{};
            std::vector<rtype::AugmentId> _sentOffers;
            std::atomic<rtype::Phase> _phase{rtype::Phase::Lobby};
            std::mutex _inboxMutex;
            std::vector<Engine::ServerEvent> _inbox;
            std::jthread _thread;
    };
}