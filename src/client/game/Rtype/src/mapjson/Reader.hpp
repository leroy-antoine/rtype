#pragma once

#include <array>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <nlohmann/json.hpp>
#include "mapjson/Limits.hpp"

namespace rtype::mapjson {
    using Json = nlohmann::json;

    // The text of a map is wrong. The message starts with the path of the
    // faulty field, e.g. `map.levels[2].waves[3].kind: unknown "Tonk"`.
    class MapError : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    template<typename E>
    struct Name {
        std::string_view name;
        E value;
    };

    template<typename E, std::size_t N>
    std::string namesOf(const std::array<Name<E>, N> &table)
    {
        std::string names;

        for (const Name<E> &entry : table) {
            if (!names.empty()) {
                names += ", ";
            }
            names += entry.name;
        }
        return names;
    }

    // Reads the fields of one JSON object. Every read checks the type and the
    // range, and throws a MapError that carries the path of the field.
    class Reader {
    public:
        Reader(const Json &json, std::string path);

        [[noreturn]] void fail(const std::string &message) const;
        [[noreturn]] void failKey(std::string_view key,
            const std::string &message) const;

        void allow(std::initializer_list<std::string_view> keys) const;
        [[nodiscard]] bool has(std::string_view key) const;
        [[nodiscard]] bool isText(std::string_view key) const;

        [[nodiscard]] float number(std::string_view key, Range range) const;
        [[nodiscard]] float numberOr(std::string_view key, Range range,
            float fallback) const;
        [[nodiscard]] int integer(std::string_view key, Range range) const;
        [[nodiscard]] std::string text(std::string_view key) const;
        [[nodiscard]] std::string textOr(std::string_view key,
            const std::string &fallback) const;
        [[nodiscard]] bool flagOr(std::string_view key, bool fallback) const;

        [[nodiscard]] std::vector<Reader> items(std::string_view key,
            std::size_t maxCount) const;
        [[nodiscard]] std::vector<Reader> itemsOr(std::string_view key,
            std::size_t maxCount) const;

        template<typename E, std::size_t N>
        [[nodiscard]] E choice(std::string_view key,
            const std::array<Name<E>, N> &table) const
        {
            const std::string word = text(key);

            for (const Name<E> &entry : table) {
                if (entry.name == word) {
                    return entry.value;
                }
            }
            failKey(key, "unknown \"" + word + "\" (expected: "
                + namesOf(table) + ")");
        }

        template<typename E, std::size_t N>
        [[nodiscard]] E choiceOr(std::string_view key,
            const std::array<Name<E>, N> &table, const E fallback) const
        {
            if (!has(key)) {
                return fallback;
            }
            return choice(key, table);
        }

    private:
        [[nodiscard]] const Json *find(std::string_view key) const;
        [[nodiscard]] const Json &require(std::string_view key) const;
        [[nodiscard]] double numeric(std::string_view key,
            Range range) const;
        [[nodiscard]] std::string keyPath(std::string_view key) const;

        const Json *_json;
        std::string _path;
    };
}
