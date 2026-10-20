#pragma once

#include <cstdint>
#include <string>
#include "engine/network/Client.hpp"
#include "engine/network/Event.hpp"
#include "Components.hpp"
#include "game/Replica.hpp"

namespace client {
    class Network {
    public:
        Network(const std::string &host, std::uint16_t port);

        bool update(Replica &replica);
        void sendInput(const rtype::PlayerInput &input);
        void requestRespawn();
        void flush();

    private:
        void handle(Replica &replica, const Engine::ClientEvents::Connected &event);
        void handle(Replica &replica, const Engine::ClientEvents::Refused &event);
        void handle(Replica &replica, const Engine::ClientEvents::Unreachable &event);
        void handle(Replica &replica, const Engine::ClientEvents::Disconnected &event);
        void handle(Replica &replica, const Engine::ClientEvents::Message &event);

        Engine::Client _client;
        bool _open = true;
    };
}
