#include <algorithm>
#include <utility>
#include "client/CustomMapList.hpp"

namespace client {
    CustomMapList::CustomMapList(std::filesystem::path folder)
        : _folder(std::move(folder))
    {
        rescan();
    }

    void CustomMapList::rescan()
    {
        _entries = rtype::mapjson::scanMaps(_folder);
        _cursor = std::min(_cursor, _entries.size());
        if (_cursor > 0 && _cursor == _entries.size()) {
            --_cursor;
        }
    }

    void CustomMapList::moveCursor(const int delta)
    {
        const auto count = static_cast<int>(_entries.size());

        if (count == 0 || delta == 0) {
            return;
        }
        _cursor = static_cast<std::size_t>(
            (static_cast<int>(_cursor) + delta + count) % count);
        _message.clear();
    }

    std::string CustomMapList::readSelected(std::string &text) const
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

    void CustomMapList::setMessage(std::string message)
    {
        _message = std::move(message);
    }

    std::size_t CustomMapList::cursor() const
    {
        return _cursor;
    }

    const std::string &CustomMapList::message() const
    {
        return _message;
    }

    const std::vector<rtype::mapjson::MapEntry> &
        CustomMapList::entries() const
    {
        return _entries;
    }

    const rtype::mapjson::MapEntry *CustomMapList::selected() const
    {
        if (_cursor >= _entries.size() || !_entries[_cursor].map) {
            return nullptr;
        }
        return &_entries[_cursor];
    }
}
