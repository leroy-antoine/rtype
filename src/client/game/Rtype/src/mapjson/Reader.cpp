#include "Reader.hpp"
#include "mapjson/Text.hpp"

#include <algorithm>
#include <sstream>
#include <utility>

namespace rtype::mapjson {
    namespace {
        std::string show(const double value)
        {
            std::ostringstream out;

            out << value;
            return out.str();
        }
    }

    Reader::Reader(const Json &json, std::string path)
        : _json(&json), _path(std::move(path))
    {
        if (!json.is_object()) {
            fail("expected an object");
        }
    }

    void Reader::fail(const std::string &message) const
    {
        throw MapError(_path + ": " + message);
    }

    void Reader::failKey(const std::string_view key,
        const std::string &message) const
    {
        throw MapError(keyPath(key) + ": " + message);
    }

    std::string Reader::keyPath(const std::string_view key) const
    {
        return _path + "." + std::string(key);
    }

    void Reader::allow(
        const std::initializer_list<std::string_view> keys) const
    {
        for (const auto &[key, value] : _json->items()) {
            if (std::ranges::find(keys, key) == keys.end()) {
                fail("unknown field \"" + key + "\"");
            }
        }
    }

    const Json *Reader::find(const std::string_view key) const
    {
        const auto found = _json->find(std::string(key));

        if (found == _json->end()) {
            return nullptr;
        }
        return &*found;
    }

    const Json &Reader::require(const std::string_view key) const
    {
        const Json *value = find(key);

        if (value == nullptr) {
            failKey(key, "missing field");
        }
        return *value;
    }

    bool Reader::has(const std::string_view key) const
    {
        return find(key) != nullptr;
    }

    bool Reader::isText(const std::string_view key) const
    {
        const Json *value = find(key);

        return value != nullptr && value->is_string();
    }

    double Reader::numeric(const std::string_view key, const Range range) const
    {
        const Json &json = require(key);

        if (!json.is_number()) {
            failKey(key, "expected a number");
        }
        const double value = json.get<double>();

        if (value < range.min || value > range.max) {
            failKey(key, show(value) + " is out of range ["
                + show(range.min) + ", " + show(range.max) + "]");
        }
        return value;
    }

    float Reader::number(const std::string_view key, const Range range) const
    {
        return static_cast<float>(numeric(key, range));
    }

    float Reader::numberOr(const std::string_view key, const Range range,
        const float fallback) const
    {
        if (!has(key)) {
            return fallback;
        }
        return number(key, range);
    }

    int Reader::integer(const std::string_view key, const Range range) const
    {
        if (!require(key).is_number_integer()) {
            failKey(key, "expected an integer");
        }
        return static_cast<int>(numeric(key, range));
    }

    std::string Reader::text(const std::string_view key) const
    {
        const Json &json = require(key);

        if (!json.is_string()) {
            failKey(key, "expected a string");
        }
        std::string word = json.get<std::string>();

        if (word.size() > MAX_TEXT) {
            failKey(key, "text longer than " + std::to_string(MAX_TEXT));
        }
        if (!isPrintableText(word)) {
            failKey(key, "control characters are not allowed");
        }
        return word;
    }

    std::string Reader::textOr(const std::string_view key,
        const std::string &fallback) const
    {
        if (!has(key)) {
            return fallback;
        }
        return text(key);
    }

    bool Reader::flagOr(const std::string_view key, const bool fallback) const
    {
        const Json *value = find(key);

        if (value == nullptr) {
            return fallback;
        }
        if (!value->is_boolean()) {
            failKey(key, "expected true or false");
        }
        return value->get<bool>();
    }

    std::vector<Reader> Reader::items(const std::string_view key,
        const std::size_t maxCount) const
    {
        const Json &json = require(key);
        std::vector<Reader> readers;

        if (!json.is_array()) {
            failKey(key, "expected a list");
        }
        if (json.size() > maxCount) {
            failKey(key, "more than " + std::to_string(maxCount) + " items");
        }
        readers.reserve(json.size());
        for (std::size_t i = 0; i < json.size(); ++i) {
            readers.emplace_back(json[i],
                keyPath(key) + "[" + std::to_string(i) + "]");
        }
        return readers;
    }

    std::vector<Reader> Reader::itemsOr(const std::string_view key,
        const std::size_t maxCount) const
    {
        if (!has(key)) {
            return {};
        }
        return items(key, maxCount);
    }
}
