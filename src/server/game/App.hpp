#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include "engine/network/Event.hpp"
#include "engine/network/Server.hpp"
#include "logic/Config.hpp"
#include "rtype/Protocol.hpp"
#include "room/Lobby.hpp"

namespace server {
    class App {
    public:
        explicit App(std::uint16_t port);

        int run();

    private:
        static constexpr std::size_t MAX_CLIENTS = rtype::protocol::MAX_ROOMS * rtype::config::MAX_PLAYERS;
        static constexpr std::chrono::milliseconds POLL_DELAY{1};

        void receive();

        Engine::Server _server;
        Lobby _lobby;
    };
}