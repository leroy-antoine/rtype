#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include "engine/core/Registry.hpp"
#include "Components.hpp"
#include "Config.hpp"

namespace rtype {
    enum class Rarity : std::uint8_t { Common, Rare, Epic, Legendary };

    enum class AugmentId : std::uint8_t {
        DamageUp, SpeedUp, MaxHpUp, FireRateUp, Count
    };

    using AugmentEffect = void (*)(Engine::Registry &, Engine::Entity);

    struct AugmentDef {
        AugmentId id;
        Rarity rarity;
        int price;
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
    }

    inline void reduceFireCooldown(Engine::Registry &reg,
        const Engine::Entity e)
    {
        if (reg.has<Weapon>(e)) {
            reg.get<Weapon>(e).cooldown *= config::AUGMENT_FIRE_COOLDOWN_FACTOR;
        }
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
    };

    inline std::optional<AugmentDef> findAugment(const AugmentId id)
    {
        for (const AugmentDef &def : AUGMENTS) {
            if (def.id == id) {
                return def;
            }
        }
        return std::nullopt;
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
