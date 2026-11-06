#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "logic/Builder.hpp"
#include "logic/CustomLeaderboard.hpp"
#include "logic/Game.hpp"
#include "mapjson/CustomMap.hpp"
#include "rtype/CustomMapProtocol.hpp"
#include "rtype/MapUpload.hpp"
#include "TestHarness.hpp"

using namespace rtype;

namespace {
    using Bytes = std::vector<std::uint8_t>;

    const std::string MAP =
        R"({"name":"Up","levels":[{"length":1000,)"
        R"("bosses":[{"kind":"Standard","at":900}]}]})";

    Bytes bytes(const std::string &text)
    {
        return Bytes(text.begin(), text.end());
    }

    Bytes truncated(Bytes payload)
    {
        payload.pop_back();
        return payload;
    }

    std::string uploaded(MapUpload &upload, const std::string &text)
    {
        upload.begin(0, text.size());
        upload.add(0, bytes(text.substr(0, 10)));
        upload.add(0, bytes(text.substr(10)));
        return upload.finish(0).value_or("");
    }
}

TEST(an_upload_in_pieces_gives_back_the_text)
{
    MapUpload upload;

    CHECK(uploaded(upload, MAP) == MAP);
    CHECK(!upload.active());
}

TEST(an_upload_that_overflows_or_is_incomplete_is_dropped)
{
    MapUpload upload;

    CHECK(upload.begin(0, 5));
    CHECK(!upload.add(0, bytes("too long")));
    CHECK(!upload.active());
    CHECK(upload.begin(0, 5));
    CHECK(upload.add(0, bytes("ab")));
    CHECK(!upload.finish(0).has_value());
    CHECK(!upload.active());
}

TEST(only_the_owner_can_continue_an_upload)
{
    MapUpload upload;

    CHECK(upload.begin(1, 4));
    CHECK(!upload.add(2, bytes("ab")));
    CHECK(upload.active());
    upload.cancel(2);
    CHECK(upload.active());
    upload.cancel(1);
    CHECK(!upload.active());
}

TEST(an_upload_bigger_than_the_cap_is_refused)
{
    MapUpload upload;

    CHECK(!upload.begin(0, 0));
    CHECK(!upload.begin(0, mapjson::MAX_FILE_BYTES + 1));
    CHECK(upload.begin(0, mapjson::MAX_FILE_BYTES));
}

TEST(the_upload_messages_are_validated)
{
    CHECK(protocol::decodeUploadBegin(protocol::encodeUploadBegin(100))
        == 100u);
    CHECK(!protocol::decodeUploadBegin(protocol::encodeUploadBegin(0)));
    CHECK(!protocol::decodeUploadBegin(protocol::encodeUploadBegin(
        protocol::MAX_UPLOAD_SIZE + 1)));
    CHECK(!protocol::decodeUploadBegin({1, 2}));
}

TEST(the_custom_info_survives_a_round_trip)
{
    const protocol::CustomInfo info{"My map", 0xFEEDULL,
        {Zone::Nebula, Zone::Core}};
    const auto decoded = protocol::decodeCustomInfo(protocol::encode(info));

    CHECK(decoded.has_value());
    CHECK(decoded->name == "My map");
    CHECK(decoded->hash == 0xFEEDULL);
    CHECK(decoded->zones == info.zones);
}

TEST(a_bad_custom_info_is_rejected)
{
    const Bytes good = protocol::encode(protocol::CustomInfo{"A", 0, {}});
    Bytes badZone = protocol::encode(protocol::CustomInfo{"A", 0,
        {Zone::Core}});
    Bytes control = protocol::encode(protocol::CustomInfo{"A", 0, {}});

    badZone.back() = static_cast<std::uint8_t>(Zone::Count);
    control[1] = 1;
    CHECK(protocol::decodeCustomInfo(good).has_value());
    CHECK(!protocol::decodeCustomInfo(badZone));
    CHECK(!protocol::decodeCustomInfo(control));
    CHECK(!protocol::decodeCustomInfo(truncated(good)));
    CHECK(!protocol::decodeCustomInfo({}));
    CHECK(!protocol::decodeCustomInfo(protocol::encode(
        protocol::CustomInfo{std::string(41, 'x'), 0, {}})));
}

TEST(an_upload_result_survives_a_round_trip)
{
    const auto bad = protocol::decodeUploadResult(
        protocol::encode(protocol::UploadResult{false, "map: oops"}));
    const auto good = protocol::decodeUploadResult(
        protocol::encode(protocol::UploadResult{true, ""}));

    CHECK(bad.has_value() && !bad->ok && bad->error == "map: oops");
    CHECK(good.has_value() && good->ok);
    CHECK(!protocol::decodeUploadResult({2, 0}));
    CHECK(!protocol::decodeUploadResult({}));
}

