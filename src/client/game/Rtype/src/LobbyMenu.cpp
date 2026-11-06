#include <algorithm>
#include <utility>
#include "client/LobbyMenu.hpp"

namespace client {
    namespace {
        bool pressedKey(const std::vector<Engine::Key> &pressed,
            const Engine::Key key)
        {
            return std::ranges::find(pressed, key) != pressed.end();
        }
    }

    LobbyMenu::LobbyMenu(std::filesystem::path folder)
        : _folder(std::move(folder))
    {
        rescan();
    }

    void LobbyMenu::rescan()
    {
        _entries = rtype::mapjson::scanMaps(_folder);
        _cursor = std::min(_cursor, _entries.size());
        if (_cursor > 0 && _cursor == _entries.size()) {
            --_cursor;
        }
    }

    void LobbyMenu::navigate(const std::vector<Engine::Key> &pressed)
    {
        if (pressedKey(pressed, Engine::Key::Tab)) {
            switchTab();
        }
        if (pressedKey(pressed, Engine::Key::Right)) {
            openRecords();
        }
        if (pressedKey(pressed, Engine::Key::Left)
            && _tab == Tab::Records) {
            _tab = Tab::Custom;
        }
        if (pressedKey(pressed, Engine::Key::R) && _tab == Tab::Custom) {
            rescan();
        }
    }

    void LobbyMenu::switchTab()
    {
        _message.clear();
        if (_tab == Tab::Maps) {
            rescan();
            _tab = Tab::Custom;
            return;
        }
        _tab = Tab::Maps;
    }

    // The records are those of a valid map: the server knows no other.
    void LobbyMenu::openRecords()
    {
        if (_tab == Tab::Custom && selected() != nullptr) {
            _tab = Tab::Records;
            _recordsWanted = true;
        }
    }

    void LobbyMenu::moveCursor(const int delta)
    {
        const auto count = static_cast<int>(_entries.size());

        if (count == 0 || delta == 0) {
            return;
        }
        _cursor = static_cast<std::size_t>(
            (static_cast<int>(_cursor) + delta + count) % count);
        _message.clear();
        _recordsWanted = _tab == Tab::Records && selected() != nullptr;
    }

    std::string LobbyMenu::readSelected(std::string &text) const
    {
        if (_cursor >= _entries.size()) {
            return "NO MAP SELECTED";
        }
        if (selected() == nullptr) {
            return "THIS MAP IS NOT VALID";
        }
        return rtype::mapjson::readMapFile(
            _folder / _entries[_cursor].file, text);
    }

    std::optional<std::uint64_t> LobbyMenu::takeRecordsRequest()
    {
        if (!_recordsWanted || selected() == nullptr) {
            _recordsWanted = false;
            return std::nullopt;
        }
        _recordsWanted = false;
        return _entries[_cursor].hash;
    }

    void LobbyMenu::setMessage(std::string message)
    {
        _message = std::move(message);
    }

    Tab LobbyMenu::tab() const
    {
        return _tab;
    }

    std::size_t LobbyMenu::cursor() const
    {
        return _cursor;
    }

    const std::string &LobbyMenu::message() const
    {
        return _message;
    }

    const std::vector<rtype::mapjson::MapEntry> &LobbyMenu::entries() const
    {
        return _entries;
    }

    const rtype::mapjson::MapEntry *LobbyMenu::selected() const
    {
        if (_cursor >= _entries.size() || !_entries[_cursor].map) {
            return nullptr;
        }
        return &_entries[_cursor];
    }
}
