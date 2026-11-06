#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include "mapjson/Limits.hpp"

namespace rtype {
    // Rebuilds the JSON that a client sends in pieces. Only one upload at a
    // time, owned by one player (identified by its slot). Any mistake drops
    // the upload, so a client cannot make the server keep more than
    // MAX_FILE_BYTES.
    class MapUpload {
    public:
        // Starts an upload and forgets the previous one.
        bool begin(const int owner, const std::size_t size)
        {
            cancel(owner);
            if (size == 0 || size > mapjson::MAX_FILE_BYTES) {
                return false;
            }
            _owner = owner;
            _size = size;
            _text.clear();
            _text.reserve(size);
            return true;
        }

        bool add(const int owner, const std::vector<std::uint8_t> &chunk)
        {
            if (!_owner || *_owner != owner
                || _text.size() + chunk.size() > _size) {
                cancel(owner);
                return false;
            }
            _text.append(chunk.begin(), chunk.end());
            return true;
        }

        // The text, once every byte has arrived. The upload is over.
        std::optional<std::string> finish(const int owner)
        {
            std::optional<std::string> text;

            if (_owner && *_owner == owner && _text.size() == _size) {
                text = std::move(_text);
            }
            cancel(owner);
            return text;
        }

        void cancel(const int owner)
        {
            if (_owner && *_owner == owner) {
                _owner.reset();
                _text.clear();
                _text.shrink_to_fit();
            }
        }

        [[nodiscard]] bool active() const
        {
            return _owner.has_value();
        }

    private:
        std::optional<int> _owner;
        std::size_t _size = 0;
        std::string _text;
    };
}
