#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include "logic/Builder.hpp"
#include "logic/Game.hpp"
#include "mapjson/CustomMap.hpp"
#include "mapjson/Folder.hpp"
#include "mapjson/Limits.hpp"
#include "TestHarness.hpp"

using namespace rtype;

namespace {
    namespace fs = std::filesystem;

    const std::string VALID =
        R"({"name":"T","levels":[{"length":1000,)"
        R"("waves":[{"at":100,"kind":"Basic","count":3}],)"
        R"("bosses":[{"kind":"Standard","at":900}],)"
        R"("obstacles":[{"type":"Gate","at":300,"gapY":400,)"
        R"("gapHeight":300,"width":80}]}]})";

    std::string replaced(const std::string &from, const std::string &to)
    {
        std::string text = VALID;
        const std::size_t at = text.find(from);

        CHECK(at != std::string::npos);
        if (at != std::string::npos) {
            text.replace(at, from.size(), to);
        }
        return text;
    }

    bool failsWith(const std::string &text, const std::string &needle)
    {
        const std::string error = mapjson::parseMap(text).error;

        return error.find(needle) != std::string::npos;
    }

    std::string tinyLevel(const std::size_t waves, const int count)
    {
        std::string level = R"({"length":1000,"waves":[)";

        for (std::size_t i = 0; i < waves; ++i) {
            level += R"({"at":10,"kind":"Basic","count":)"
                + std::to_string(count) + "},";
        }
        level.back() = ']';
        return level + R"(,"bosses":[{"kind":"Standard","at":900}]})";
    }

    std::string mapOf(const std::string &level, const std::size_t times)
    {
        std::string text = R"({"name":"T","levels":[)";

        for (std::size_t i = 0; i < times; ++i) {
            text += level + ",";
        }
        text.back() = ']';
        return text + "}";
    }

    void write(const fs::path &file, const std::string &text)
    {
        std::ofstream(file, std::ios::binary) << text;
    }

    fs::path freshFolder(const std::string &name)
    {
        const fs::path folder = fs::temp_directory_path() / name;

        fs::remove_all(folder);
        return folder;
    }
}

TEST(a_valid_map_is_read)
{
    const mapjson::ParseResult result = mapjson::parseMap(VALID);

    CHECK(result.error.empty());
    CHECK(result.map.has_value());
    CHECK(result.map->name == "T");
    CHECK(result.map->levels.size() == 1);
    CHECK(result.map->zones[0] == Zone::DeepSpace);
}

TEST(the_elements_of_a_level_are_read)
{
    const LevelDef level = mapjson::parseMap(VALID).map->levels[0];

    CHECK(level.length == 1000.f);
    CHECK(level.waves.size() == 1);
    CHECK(level.waves[0].at == 100.f);
    CHECK(level.waves[0].kind == MonsterKind::Basic);
    CHECK(level.waves[0].count == 3);
    CHECK(level.waves[0].y == RANDOM_Y);
    CHECK(level.bosses[0].first == BossKind::Standard);
    CHECK(level.bosses[0].second == 900.f);
    CHECK(level.obstacles.size() == 2);
}

TEST(a_portal_covers_the_whole_height)
{
    const std::string text = replaced(R"({"type":"Gate","at":300,)"
        R"("gapY":400,"gapHeight":300,"width":80})",
        R"({"type":"FastPortal","at":300})");
    const mapjson::ParseResult result = mapjson::parseMap(text);

    CHECK(result.error.empty());
    CHECK(result.map->levels[0].obstacles.size() == 1);
    CHECK(result.map->levels[0].obstacles[0].kind == ObstacleKind::FastPortal);
    CHECK(result.map->levels[0].obstacles[0].height == config::MAP_HEIGHT);
}

TEST(the_elements_are_sorted_by_position)
{
    const std::string text = replaced(
        R"("waves":[{"at":100,"kind":"Basic","count":3}])",
        R"("waves":[{"at":500,"kind":"Tank","count":1},)"
        R"({"at":100,"kind":"Basic","count":3}])");
    const LevelDef level = mapjson::parseMap(text).map->levels[0];

    CHECK(level.waves.size() == 2);
    CHECK(level.waves[0].at == 100.f);
    CHECK(level.waves[1].at == 500.f);
}

TEST(a_map_is_played_by_the_game)
{
    const mapjson::CustomMap map = *mapjson::parseMap(VALID).map;
    Game game(1, buildRun(map.levels));

    game.addPlayer();
    for (int tick = 0; tick < 60 * 30; ++tick) {
        game.update(config::FIXED_DT);
    }
    CHECK(buildRun(map.levels).levels.size() == 1);
}

TEST(a_text_that_is_not_a_map_is_refused)
{
    CHECK(failsWith("", "map"));
    CHECK(failsWith("{oops", "map"));
    CHECK(failsWith("[]", "expected an object"));
    CHECK(failsWith("{}", "map.name: missing field"));
    CHECK(failsWith(std::string(100, '[') + std::string(100, ']'),
        "nested deeper"));
}

