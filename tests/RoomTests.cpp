#include <cstdint>
#include <optional>
#include <vector>
#include "rtype/Protocol.hpp"
#include "TestHarness.hpp"

using namespace rtype;

TEST(a_room_id_survives_an_encode_decode_round_trip)
{
    const std::optional<protocol::RoomId> decoded =
        protocol::decodeRoomId(protocol::encodeRoomId(513));

    CHECK(decoded.has_value());
    CHECK(*decoded == 513);
}

TEST(a_room_id_with_a_wrong_size_is_rejected)
{
    CHECK(!protocol::decodeRoomId({}).has_value());
    CHECK(!protocol::decodeRoomId({1}).has_value());
    CHECK(!protocol::decodeRoomId({1, 2, 3}).has_value());
}

TEST(a_room_list_survives_an_encode_decode_round_trip)
{
    const std::vector<protocol::RoomInfo> rooms = {
        {1, 2, Phase::Lobby}, {7, 4, Phase::Playing}, {300, 1, Phase::Shop}};
    const std::optional<std::vector<protocol::RoomInfo>> decoded =
        protocol::decodeRoomList(protocol::encode(rooms));

    CHECK(decoded.has_value());
    CHECK(*decoded == rooms);
}

TEST(an_empty_room_list_is_valid)
{
    const std::optional<std::vector<protocol::RoomInfo>> decoded =
        protocol::decodeRoomList(protocol::encode(std::vector<protocol::RoomInfo>{}));

    CHECK(decoded.has_value());
    CHECK(decoded->empty());
}

TEST(a_room_list_is_clamped_to_max_rooms)
{
    const std::vector<protocol::RoomInfo> rooms(protocol::MAX_ROOMS + 3, protocol::RoomInfo{});
    const std::optional<std::vector<protocol::RoomInfo>> decoded =
        protocol::decodeRoomList(protocol::encode(rooms));

    CHECK(decoded.has_value());
    CHECK(decoded->size() == protocol::MAX_ROOMS);
}

TEST(a_bad_room_list_is_rejected)
{
    const std::uint8_t tooMany = static_cast<std::uint8_t>(protocol::MAX_ROOMS + 1);
    const std::uint8_t tooManyPlayers = static_cast<std::uint8_t>(config::MAX_PLAYERS + 1);
    const std::uint8_t badPhase = static_cast<std::uint8_t>(Phase::Shop) + 1;

    CHECK(!protocol::decodeRoomList({}).has_value());
    CHECK(!protocol::decodeRoomList({tooMany}).has_value());
    CHECK(!protocol::decodeRoomList({1, 0, 1, 2}).has_value());
    CHECK(!protocol::decodeRoomList({1, 0, 1, 2, 0, 9}).has_value());
    CHECK(!protocol::decodeRoomList({1, 0, 1, tooManyPlayers, 0}).has_value());
    CHECK(!protocol::decodeRoomList({1, 0, 1, 2, badPhase}).has_value());
}

TEST(a_room_refusal_survives_an_encode_decode_round_trip)
{
    const std::optional<protocol::RoomRefusal> decoded =
        protocol::decodeRoomRefusal(protocol::encode(protocol::RoomRefusal::Full));

    CHECK(decoded.has_value());
    CHECK(*decoded == protocol::RoomRefusal::Full);
}

TEST(a_bad_room_refusal_is_rejected)
{
    const std::uint8_t unknown = static_cast<std::uint8_t>(protocol::RoomRefusal::AlreadyInRoom) + 1;

    CHECK(!protocol::decodeRoomRefusal({}).has_value());
    CHECK(!protocol::decodeRoomRefusal({unknown}).has_value());
    CHECK(!protocol::decodeRoomRefusal({0, 0}).has_value());
}
