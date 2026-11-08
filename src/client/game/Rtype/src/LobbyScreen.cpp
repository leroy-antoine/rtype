#include <algorithm>
#include "client/LobbyScreen.hpp"

namespace client {
    namespace {
        using Engine::Color;

        constexpr Color WHITE{255, 255, 255, 255};
        constexpr Color GREY{110, 110, 120, 255};
        constexpr Color RED{235, 70, 60, 255};
        constexpr Color YELLOW{255, 220, 40, 255};
        constexpr Color ROW{25, 25, 45, 255};
        constexpr Color ROW_SELECTED{55, 55, 95, 255};

        std::string phaseName(const rtype::Phase phase)
        {
            switch (phase) {
                case rtype::Phase::Lobby: return "WAITING";
                case rtype::Phase::Shop: return "SHOP";
                default: return "IN GAME";
            }
        }

        std::string refusalText(const rtype::protocol::RoomRefusal reason)
        {
            switch (reason) {
                case rtype::protocol::RoomRefusal::NotFound: return "ROOM NOT FOUND";
                case rtype::protocol::RoomRefusal::Full: return "ROOM FULL";
                case rtype::protocol::RoomRefusal::TooManyRooms: return "TOO MANY ROOMS";
                case rtype::protocol::RoomRefusal::AlreadyInRoom: return "ALREADY IN A ROOM";
            }
            return "REFUSED";
        }
    }

    LobbyScreen::LobbyScreen(Engine::Renderer &renderer, const float screenWidth, const float screenHeight)
        : _renderer(renderer), _font(renderer), _width(screenWidth), _height(screenHeight)
    {
    }

    std::optional<LobbyAction> LobbyScreen::handle(const std::vector<Engine::Key> &keys,
        const std::vector<rtype::protocol::RoomInfo> &rooms)
    {
        _selected = selection(rooms.size());
        for (const Engine::Key key : keys) {
            if ((key == Engine::Key::Up || key == Engine::Key::Z) && _selected > 0) {
                --_selected;
            } else if ((key == Engine::Key::Down || key == Engine::Key::S) && _selected + 1 < rooms.size()) {
                ++_selected;
            } else if (key == Engine::Key::C) {
                return LobbyAction{LobbyAction::Kind::Create, 0};
            } else if (key == Engine::Key::Enter && !rooms.empty()) {
                return LobbyAction{LobbyAction::Kind::Join, rooms[_selected].id};
            }
        }
        return std::nullopt;
    }

    void LobbyScreen::showRefusal(const rtype::protocol::RoomRefusal reason, const float elapsed)
    {
        _message = refusalText(reason);
        _messageUntil = elapsed + MESSAGE_DURATION;
    }

    std::size_t LobbyScreen::selection(const std::size_t count) const
    {
        if (count == 0) {
            return 0;
        }
        return std::min(_selected, count - 1);
    }

    void LobbyScreen::draw(const std::vector<rtype::protocol::RoomInfo> &rooms, const float elapsed)
    {
        const std::size_t selected = selection(rooms.size());
        float top = LIST_TOP;

        _font.drawCentered("ROOMS", _width / 2.f, 60.f, 6.f, WHITE);
        if (rooms.empty()) {
            _font.drawCentered("NO ROOM YET", _width / 2.f, LIST_TOP, 3.f, GREY);
        }
        for (std::size_t i = 0; i < rooms.size(); ++i) {
            drawRow(rooms[i], top, i == selected);
            top += ROW_HEIGHT + ROW_GAP;
        }
        _font.drawCentered("UP/DOWN: CHOOSE   ENTER: JOIN   C: CREATE   ESC: QUIT", _width / 2.f,
            _height - 80.f, 2.f, GREY);
        if (!_message.empty() && elapsed <= _messageUntil) {
            _font.drawCentered(_message, _width / 2.f, _height - 130.f, 3.f, RED);
        }
    }

    void LobbyScreen::drawRow(const rtype::protocol::RoomInfo &room, const float top, const bool selected)
    {
        const float left = (_width - ROW_WIDTH) / 2.f;
        const float textY = top + (ROW_HEIGHT - Font::height(3.f)) / 2.f;
        const Color text = selected ? YELLOW : WHITE;
        const std::string players = std::to_string(room.players) + "/"
            + std::to_string(rtype::config::MAX_PLAYERS);

        _renderer.drawRect({left, top}, {ROW_WIDTH, ROW_HEIGHT}, selected ? ROW_SELECTED : ROW);
        _font.draw("ROOM " + std::to_string(room.id), {left + 24.f, textY}, 3.f, text);
        _font.draw(players, {left + 300.f, textY}, 3.f, text);
        _font.draw(phaseName(room.phase), {left + 440.f, textY}, 3.f, text);
    }
}
