#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <cstddef>
#include <vector>
#include "engine/network/Client.hpp"
#include "engine/network/Event.hpp"
#include "logic/Augments.hpp"
#include "logic/Components.hpp"
#include "logic/Settings.hpp"
#include "client/Replica.hpp"
#include "rtype/Protocol.hpp"

namespace client {
    class Network {
    public:
        Network(const std::string &host, std::uint16_t port);

        bool update(Replica &replica);
        void sendInput(std::uint32_t sequence, const rtype::PlayerInput &input);
        void requestRespawn();
        void selectMap(rtype::MapId map);
        void leaveMatch();
        void setPause(bool paused);
        void buy(rtype::AugmentId id);
        void requestReady(bool ready);
        void requestHistory();
        void changeSettings(const rtype::Settings &settings);
        void createRoom();
        void joinRoom(rtype::protocol::RoomId room);
        void leaveRoom();

        // Sends the JSON of a map in pieces (a few at each update), then asks
        // the server to play it.
        void uploadMap(std::string text);
        void requestCustomRecords(std::uint64_t hash);
        [[nodiscard]] bool uploading() const;
        void flush();
        [[nodiscard]] bool connected() const;
        [[nodiscard]] const std::optional<rtype::protocol::RoomId> &room() const;
        [[nodiscard]] const std::vector<rtype::protocol::RoomInfo> &rooms() const;
        std::optional<rtype::protocol::RoomRefusal> takeRefusal();

    private:
        // Pieces sent at each update: enough to be quick, few enough not to
        // overfill the reliable channel.
        static constexpr std::size_t CHUNKS_PER_UPDATE = 8;

        struct Upload {
            std::string text;
            std::size_t sent = 0;
            bool begun = false;
        };

        void pumpUpload();
        void sendChunks(Upload &upload);
        static void assignSlot(Replica &replica, const std::optional<int> &slot);
        void handle(Replica &replica, const Engine::ClientEvents::Connected &event);
        void handle(Replica &replica, const Engine::ClientEvents::Refused &event);
        void handle(Replica &replica, const Engine::ClientEvents::Unreachable &event);
        void handle(Replica &replica, const Engine::ClientEvents::Disconnected &event);
        void handle(Replica &replica, const Engine::ClientEvents::Message &event);
        bool handleRoom(Replica &replica, const Engine::ClientEvents::Message &event);

        Engine::Client _client;
        bool _open = true;
        std::optional<Upload> _upload;
        std::optional<rtype::protocol::RoomId> _room;
        std::vector<rtype::protocol::RoomInfo> _rooms;
        std::optional<rtype::protocol::RoomRefusal> _refusal;
    };
}
