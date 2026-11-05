#include <cstdint>
#include <optional>
#include <vector>
#include "rtype/Protocol.hpp"
#include "TestHarness.hpp"

using namespace rtype;

namespace {
    GameStatus sampleStatus()
    {
        GameStatus status;

        status.phase = Phase::Victory;
        status.bossHp = 640;
        status.bossMaxHp = 1000;
        status.coins = 42;
        status.elapsedMs = 83456;
        status.level = 7;
        status.players.push_back({0, 100, true});
        status.players.push_back({3, 0, false});
        return status;
    }
}

TEST(a_status_survives_an_encode_decode_round_trip)
{
    const GameStatus status = sampleStatus();
    const std::optional<GameStatus> decoded =
        protocol::decodeStatus(protocol::encode(status));

    CHECK(decoded.has_value());
    CHECK(decoded->phase == Phase::Victory);
    CHECK(decoded->bossHp == 640);
    CHECK(decoded->bossMaxHp == 1000);
    CHECK(decoded->coins == 42);
    CHECK(decoded->elapsedMs == 83456);
    CHECK(decoded->level == 7);
    CHECK(decoded->players.size() == 2);
    CHECK(decoded->players[0].slot == 0);
    CHECK(decoded->players[0].hp == 100);
    CHECK(decoded->players[0].alive);
    CHECK(decoded->players[1].slot == 3);
    CHECK(!decoded->players[1].alive);
}

TEST(an_empty_status_is_valid)
{
    const std::optional<GameStatus> decoded =
        protocol::decodeStatus(protocol::encode(GameStatus{}));

    CHECK(decoded.has_value());
    CHECK(decoded->players.empty());
}

TEST(a_truncated_or_padded_status_is_rejected)
{
    protocol::Payload payload = protocol::encode(sampleStatus());

    payload.push_back(0);
    CHECK(!protocol::decodeStatus(payload).has_value());
    payload.pop_back();
    payload.pop_back();
    CHECK(!protocol::decodeStatus(payload).has_value());
    CHECK(!protocol::decodeStatus({}).has_value());
}

TEST(a_status_with_invalid_fields_is_rejected)
{
    protocol::Payload payload = protocol::encode(sampleStatus());

    payload[0] = 9;
    CHECK(!protocol::decodeStatus(payload).has_value());
    payload = protocol::encode(sampleStatus());
    payload[14] = 5;
    CHECK(!protocol::decodeStatus(payload).has_value());
    payload = protocol::encode(sampleStatus());
    payload[17] = 5;
    CHECK(!protocol::decodeStatus(payload).has_value());
    payload = protocol::encode(sampleStatus());
    payload[protocol::STATUS_HEADER_SIZE] = 7;
    CHECK(!protocol::decodeStatus(payload).has_value());
    payload = protocol::encode(sampleStatus());
    payload[protocol::STATUS_HEADER_SIZE + 3] = 2;
    CHECK(!protocol::decodeStatus(payload).has_value());
}

TEST(a_boss_hp_above_its_maximum_is_rejected)
{
    GameStatus status = sampleStatus();

    status.bossHp = 1001;
    CHECK(!protocol::decodeStatus(protocol::encode(status)).has_value());
}

TEST(every_sprite_id_survives_a_spawn_round_trip)
{
    for (int id = 0; id < static_cast<int>(SpriteId::Count); ++id) {
        EntitySpawned spawn;

        spawn.id = 42;
        spawn.sprite = static_cast<SpriteId>(id);
        spawn.pos = gameLogic::Vec2Pos{{10.f, 20.f}};
        spawn.box = gameLogic::Vec2Hitbox{{30.f, 40.f}};
        const std::optional<EntitySpawned> decoded =
            protocol::decodeSpawn(protocol::encode(spawn));

        CHECK(decoded.has_value());
        CHECK(decoded->sprite == spawn.sprite);
    }
}

TEST(an_unknown_sprite_id_is_rejected)
{
    EntitySpawned spawn;
    protocol::Payload payload = protocol::encode(spawn);

    payload[4] = static_cast<std::uint8_t>(SpriteId::Count);
    CHECK(!protocol::decodeSpawn(payload).has_value());
}

TEST(inputs_round_trip)
{
    const PlayerInput input{true, false, true, false, true};
    const std::optional<protocol::InputMessage> decoded =
        protocol::decodeInput(protocol::encodeInput(7, input));

    CHECK(decoded.has_value());
    CHECK(decoded->sequence == 7);
    CHECK(decoded->input.up && !decoded->input.down);
    CHECK(decoded->input.left && !decoded->input.right);
    CHECK(decoded->input.fire);
    CHECK(!protocol::decodeInput({0xFF}).has_value());
}