TEST(the_records_of_a_custom_map_survive_a_round_trip)
{
    protocol::CustomRecords records;
    const std::size_t normal = static_cast<std::size_t>(Difficulty::Normal);
    const std::size_t brutal = static_cast<std::size_t>(Difficulty::Brutal);

    records.hash = 0x0123456789ABCDEFULL;
    records.records[normal] = {{1000, 1, {Difficulty::Normal, false}}};
    records.records[brutal] = {{2000, 4, {Difficulty::Brutal, true}}};
    const auto decoded = protocol::decodeCustomRecords(
        protocol::encode(records));

    CHECK(decoded.has_value());
    CHECK(decoded->hash == records.hash);
    CHECK(decoded->records[normal].size() == 1);
    CHECK(decoded->records[brutal][0].timeMs == 2000);
    CHECK(decoded->records[brutal][0].settings.friendlyFire);
    CHECK(protocol::decodeCustomRecordsGet(
        protocol::encodeCustomRecordsGet(records.hash)) == records.hash);
    CHECK(!protocol::decodeCustomRecordsGet({1, 2, 3}));
    CHECK(!protocol::decodeCustomRecords(truncated(
        protocol::encode(records))));
}

TEST(a_record_of_the_wrong_difficulty_block_is_rejected)
{
    protocol::CustomRecords records;

    records.records[0] = {{1000, 1, {Difficulty::Brutal, false}}};
    CHECK(!protocol::decodeCustomRecords(protocol::encode(records)));
}

TEST(the_records_of_a_custom_map_are_sorted_and_capped_per_difficulty)
{
    CustomLeaderboard board;
    const std::size_t normal = static_cast<std::size_t>(Difficulty::Normal);

    for (unsigned int i = 0; i < 8; ++i) {
        board.add(7, {9000 - i * 100, 1, {Difficulty::Normal, false}});
    }
    board.add(7, {5000, 1, {Difficulty::Hard, false}});
    CHECK(board.records(7)[normal].size() == MAX_RECORDS);
    CHECK(board.records(7)[normal].front().timeMs == 8300);
    CHECK(board.records(7)[static_cast<std::size_t>(Difficulty::Hard)]
        .size() == 1);
    CHECK(board.records(8)[normal].empty());
}

TEST(the_number_of_custom_maps_in_the_records_is_capped)
{
    CustomLeaderboard board;

    for (std::uint64_t hash = 0; hash < CustomLeaderboard::MAX_MAPS; ++hash) {
        CHECK(board.add(hash, {1000, 1, {}}));
    }
    CHECK(!board.add(CustomLeaderboard::MAX_MAPS, {1000, 1, {}}));
    CHECK(board.add(0, {900, 1, {}}));
}

TEST(the_records_of_custom_maps_are_saved_and_loaded)
{
    const std::filesystem::path file =
        std::filesystem::temp_directory_path() / "rtype_custom_records.txt";
    const std::size_t normal = static_cast<std::size_t>(Difficulty::Normal);
    CustomLeaderboard board;
    CustomLeaderboard loaded;

    board.add(42, {5000, 2, {Difficulty::Normal, true}});
    board.add(42, {4000, 1, {Difficulty::Normal, false}});
    CHECK(board.save(file));
    std::ofstream(file, std::ios::app) << "43 9000 0 1 0\n43 9000 9 1 0\n"
        << "43 9000 1 9 0\nnot a line\n";
    loaded.load(file);
    CHECK(loaded.records(42)[normal].size() == 2);
    CHECK(loaded.records(42)[normal].front().timeMs == 4000);
    CHECK(loaded.records(42)[normal][1].settings.friendlyFire);
    CHECK(loaded.records(43)[normal].empty());
    std::filesystem::remove(file);
}

TEST(a_custom_map_is_started_with_its_zones)
{
    const mapjson::CustomMap map = *mapjson::parseMap(MAP).map;
    Game game(1);

    game.openLobby();
    CHECK(game.startMap(buildRun(map.levels), CUSTOM_MAP, {Zone::Nebula}));
    CHECK(game.map() == CUSTOM_MAP);
    CHECK(game.phase() == Phase::Playing);
    CHECK(customZone({Zone::Nebula}, 1) == Zone::Nebula);
    CHECK(customZone({Zone::Nebula}, 2) == Zone::DeepSpace);
}

TEST(an_error_that_quotes_the_map_can_always_be_sent)
{
    const auto result = protocol::decodeUploadResult(protocol::encode(
        protocol::UploadResult{false, "unknown field \"\x01x\""}));

    CHECK(result.has_value());
    CHECK(result->error == "unknown field \"?x\"");
}

TEST(a_full_upload_never_fills_the_reliable_channel)
{
    const std::size_t pieces = (protocol::MAX_UPLOAD_SIZE
        + protocol::MAX_UPLOAD_CHUNK - 1) / protocol::MAX_UPLOAD_CHUNK;

    CHECK(pieces + 2 < Engine::Protocol::MAX_PENDING);
    CHECK(protocol::MAX_UPLOAD_CHUNK <= Engine::Protocol::MAX_PAYLOAD_SIZE);
}
