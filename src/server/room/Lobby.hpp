 #pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>
#include "engine/network/Event.hpp"
#include "engine/network/Server.hpp"
#include "rtype/Protocol.hpp"
#include "Room.hpp"

namespace server {
    class Lobby {
        public:

        explicit Lobby(Engine::Server &server);

        void handle(const Engine::ServerEvents::Connected &event);
        void handle(const Engine::ServerEvents::Disconnected &event);
        void handle(const Engine::ServerEvents::Message &event);
        void sync();

        private:

        struct Entry {
            std::unique_ptr<Room> room;
            std::uint8_t players = 0;
        };

        void create(Engine::ClientId client);
        void join(Engine::ClientId client, rtype::protocol::RoomId room);
        void enter(Engine::ClientId client, rtype::protocol::RoomId room);
        void leave(Engine::ClientId client, Engine::DisconnectReason reason);
        void browse(Engine::ClientId client);
        void refuse(Engine::ClientId client, rtype::protocol::RoomRefusal reason);
        [[nodiscard]] std::vector<rtype::protocol::RoomInfo> list() const;

        Engine::Server &_server;
        Records _records;
        std::map<rtype::protocol::RoomId, Entry> _rooms;
        std::unordered_map<Engine::ClientId, rtype::protocol::RoomId> _members;
        std::set<Engine::ClientId> _browsing;
        std::vector<rtype::protocol::RoomInfo> _sentList;
        rtype::protocol::RoomId _nextId = 1;
    };
}