TEST(the_assigned_slot_round_trips_and_is_validated)
{
    for (int slot = 0; slot < static_cast<int>(config::MAX_PLAYERS); ++slot) {
        CHECK(protocol::decodeYou(protocol::encodeYou(slot)) == slot);
    }
    CHECK(!protocol::decodeYou({}).has_value());
    CHECK(!protocol::decodeYou({0, 0}).has_value());
    CHECK(!protocol::decodeYou({static_cast<std::uint8_t>(
        config::MAX_PLAYERS)}).has_value());
}

TEST(the_protocol_version_changed_with_the_new_messages)
{
    CHECK(protocol::VERSION >= 7);
}

TEST(owned_augments_survive_an_encode_decode_round_trip)
{
    protocol::OwnedAugment owned{};

    owned[0] = {AugmentId::DamageUp, AugmentId::FireRateUp};
    owned[3] = {AugmentId::MaxHpUp};
    const std::optional<protocol::OwnedAugment> decoded =
        protocol::decodeAugments(protocol::encodeAugments(owned));

    CHECK(decoded.has_value());
    CHECK(*decoded == owned);
}

TEST(no_owned_augments_is_a_one_byte_message)
{
    const protocol::Payload payload =
        protocol::encodeAugments(protocol::OwnedAugment{});

    CHECK(payload.size() == 1);
    CHECK(protocol::decodeAugments(payload).has_value());
}

TEST(malformed_augments_are_rejected)
{
    const auto bad = static_cast<std::uint8_t>(AugmentId::Count);
    const auto noSlot = static_cast<std::uint8_t>(config::MAX_PLAYERS);

    CHECK(!protocol::decodeAugments({}).has_value());
    CHECK(!protocol::decodeAugments({1, 0, 1}).has_value());
    CHECK(!protocol::decodeAugments({1, 0, 1, bad}).has_value());
    CHECK(!protocol::decodeAugments({1, noSlot, 0}).has_value());
    CHECK(!protocol::decodeAugments({2, 1, 0, 1, 0}).has_value());
    CHECK(!protocol::decodeAugments({1, 0, 0, 0}).has_value());
    CHECK(!protocol::decodeAugments({static_cast<std::uint8_t>(noSlot + 1)}).has_value());
    CHECK(!protocol::decodeAugments({1, 0,
        static_cast<std::uint8_t>(protocol::MAX_OWNED_AUGMENTS + 1)})
        .has_value());
}

TEST(too_many_owned_augments_are_clamped_when_encoding)
{
    protocol::OwnedAugment owned{};

    owned[1].assign(protocol::MAX_OWNED_AUGMENTS + 5, AugmentId::SpeedUp);
    const std::optional<protocol::OwnedAugment> decoded =
        protocol::decodeAugments(protocol::encodeAugments(owned));

    CHECK(decoded.has_value());
    CHECK((*decoded)[1].size() == protocol::MAX_OWNED_AUGMENTS);
}

TEST(shop_offers_round_trip_and_are_validated)
{
    const std::vector<AugmentId> offers = {AugmentId::SpeedUp,
        AugmentId::MaxHpUp};
    const std::optional<std::vector<AugmentId>> decoded =
        protocol::decodeShop(protocol::encodeShop(offers));

    CHECK(decoded.has_value());
    CHECK(*decoded == offers);
    CHECK(protocol::decodeShop({0}).has_value());
    CHECK(!protocol::decodeShop({}).has_value());
    CHECK(!protocol::decodeShop({2, 0}).has_value());
    CHECK(!protocol::decodeShop({1, 0, 0}).has_value());
    CHECK(!protocol::decodeShop({1,
        static_cast<std::uint8_t>(AugmentId::Count)}).has_value());
    CHECK(!protocol::decodeShop({static_cast<std::uint8_t>(
        config::SHOP_OFFER_COUNT + 1)}).has_value());
}

TEST(every_augment_id_survives_a_buy_round_trip)
{
    for (const AugmentDef &def : AUGMENTS) {
        const std::optional<AugmentId> decoded =
            protocol::decodeBuy(protocol::encodeBuy(def.id));

        CHECK(decoded.has_value());
        CHECK(*decoded == def.id);
    }
}

TEST(a_malformed_buy_is_rejected)
{
    CHECK(!protocol::decodeBuy({}).has_value());
    CHECK(!protocol::decodeBuy({0, 0}).has_value());
    CHECK(!protocol::decodeBuy({static_cast<std::uint8_t>(AugmentId::Count)})
        .has_value());
}

TEST(a_buy_result_survives_an_encode_decode_round_trip)
{
    const protocol::BuyResult accepted{AugmentId::Shield, true};
    const protocol::BuyResult refused{AugmentId::MultiShot, false};
    const std::optional<protocol::BuyResult> first =
        protocol::decodeBuyResult(protocol::encode(accepted));
    const std::optional<protocol::BuyResult> second =
        protocol::decodeBuyResult(protocol::encode(refused));

    CHECK(first.has_value());
    CHECK(first->id == AugmentId::Shield);
    CHECK(first->ok);
    CHECK(second.has_value());
    CHECK(second->id == AugmentId::MultiShot);
    CHECK(!second->ok);
}

