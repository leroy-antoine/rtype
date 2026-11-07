#include <iostream>
#include <optional>
#include "logic/Config.hpp"
#include "Lobby.hpp"

namespace server {

    Lobby::Lobby(Engine::Server &server)
        : _server(server)
    {
        _records.board.load(Records::FILE);
        _records.custom.load(Records::CUSTOM_FILE);
    }

    void Lobby::handle(const Engine::ServerEvents::Connected &event)
    {
        browse(event.client);
        std::cout << "client " << event.client << " connected" << std::endl;
    }

    void Lobby::handle(const Engine::ServerEvents::Disconnected &event)
    {
        leave(event.client, event.reason);
        _browsing.erase(event.client);
        std::cout << "client " << event.client << " disconnected" << std::endl;
    }

    void Lobby::handle(const Engine::ServerEvents::Message &event)
    {
        if (event.type == rtype::protocol::ROOM_CREATE) {
            create(event.client);
            return;
        }
        if (event.type == rtype::protocol::ROOM_JOIN) {
            if (const std::optional<rtype::protocol::RoomId> room = rtype::protocol::decodeRoomId(event.payload)) {
                join(event.client, *room);
            }
            return;
        }
        if (event.type == rtype::protocol::ROOM_LEAVE) {
            if (_members.contains(event.client)) {
                leave(event.client, Engine::DisconnectReason::Quit);
                browse(event.client);
            }
            return;
        }
        const auto member = _members.find(event.client);

        if (member != _members.end()) {
            _rooms.at(member->second).room->post(event);
        }
    }

    void Lobby::sync()
   {
       std::vector<rtype::protocol::RoomInfo> rooms = list();

       if (rooms == _sentList) {
           return;
       }
       const rtype::protocol::Payload payload = rtype::protocol::encode(rooms);

       for (const Engine::ClientId client : _browsing) {
           _server.send(client, rtype::protocol::ROOM_LIST, payload, Engine::Delivery::Reliable);
       }
       _sentList = std::move(rooms);
   }

    void Lobby::create(const Engine::ClientId client)
    {
        if (_members.contains(client)) {
            refuse(client, rtype::protocol::RoomRefusal::AlreadyInRoom);
            return;
        }
        if (_rooms.size() >= rtype::protocol::MAX_ROOMS) {
            refuse(client, rtype::protocol::RoomRefusal::TooManyRooms);
            return;
        }
        const rtype::protocol::RoomId id = _nextId++;

        _rooms[id].room = std::make_unique<Room>(id, _server, _records);
        std::cout << "room " << id << " created" << std::endl;
        enter(client, id);
    }

    void Lobby::join(const Engine::ClientId client, const rtype::protocol::RoomId room)
    {
        const auto found = _rooms.find(room);

        if (_members.contains(client)) {
            refuse(client, rtype::protocol::RoomRefusal::AlreadyInRoom);
        } else if (found == _rooms.end()) {
            refuse(client, rtype::protocol::RoomRefusal::NotFound);
        } else if (found->second.players >= rtype::config::MAX_PLAYERS) {
            refuse(client, rtype::protocol::RoomRefusal::Full);
        } else {
            enter(client, room);
        }
    }

    void Lobby::enter(const Engine::ClientId client, const rtype::protocol::RoomId room)
    {
        Entry &entry = _rooms.at(room);

        ++entry.players;
        _members[client] = room;
        _browsing.erase(client);
        _server.send(client, rtype::protocol::ROOM_JOINED, rtype::protocol::encodeRoomId(room),
            Engine::Delivery::Reliable);
        entry.room->post(Engine::ServerEvents::Connected{client});
    }

    void Lobby::leave(const Engine::ClientId client, const Engine::DisconnectReason reason)
    {
        const auto member = _members.find(client);

        if (member == _members.end()) {
            return;
        }
        const auto found = _rooms.find(member->second);
        _members.erase(member);
        if (found == _rooms.end()) {
            return;
        }
        found->second.room->post(Engine::ServerEvents::Disconnected{client, reason});
        if (--found->second.players == 0) {
            std::cout << "room " << found->first << " closed" << std::endl;
            _rooms.erase(found);
        }
    }

    void Lobby::browse(const Engine::ClientId client)
    {
        _browsing.insert(client);
        _server.send(client, rtype::protocol::ROOM_LIST, rtype::protocol::encode(list()),
            Engine::Delivery::Reliable);
    }

    void Lobby::refuse(const Engine::ClientId client, const rtype::protocol::RoomRefusal reason)
    {
        _server.send(client, rtype::protocol::ROOM_REFUSED, rtype::protocol::encode(reason),
            Engine::Delivery::Reliable);
    }

    std::vector<rtype::protocol::RoomInfo> Lobby::list() const
    {
        std::vector<rtype::protocol::RoomInfo> rooms;

        for (const auto &[id, entry] : _rooms) {
            rooms.push_back({id, entry.players, entry.room->phase()});
        }
        return rooms;
    }

}