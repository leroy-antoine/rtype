#include <algorithm>
#include <vector>
#include "logic/Augments.hpp"
#include "logic/Damages.hpp"
#include "logic/Factories.hpp"
#include "logic/Systems.hpp"
#include "TestHarness.hpp"

using namespace rtype;

TEST(every_augment_id_has_one_definition)
{
    CHECK(findAugment(AugmentId::DamageUp).has_value());
    CHECK(findAugment(AugmentId::SpeedUp).has_value());
    CHECK(findAugment(AugmentId::MaxHpUp).has_value());
    CHECK(findAugment(AugmentId::FireRateUp).has_value());
    CHECK(findAugment(AugmentId::Regeneration).has_value());
    CHECK(findAugment(AugmentId::Shield).has_value());
    CHECK(findAugment(AugmentId::MultiShot).has_value());
    CHECK(!findAugment(AugmentId::Count).has_value());
}

TEST(every_rarity_has_at_least_one_augment)
{
    for (const Rarity rarity : {Rarity::Common, Rarity::Rare, Rarity::Epic,
        Rarity::Legendary}) {
        CHECK(std::ranges::any_of(AUGMENTS, [rarity](const AugmentDef &def) {
            return def.rarity == rarity;
        }));
    }
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
    CHECK(reg.get<MaxHealth>(player).hp
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

TEST(multi_shot_fires_a_fan_of_bullets_to_the_right)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    applyAugment(reg, player, AugmentId::MultiShot);
    const std::vector<gameLogic::Vec2> shots = shotVelocities(reg, player);

    CHECK(shots.size() == config::MULTISHOT_BULLETS);
    for (const gameLogic::Vec2 &shot : shots) {
        CHECK(shot.x > 0.f);
    }
    CHECK(shots.front().y < 0.f);
    CHECK(shots.back().y > 0.f);
}

TEST(multi_shot_creates_one_bullet_per_shot)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    applyAugment(reg, player, AugmentId::MultiShot);
    shoot(reg, player);
    CHECK(collectProjectileEntities(reg).size() == config::MULTISHOT_BULLETS);
}

TEST(multi_shot_stacks_extra_bullets)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    applyAugment(reg, player, AugmentId::MultiShot);
    applyAugment(reg, player, AugmentId::MultiShot);
    CHECK(shotVelocities(reg, player).size()
        == config::MULTISHOT_BULLETS + config::MULTISHOT_EXTRA_BULLETS);
}

TEST(the_boss_fan_is_unchanged)
{
    const std::vector<gameLogic::Vec2> shots =
        volley(bossDef(BossKind::Standard).projectile);

    CHECK(shots.size() == config::BOSS_FAN_BULLETS);
    CHECK(shots[config::BOSS_FAN_BULLETS / 2].x
        == -config::BOSS_BULLET_SPEED);
}

TEST(a_shield_absorbs_one_hit_then_recharges)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);
    const Engine::Entity monster = createMonster(reg);

    applyAugment(reg, player, AugmentId::Shield);
    applyDamages(player, monster, reg);
    CHECK(reg.get<Health>(player).hp == config::PLAYER_HP);
    applyDamages(player, monster, reg);
    CHECK(reg.get<Health>(player).hp
        == config::PLAYER_HP - config::MONSTER_DAMAGE);
    updateShieldTimers(reg, config::SHIELD_COOLDOWN);
    applyDamages(player, monster, reg);
    CHECK(reg.get<Health>(player).hp
        == config::PLAYER_HP - config::MONSTER_DAMAGE);
}

TEST(regeneration_heals_over_time)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    applyAugment(reg, player, AugmentId::Regeneration);
    reg.get<Health>(player).hp = 50;
    regenerationSystem(reg, config::REGENERATION_INTERVAL / 2.f);
    CHECK(reg.get<Health>(player).hp == 50);
    regenerationSystem(reg, config::REGENERATION_INTERVAL / 2.f);
    CHECK(reg.get<Health>(player).hp == 50 + config::REGENERATION_HP);
}

TEST(regeneration_never_exceeds_the_max_health)
{
    Engine::Registry reg;
    const Engine::Entity player = createPlayer(reg, 0);

    applyAugment(reg, player, AugmentId::Regeneration);
    regenerationSystem(reg, config::REGENERATION_INTERVAL);
    CHECK(reg.get<Health>(player).hp == config::PLAYER_HP);
}
