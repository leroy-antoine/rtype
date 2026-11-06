#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
#include "engine/core/Registry.hpp"
#include "Components.hpp"
#include "Config.hpp"
#include "Secondary.hpp"

namespace rtype {
    enum class Rarity : std::uint8_t { Common, Rare, Epic, Legendary };

    enum class AugmentId : std::uint8_t {
        DamageUp, SpeedUp, MaxHpUp, FireRateUp, Regeneration, Shield,
        MultiShot, Rocket, Laser, Lightning, Drone, DroneBoost, DroneTwin,
        Count
    };

    using AugmentEffect = void (*)(Engine::Registry &, Engine::Entity);

    struct AugmentDef {
        AugmentId id;
        Rarity rarity;
        std::size_t price;
        AugmentEffect apply;
    };

    inline void addDamage(Engine::Registry &reg, const Engine::Entity e)
    {
        if (reg.has<Damage>(e)) {
            reg.get<Damage>(e).value += config::AUGMENT_DAMAGE_BONUS;
        }
    }

    inline void addSpeed(Engine::Registry &reg, const Engine::Entity e)
    {
        if (reg.has<Speed>(e)) {
            reg.get<Speed>(e).value += config::AUGMENT_SPEED_BONUS;
        }
    }

    inline void addHp(Engine::Registry &reg, const Engine::Entity e)
    {
        if (reg.has<Health>(e)) {
            reg.get<Health>(e).hp += config::AUGMENT_HP_BONUS;
        }
        if (reg.has<MaxHealth>(e)) {
            reg.get<MaxHealth>(e).hp += config::AUGMENT_HP_BONUS;
        }
    }

    inline void reduceFireCooldown(Engine::Registry &reg,
        const Engine::Entity e)
    {
        if (reg.has<Weapon>(e)) {
            reg.get<Weapon>(e).cooldown *= config::AUGMENT_FIRE_COOLDOWN_FACTOR;
        }
    }

    inline void addRegeneration(Engine::Registry &reg, const Engine::Entity e)
    {
        if (reg.has<Regeneration>(e)) {
            reg.get<Regeneration>(e).hp += config::REGENERATION_HP;
            return;
        }
        reg.add(e, Regeneration{});
    }

    inline void addShield(Engine::Registry &reg, const Engine::Entity e)
    {
        if (reg.has<Shield>(e)) {
            reg.get<Shield>(e).cooldown *= config::SHIELD_COOLDOWN_FACTOR;
            return;
        }
        reg.add(e, Shield{});
    }

    inline void addMultiShot(Engine::Registry &reg, const Engine::Entity e)
    {
        if (reg.has<MultiShot>(e)) {
            reg.get<MultiShot>(e).bullets += config::MULTISHOT_EXTRA_BULLETS;
            return;
        }
        reg.add(e, MultiShot{});
    }

    // Le drone prend le niveau de l'article, sans jamais en perdre.
    inline void setDroneLevel(Engine::Registry &reg, const Engine::Entity e,
        const int level)
    {
        if (reg.has<DroneLevel>(e)) {
            DroneLevel &drone = reg.get<DroneLevel>(e);

            drone.level = std::max(drone.level, level);
            return;
        }
        reg.add(e, DroneLevel{level});
    }

    inline void addDrone(Engine::Registry &reg, const Engine::Entity e)
    {
        setDroneLevel(reg, e, 1);
    }

    inline void boostDrone(Engine::Registry &reg, const Engine::Entity e)
    {
        setDroneLevel(reg, e, 2);
    }

    inline void twinDrone(Engine::Registry &reg, const Engine::Entity e)
    {
        setDroneLevel(reg, e, config::DRONE_MAX_LEVEL);
    }

    inline void equipRocket(Engine::Registry &reg, const Engine::Entity e)
    {
        equipSecondary(reg, e, WeaponKind::Rocket);
    }

    inline void equipLaser(Engine::Registry &reg, const Engine::Entity e)
    {
        equipSecondary(reg, e, WeaponKind::Laser);
    }

    inline void equipLightning(Engine::Registry &reg, const Engine::Entity e)
    {
        equipSecondary(reg, e, WeaponKind::Lightning);
    }

    inline constexpr std::array AUGMENTS = {
        AugmentDef{AugmentId::DamageUp, Rarity::Common,
            config::AUGMENT_DAMAGE_PRICE, addDamage},
        AugmentDef{AugmentId::SpeedUp, Rarity::Common,
            config::AUGMENT_SPEED_PRICE, addSpeed},
        AugmentDef{AugmentId::MaxHpUp, Rarity::Rare,
            config::AUGMENT_HP_PRICE, addHp},
        AugmentDef{AugmentId::FireRateUp, Rarity::Epic,
            config::AUGMENT_FIRE_RATE_PRICE, reduceFireCooldown},
        AugmentDef{AugmentId::Regeneration, Rarity::Rare,
            config::AUGMENT_REGENERATION_PRICE, addRegeneration},
        AugmentDef{AugmentId::Shield, Rarity::Epic,
            config::AUGMENT_SHIELD_PRICE, addShield},
        AugmentDef{AugmentId::MultiShot, Rarity::Legendary,
            config::AUGMENT_MULTISHOT_PRICE, addMultiShot},
        AugmentDef{AugmentId::Rocket, Rarity::Rare,
            config::AUGMENT_ROCKET_PRICE, equipRocket},
        AugmentDef{AugmentId::Laser, Rarity::Epic,
            config::AUGMENT_LASER_PRICE, equipLaser},
        AugmentDef{AugmentId::Lightning, Rarity::Epic,
            config::AUGMENT_LIGHTNING_PRICE, equipLightning},
        AugmentDef{AugmentId::Drone, Rarity::Rare,
            config::AUGMENT_DRONE_PRICE, addDrone},
        AugmentDef{AugmentId::DroneBoost, Rarity::Epic,
            config::AUGMENT_DRONE_BOOST_PRICE, boostDrone},
        AugmentDef{AugmentId::DroneTwin, Rarity::Legendary,
            config::AUGMENT_DRONE_TWIN_PRICE, twinDrone},
    };

    inline std::optional<WeaponKind> weaponOf(const AugmentId id)
    {
        switch (id) {
            case AugmentId::Rocket: return WeaponKind::Rocket;
            case AugmentId::Laser: return WeaponKind::Laser;
            case AugmentId::Lightning: return WeaponKind::Lightning;
            default: return std::nullopt;
        }
    }

    // L'arme secondaire equipee : la derniere arme achetee.
    inline std::optional<WeaponKind> equippedWeapon(
        const std::vector<AugmentId> &owned)
    {
        for (auto it = owned.rbegin(); it != owned.rend(); ++it) {
            if (const std::optional<WeaponKind> kind = weaponOf(*it)) {
                return kind;
            }
        }
        return std::nullopt;
    }

    inline std::optional<AugmentDef> findAugment(const AugmentId id)
    {
        for (const AugmentDef &def : AUGMENTS) {
            if (def.id == id) {
                return def;
            }
        }
        return std::nullopt;
    }

    inline std::size_t priceOf(const AugmentId id)
    {
        const std::optional<AugmentDef> def = findAugment(id);

        if (!def) {
            return 0;
        }
        return def->price;
    }

    inline bool applyAugment(Engine::Registry &reg, const Engine::Entity e,
        const AugmentId id)
    {
        const std::optional<AugmentDef> def = findAugment(id);

        if (!def || !reg.alive(e)) {
            return false;
        }
        def->apply(reg, e);
        return true;
    }
}
