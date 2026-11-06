#include <utility>
#include <variant>
#include "CustomMaps.hpp"

namespace server {
    CustomMaps::CustomMaps(std::filesystem::path recordsFile)
        : _recordsFile(std::move(recordsFile))
    {
        _board.load(_recordsFile);
    }

    bool CustomMaps::begin(const int slot,
        const rtype::protocol::Payload &payload)
    {
        const std::optional<std::uint32_t> size =
            rtype::protocol::decodeUploadBegin(payload);

        if (!size) {
            _upload.cancel(slot);
            return false;
        }
        return _upload.begin(slot, *size);
    }

    bool CustomMaps::add(const int slot,
        const rtype::protocol::Payload &chunk)
    {
        if (chunk.empty() || chunk.size() > rtype::protocol::MAX_UPLOAD_CHUNK) {
            _upload.cancel(slot);
            return false;
        }
        return _upload.add(slot, chunk);
    }

    void CustomMaps::cancel(const int slot)
    {
        _upload.cancel(slot);
    }

    std::variant<LoadedMap, std::string> CustomMaps::finish(const int slot)
    {
        const std::optional<std::string> text = _upload.finish(slot);

        if (!text) {
            return std::string("the map was not received completely");
        }
        rtype::mapjson::ParseResult parsed = rtype::mapjson::parseMap(*text);

        if (!parsed.map) {
            return std::move(parsed.error);
        }
        return LoadedMap{std::move(*parsed.map),
            rtype::mapjson::contentHash(*text)};
    }

    void CustomMaps::select(const LoadedMap &loaded)
    {
        _current = loaded;
    }

    bool CustomMaps::selected() const
    {
        return _current.has_value();
    }

    rtype::protocol::CustomInfo CustomMaps::info() const
    {
        if (!_current) {
            return {};
        }
        return {_current->map.name, _current->hash, _current->map.zones};
    }

    std::uint64_t CustomMaps::recordVictory(const rtype::RunRecord &record)
    {
        if (!_current) {
            return 0;
        }
        _board.add(_current->hash, record);
        _board.save(_recordsFile);
        return _current->hash;
    }

    rtype::protocol::CustomRecords CustomMaps::recordsOf(
        const std::uint64_t hash) const
    {
        return {hash, _board.records(hash)};
    }
}