TEST(a_wrong_value_says_where_it_is)
{
    CHECK(failsWith(replaced("Basic", "Tonk"),
        "map.levels[0].waves[0].kind: unknown \"Tonk\""));
    CHECK(failsWith(replaced(R"("count":3)", R"("count":0)"),
        "waves[0].count: 0 is out of range"));
    CHECK(failsWith(replaced(R"("count":3)", R"("count":51)"),
        "waves[0].count"));
    CHECK(failsWith(replaced(R"("count":3)", R"("count":2.5)"),
        "expected an integer"));
    CHECK(failsWith(replaced(R"("at":100)", R"("at":1001)"),
        "waves[0].at: 1001 is out of range"));
    CHECK(failsWith(replaced(R"("at":100,)", R"("at":100,"cout":1,)"),
        "unknown field \"cout\""));
}

TEST(the_wave_must_stay_on_screen)
{
    CHECK(failsWith(replaced(R"("count":3)", R"("count":3,"y":"middle")"),
        "waves[0].y"));
    CHECK(failsWith(replaced(R"("count":3)", R"("count":3,"y":5000)"),
        "waves[0].y: 5000 is out of range"));
    CHECK(failsWith(replaced(R"("count":3)",
        R"("count":3,"y":700,"dy":200)"), "outside of the screen"));
}

TEST(a_gate_must_let_the_ship_pass)
{
    CHECK(failsWith(replaced(R"("gapHeight":300)", R"("gapHeight":50)"),
        "obstacles[0].gapHeight"));
    CHECK(failsWith(replaced(R"("gapY":400)", R"("gapY":900)"),
        "the gap goes below the map"));
    CHECK(failsWith(replaced(R"("width":80)", R"("width":80,"amplitude":50)"),
        "needs a period"));
    CHECK(failsWith(replaced(R"("width":80)", R"("width":80,"wall":"Mine")"),
        "unknown \"Mine\""));
}

TEST(the_whole_map_is_checked)
{
    const std::string noBoss = replaced(
        R"("bosses":[{"kind":"Standard","at":900}],)", "");

    CHECK(failsWith(noBoss, "needs at least one boss"));
    CHECK(failsWith(R"({"name":"T","levels":[]})", "at least one level"));
    CHECK(failsWith(replaced(R"("name":"T")", R"("name":"")"),
        "must not be empty"));
    CHECK(failsWith(replaced(R"({"name")", R"({"version":2,"name")"),
        "map.version"));
    CHECK(failsWith(replaced("\"T\"", "\"a\\u0001\""), "control characters"));
}

TEST(the_technical_caps_protect_the_server)
{
    const std::string huge = R"({"name":"T","pad":")"
        + std::string(mapjson::MAX_FILE_BYTES, 'x') + R"("})";

    CHECK(failsWith(huge, "file bigger than"));
    CHECK(failsWith(mapOf(tinyLevel(1, 1), mapjson::MAX_LEVELS + 1),
        "map.levels: more than"));
    CHECK(failsWith(mapOf(tinyLevel(300, 50), 2), "monsters in total"));
    CHECK(mapjson::parseMap(mapOf(tinyLevel(300, 50), 1)).error.empty());
}

TEST(the_hash_follows_the_content)
{
    CHECK(mapjson::contentHash(VALID) == mapjson::contentHash(VALID));
    CHECK(mapjson::contentHash(VALID) != mapjson::contentHash(VALID + " "));
}

TEST(the_example_map_is_valid)
{
    const std::vector<mapjson::MapEntry> entries =
        mapjson::scanMaps(CUSTOM_MAPS_DIR);
    const auto example = std::ranges::find(entries, "example.json",
        &mapjson::MapEntry::file);

    CHECK(example != entries.end());
    CHECK(example->error.empty());
    CHECK(example->map.has_value());
    CHECK(example->map->levels.size() == 2);
}

TEST(the_folder_is_scanned)
{
    const fs::path folder = freshFolder("rtype_mapjson_scan");

    fs::create_directories(folder);
    write(folder / "b.json", VALID);
    write(folder / "a.JSON", "{oops");
    write(folder / "notes.txt", VALID);
    const std::vector<mapjson::MapEntry> entries = mapjson::scanMaps(folder);

    CHECK(entries.size() == 2);
    CHECK(entries[0].file == "a.JSON");
    CHECK(!entries[0].error.empty());
    CHECK(!entries[0].map.has_value());
    CHECK(entries[1].file == "b.json");
    CHECK(entries[1].map.has_value());
    CHECK(entries[1].hash == mapjson::contentHash(VALID));
    fs::remove_all(folder);
}

TEST(a_missing_folder_is_created)
{
    const fs::path folder = freshFolder("rtype_mapjson_missing");

    CHECK(mapjson::scanMaps(folder).empty());
    CHECK(fs::is_directory(folder));
    fs::remove_all(folder);
}

TEST(a_big_file_is_refused_without_being_read)
{
    const fs::path folder = freshFolder("rtype_mapjson_big");

    fs::create_directories(folder);
    write(folder / "big.json", std::string(mapjson::MAX_FILE_BYTES + 1, ' '));
    const std::vector<mapjson::MapEntry> entries = mapjson::scanMaps(folder);

    CHECK(entries.size() == 1);
    CHECK(entries[0].error.find("bigger than") != std::string::npos);
    fs::remove_all(folder);
}