TEST(a_malformed_buy_result_is_rejected)
{
    const auto bad = static_cast<std::uint8_t>(AugmentId::Count);

    CHECK(!protocol::decodeBuyResult({}).has_value());
    CHECK(!protocol::decodeBuyResult({0}).has_value());
    CHECK(!protocol::decodeBuyResult({0, 1, 0}).has_value());
    CHECK(!protocol::decodeBuyResult({0, 2}).has_value());
    CHECK(!protocol::decodeBuyResult({bad, 1}).has_value());
}

TEST(the_lobby_phase_and_the_map_survive_a_status_round_trip)
{
    GameStatus status = sampleStatus();

    status.phase = Phase::Lobby;
    status.map = 2;
    const std::optional<GameStatus> decoded =
        protocol::decodeStatus(protocol::encode(status));

    CHECK(decoded.has_value());
    CHECK(decoded->phase == Phase::Lobby);
    CHECK(decoded->map == 2);
}

TEST(a_status_with_an_unknown_map_is_rejected)
{
    GameStatus status = sampleStatus();

    status.map = static_cast<std::uint8_t>(MAP_COUNT);
    CHECK(!protocol::decodeStatus(protocol::encode(status)).has_value());
}

TEST(a_map_selection_is_validated)
{
    CHECK(protocol::decodeSelectMap(protocol::encodeSelectMap(1)) == 1);
    CHECK(!protocol::decodeSelectMap({static_cast<std::uint8_t>(MAP_COUNT)})
        .has_value());
    CHECK(!protocol::decodeSelectMap({}).has_value());
    CHECK(!protocol::decodeSelectMap({0, 0}).has_value());
}

TEST(the_leaderboard_survives_a_round_trip)
{
    Leaderboard board;

    board.add(0, {61000, 2});
    board.add(0, {59000, 1});
    board.add(2, {12000, 4});
    const std::optional<AllRecords> decoded =
        protocol::decodeLeaderboard(protocol::encode(board.all()));

    CHECK(decoded.has_value());
    CHECK((*decoded)[0].size() == 2);
    CHECK((*decoded)[0][0].timeMs == 59000);
    CHECK((*decoded)[1].empty());
    CHECK((*decoded)[2][0].players == 4);
}

TEST(a_bad_leaderboard_is_rejected)
{
    Leaderboard board;
    protocol::Payload payload = protocol::encode(board.all());

    payload.push_back(0);
    CHECK(!protocol::decodeLeaderboard(payload).has_value());
    CHECK(!protocol::decodeLeaderboard({}).has_value());
    CHECK(!protocol::decodeLeaderboard({9, 0, 0}).has_value());
    board.add(0, {1000, 1});
    payload = protocol::encode(board.all());
    payload[5] = 0;
    CHECK(!protocol::decodeLeaderboard(payload).has_value());
}

TEST(the_effects_of_the_portals_survive_an_own_state_round_trip)
{
    protocol::OwnState state;

    state.lastInput = 9;
    state.pos = gameLogic::Vec2Pos{{10.f, 20.f}};
    state.inverted = true;
    state.speedPercent = 150;
    const std::optional<protocol::OwnState> decoded =
        protocol::decodeOwnState(protocol::encode(state));

    CHECK(decoded.has_value());
    CHECK(decoded->lastInput == 9);
    CHECK(decoded->inverted);
    CHECK(decoded->speedPercent == 150);
}

TEST(an_own_state_with_bad_effects_is_rejected)
{
    protocol::Payload payload = protocol::encode(protocol::OwnState{});

    CHECK(payload.size() == protocol::OWN_STATE_SIZE);
    payload[12] = 2;
    CHECK(!protocol::decodeOwnState(payload).has_value());
    payload[12] = 0;
    payload[13] = 0;
    CHECK(!protocol::decodeOwnState(payload).has_value());
    payload.pop_back();
    CHECK(!protocol::decodeOwnState(payload).has_value());
}

TEST(a_paused_status_survives_a_round_trip)
{
    GameStatus status;

    status.phase = Phase::Playing;
    status.paused = true;
    const auto decoded = protocol::decodeStatus(protocol::encode(status));
    CHECK(decoded.has_value());
    CHECK(decoded->paused);
    CHECK(decoded->phase == Phase::Playing);
}

TEST(a_pause_message_survives_a_round_trip)
{
    CHECK(protocol::decodePause(protocol::encodePause(true)) == true);
    CHECK(protocol::decodePause(protocol::encodePause(false)) == false);
    CHECK(!protocol::decodePause({2}).has_value());
    CHECK(!protocol::decodePause({}).has_value());
}

TEST(the_deaths_survive_a_status_round_trip)
{
    GameStatus status;

    status.deaths = 300;
    const auto decoded = protocol::decodeStatus(protocol::encode(status));
    CHECK(decoded.has_value());
    CHECK(decoded->deaths == 300);
}
