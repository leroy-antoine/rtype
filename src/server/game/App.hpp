#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <unordered_map>
#include "engine/core/FixedTimestep.hpp"
#include "engine/network/Event.hpp"
#include "engine/network/Server.hpp"
#include "Config.hpp"
#include "Game.hpp"

namespace server {
    // Colle entre le reseau et rtype::Game: un client = un slot de joueur.
    class App {
    public:
        explicit App(std::uint16_t port);

        int run();

    private:
        using Clock = std::chrono::steady_clock;

        void tick();
        void receive();
        void broadcastEvents();
        void broadcastStates();
        void sendWorld(Engine::ClientId client);
        void sleep() const;
        [[nodiscard]] std::optional<int> slotOf(Engine::ClientId client) const;

        void handle(const Engine::ServerEvents::Connected &event);
        void handle(const Engine::ServerEvents::Disconnected &event);
        void handle(const Engine::ServerEvents::Message &event);

        Engine::Server _server;
        rtype::Game _game;
        Engine::FixedTimestep _timestep{rtype::config::FIXED_DT};
        std::unordered_map<Engine::ClientId, int> _slots;
    };
}
