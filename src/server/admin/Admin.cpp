#include <algorithm>
#include <string>
#include <variant>
#include "Admin.hpp"
#include "Command.hpp"

namespace server {
    namespace {
        std::string phaseName(const rtype::Phase phase)
        {
            switch (phase) {
                case rtype::Phase::Playing: return "playing";
                case rtype::Phase::Victory: return "victory";
                case rtype::Phase::Defeat: return "defeat";
                case rtype::Phase::Lobby: return "waiting";
                case rtype::Phase::Shop: return "shop";
            }
            return "unknown";
        }

        std::string addressText(const std::optional<Engine::Address> &address)
        {
            if (!address) {
                return "?";
            }
            const std::uint32_t ip = address->ip;

            return std::to_string(ip >> 24 & 0xFF) + "." + std::to_string(ip >> 16 & 0xFF) + "."
                + std::to_string(ip >> 8 & 0xFF) + "." + std::to_string(ip & 0xFF) + ":"
                + std::to_string(address->port);
        }
    }

    Admin::Admin(Engine::Server &server, Lobby &lobby, std::ostream &out)
        : _server(server), _lobby(lobby), _out(out)
    {
    }

    bool Admin::run(const std::string_view line)
    {
        const std::variant<Command, std::string> parsed = parseCommand(line);

        if (const std::string *error = std::get_if<std::string>(&parsed)) {
            _out << *error << std::endl;
            return true;
        }
        const Command &command = std::get<Command>(parsed);

        switch (command.kind) {
            case Command::Kind::Help: help(); break;
            case Command::Kind::Rooms: rooms(); break;
            case Command::Kind::Players: players(); break;
            case Command::Kind::Kick: kick(command.target); break;
            case Command::Kind::Close: close(static_cast<rtype::protocol::RoomId>(command.target)); break;
            case Command::Kind::Quit:
                _out << "stopping the server" << std::endl;
                return false;
        }
        return true;
    }

    void Admin::help()
    {
        _out << "/rooms           list the rooms\n"
             << "/players         list the connected clients\n"
             << "/kick <client>   disconnect a client\n"
             << "/close <room>    close a room, its players go back to the room list\n"
             << "/quit            stop the server\n"
             << "/help            show this help" << std::endl;
    }

    void Admin::rooms()
    {
        const std::vector<rtype::protocol::RoomInfo> rooms = _lobby.list();

        if (rooms.empty()) {
            _out << "no room" << std::endl;
            return;
        }
        for (const rtype::protocol::RoomInfo &room : rooms) {
            _out << "room " << room.id << "  " << static_cast<int>(room.players) << "/"
                 << rtype::config::MAX_PLAYERS << " players  " << phaseName(room.phase) << std::endl;
        }
    }

    void Admin::players()
    {
        const std::vector<PlayerInfo> players = _lobby.players();

        if (players.empty()) {
            _out << "no client" << std::endl;
            return;
        }
        for (const PlayerInfo &player : players) {
            const std::optional<std::chrono::milliseconds> ping = _server.latency(player.client);

            _out << "client " << player.client << "  " << addressText(_server.address(player.client)) << "  "
                 << (player.room ? "room " + std::to_string(*player.room) : std::string("browsing")) << "  "
                 << (ping ? std::to_string(ping->count()) + " ms" : std::string("? ms")) << std::endl;
        }
    }

    void Admin::kick(const Engine::ClientId client)
    {
        const std::vector<PlayerInfo> players = _lobby.players();

        if (std::ranges::none_of(players, [client](const PlayerInfo &p) { return p.client == client; })) {
            _out << "no client " << client << std::endl;
            return;
        }
        _server.disconnect(client);
        _out << "client " << client << " kicked" << std::endl;
    }

    void Admin::close(const rtype::protocol::RoomId room)
    {
        if (!_lobby.close(room)) {
            _out << "no room " << room << std::endl;
        }
    }
}
