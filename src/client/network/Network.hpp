#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include "engine/network/Client.hpp"
#include "engine/network/Event.hpp"
#include "logic/Augments.hpp"
#include "logic/Components.hpp"
#include "client/Replica.hpp"

namespace client {
    class Network {
    public:
        Network(const std::string &host, std::uint16_t port);

        bool update(Replica &replica);
        void sendInput(std::uint32_t sequence, const rtype::PlayerInput &input);
        void requestRespawn();
        void buy(rtype::AugmentId id);
        void requestReady(bool ready);
        void flush();
        [[nodiscard]] bool connected() const;

    private:
        static void assignSlot(Replica &replica, const std::optional<int> &slot);
        void handle(Replica &replica, const Engine::ClientEvents::Connected &event);
        void handle(Replica &replica, const Engine::ClientEvents::Refused &event);
        void handle(Replica &replica, const Engine::ClientEvents::Unreachable &event);
        void handle(Replica &replica, const Engine::ClientEvents::Disconnected &event);
        void handle(Replica &replica, const Engine::ClientEvents::Message &event);

        Engine::Client _client;
        bool _open = true;
    };
}
