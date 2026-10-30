#include "logic/Augments.hpp"
#include "logic/Factories.hpp"
#include "TestHarness.hpp"

using namespace rtype;

TEST(every_augment_id_has_one_definition)
{
    CHECK(findAugment(AugmentId::DamageUp).has_value());
    CHECK(findAugment(AugmentId::SpeedUp).has_value());
    CHECK(findAugment(AugmentId::MaxHpUp).has_value());
    CHECK(findAugment(AugmentId::FireRateUp).has_value());
    CHECK(!findAugment(AugmentId::Count).has_value());
}

TEST(every_augment_has_an_effect_and_a_price)
{
    for (const AugmentDef &def : AUGMENTS) {
        CHECK(def.apply != nullptr);
        CHECK(def.price > 0);
    }
}

TEST(damage_up_increases_the_player_damage)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    CHECK(applyAugment(reg, player, AugmentId::DamageUp));
    CHECK(reg.get<Damage>(player).value
        == config::PLAYER_DAMAGE + config::AUGMENT_DAMAGE_BONUS);
}

TEST(speed_up_increases_the_player_speed)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    CHECK(applyAugment(reg, player, AugmentId::SpeedUp));
    CHECK(reg.get<Speed>(player).value
        == config::PLAYER_SPEED + config::AUGMENT_SPEED_BONUS);
}

TEST(max_hp_up_increases_the_player_health)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    CHECK(applyAugment(reg, player, AugmentId::MaxHpUp));
    CHECK(reg.get<Health>(player).hp
        == config::PLAYER_HP + config::AUGMENT_HP_BONUS);
}

TEST(fire_rate_up_shortens_the_weapon_cooldown)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    CHECK(applyAugment(reg, player, AugmentId::FireRateUp));
    CHECK(reg.get<Weapon>(player).cooldown
        < config::PLAYER_FIRE_COOLDOWN);
}

TEST(augments_stack)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    applyAugment(reg, player, AugmentId::DamageUp);
    applyAugment(reg, player, AugmentId::DamageUp);
    CHECK(reg.get<Damage>(player).value
        == config::PLAYER_DAMAGE + 2 * config::AUGMENT_DAMAGE_BONUS);
}

TEST(an_augment_on_a_dead_entity_does_nothing)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    reg.destroy(player);
    CHECK(!applyAugment(reg, player, AugmentId::DamageUp));
    CHECK(!applyAugment(reg, player, AugmentId::Count));
}
