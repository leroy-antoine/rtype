#include <exception>
#include <utility>
#include "mapjson/CustomMap.hpp"
#include "Parsers.hpp"

namespace rtype::mapjson {
    namespace {
        constexpr int FORMAT_VERSION = 1;

        bool tooDeep(const int depth, const Json::parse_event_t, Json &)
        {
            if (depth > MAX_DEPTH) {
                throw MapError("map: nested deeper than "
                    + std::to_string(MAX_DEPTH) + " levels");
            }
            return true;
        }

        // The version is optional. Only the version 1 exists for now.
        void checkVersion(const Reader &map)
        {
            if (map.has("version")) {
                (void)map.integer("version", {FORMAT_VERSION, FORMAT_VERSION});
            }
        }

        std::string readName(const Reader &map)
        {
            std::string name = map.text("name");

            if (name.empty()) {
                map.failKey("name", "must not be empty");
            }
            return name;
        }

        void readLevels(const Reader &map, CustomMap &result)
        {
            const std::vector<Reader> items = map.items("levels", MAX_LEVELS);

            if (items.empty()) {
                map.failKey("levels", "a map needs at least one level");
            }
            for (const Reader &item : items) {
                ParsedLevel parsed = parseLevel(item);

                result.levels.push_back(std::move(parsed.level));
                result.zones.push_back(parsed.zone);
            }
        }

        CustomMap readMap(const Json &json)
        {
            const Reader map(json, "map");
            CustomMap result;

            map.allow({"name", "author", "version", "levels"});
            checkVersion(map);
            result.name = readName(map);
            result.author = map.textOr("author", "");
            readLevels(map, result);
            checkTotals(result.levels);
            return result;
        }
    }

    Json parseJson(const std::string_view text)
    {
        if (text.size() > MAX_FILE_BYTES) {
            throw MapError("map: file bigger than "
                + std::to_string(MAX_FILE_BYTES / 1024) + " KiB");
        }
        try {
            return Json::parse(text.begin(), text.end(), tooDeep);
        } catch (const Json::parse_error &error) {
            throw MapError("map: not valid JSON, error near byte "
                + std::to_string(error.byte));
        }
    }

    ParseResult parseMap(const std::string_view text)
    {
        ParseResult result;

        try {
            result.map = readMap(parseJson(text));
        } catch (const MapError &error) {
            result.error = error.what();
        } catch (const std::exception &error) {
            result.error = std::string("map: ") + error.what();
        }
        return result;
    }

    std::uint64_t contentHash(const std::string_view text)
    {
        constexpr std::uint64_t OFFSET = 14695981039346656037ULL;
        constexpr std::uint64_t PRIME = 1099511628211ULL;
        std::uint64_t hash = OFFSET;

        for (const char c : text) {
            hash ^= static_cast<unsigned char>(c);
            hash *= PRIME;
        }
        return hash;
    }
}
