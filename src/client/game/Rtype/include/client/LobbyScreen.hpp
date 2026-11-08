#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>
#include "engine/window/Event.hpp"
#include "engine/window/Renderer.hpp"
#include "Font.hpp"
#include "rtype/Protocol.hpp"

namespace client {
    struct LobbyAction {
        enum class Kind { Create, Join };

        Kind kind = Kind::Create;
        rtype::protocol::RoomId room = 0;
    };

    class LobbyScreen {
    public:
        LobbyScreen(Engine::Renderer &renderer, float screenWidth, float screenHeight);

        std::optional<LobbyAction> handle(const std::vector<Engine::Key> &keys,
            const std::vector<rtype::protocol::RoomInfo> &rooms);
        void showRefusal(rtype::protocol::RoomRefusal reason, float elapsed);
        void showClosed(float elapsed);
        void draw(const std::vector<rtype::protocol::RoomInfo> &rooms, float elapsed);

    private:
        static constexpr float ROW_WIDTH = 640.f;
        static constexpr float ROW_HEIGHT = 48.f;
        static constexpr float ROW_GAP = 8.f;
        static constexpr float LIST_TOP = 160.f;
        static constexpr float MESSAGE_DURATION = 2.f;

        [[nodiscard]] std::size_t selection(std::size_t count) const;
        void drawRow(const rtype::protocol::RoomInfo &room, float top, bool selected);

        Engine::Renderer &_renderer;
        Font _font;
        float _width;
        float _height;
        std::size_t _selected = 0;
        std::string _message;
        float _messageUntil = 0.f;
    };
}
