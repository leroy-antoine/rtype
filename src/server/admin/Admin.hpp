#pragma once

#include <ostream>
#include <string_view>
#include "engine/network/Server.hpp"
#include "room/Lobby.hpp"

namespace server {
    class Admin {
    public:
        Admin(Engine::Server &server, Lobby &lobby, std::ostream &out);

        bool run(std::string_view line);

    private:
        void help();
        void rooms();
        void players();
        void kick(Engine::ClientId client);
        void close(rtype::protocol::RoomId room);

        Engine::Server &_server;
        Lobby &_lobby;
        std::ostream &_out;
    };
}
