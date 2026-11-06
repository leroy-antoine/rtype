#include "logic/Game.hpp"
#include "logic/Secondary.hpp"
#include "rtype/Protocol.hpp"
#include "TestHarness.hpp"

using namespace rtype;

TEST(the_alt_key_survives_an_input_round_trip)
{
    PlayerInput input;

    input.alt = true;
    const auto decoded = protocol::decodeInput(protocol::encodeInput(7, input));

    CHECK(decoded.has_value());
    CHECK(decoded->input.alt);
    CHECK(!decoded->input.fire);
}

TEST(an_unknown_key_bit_is_still_rejected)
{
    protocol::Payload payload = protocol::encodeInput(1, PlayerInput{});

    payload[4] = 0x40;
    CHECK(!protocol::decodeInput(payload).has_value());
    payload[4] = protocol::KEY_ALT;
    CHECK(protocol::decodeInput(payload).has_value());
}

TEST(a_player_starts_without_a_secondary_weapon)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    CHECK(!reg.has<SecondaryWeapon>(player));
}

TEST(equipping_a_weapon_sets_its_kind_and_cooldown)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    equipSecondary(reg, player, WeaponKind::Laser);
    CHECK(reg.get<SecondaryWeapon>(player).kind == WeaponKind::Laser);
    CHECK(reg.get<SecondaryWeapon>(player).cooldown == config::LASER_COOLDOWN);
}

TEST(a_new_weapon_replaces_the_previous_one)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    equipSecondary(reg, player, WeaponKind::Laser);
    reg.get<SecondaryWeapon>(player).timer = 1.f;
    equipSecondary(reg, player, WeaponKind::Rocket);
    CHECK(reg.get<SecondaryWeapon>(player).kind == WeaponKind::Rocket);
    CHECK(reg.get<SecondaryWeapon>(player).timer == 0.f);
}

TEST(the_recharge_of_the_secondary_weapon_runs_down)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    equipSecondary(reg, player, WeaponKind::Rocket);
    reg.get<SecondaryWeapon>(player).timer = 1.f;
    secondarySystem(reg, 0.4f);
    CHECK(reg.get<SecondaryWeapon>(player).timer > 0.59f);
    CHECK(reg.get<SecondaryWeapon>(player).timer < 0.61f);
    secondarySystem(reg, 5.f);
    CHECK(reg.get<SecondaryWeapon>(player).timer == 0.f);
}

TEST(buying_a_weapon_equips_it_and_the_last_one_wins_after_a_respawn)
{
    Game game(1);

    game.addPlayer();
    game.addPlayer();
    CHECK(game.addAugment(0, AugmentId::Laser));
    CHECK(game.addAugment(0, AugmentId::Rocket));
    const Engine::Entity old = *findPlayerBySlot(game.registry(), 0);

    CHECK(game.registry().get<SecondaryWeapon>(old).kind == WeaponKind::Rocket);
    game.registry().get<Health>(old).hp = 0;
    game.update(config::FIXED_DT);
    game.update(config::FIXED_DT);
    game.respawnPlayer(0);
    const Engine::Entity player = *findPlayerBySlot(game.registry(), 0);

    CHECK(game.registry().get<SecondaryWeapon>(player).kind
        == WeaponKind::Rocket);
}

TEST(the_weapons_are_sold_in_the_shop_like_any_augment)
{
    for (const AugmentId id : {AugmentId::Rocket, AugmentId::Laser,
        AugmentId::Lightning}) {
        const auto def = findAugment(id);

        CHECK(def.has_value());
        CHECK(def->price > 0);
    }
}
