#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <optional>
#include <string>
#include "engine/window/Types.hpp"
#include "logic/Augments.hpp"

namespace client {
    inline std::string augmentLabel(const rtype::AugmentId id)
    {
        switch (id) {
            case rtype::AugmentId::DamageUp: return "DMG";
            case rtype::AugmentId::SpeedUp: return "SPD";
            case rtype::AugmentId::MaxHpUp: return "HP";
            case rtype::AugmentId::FireRateUp: return "FR";
            case rtype::AugmentId::Regeneration: return "REG";
            case rtype::AugmentId::Shield: return "SHD";
            case rtype::AugmentId::MultiShot: return "MS";
            case rtype::AugmentId::Rocket: return "RKT";
            case rtype::AugmentId::Laser: return "LAS";
            case rtype::AugmentId::Lightning: return "ARC";
            case rtype::AugmentId::Drone: return "DRN";
            case rtype::AugmentId::DroneBoost: return "BST";
            case rtype::AugmentId::DroneTwin: return "TWN";
            case rtype::AugmentId::Count: break;
        }
        return "?";
    }

    // The full name under the three letters of the label.
    inline std::string augmentName(const rtype::AugmentId id)
    {
        static constexpr std::array<const char *,
            static_cast<std::size_t>(rtype::AugmentId::Count)> NAMES = {
            "DAMAGE UP", "SPEED UP", "MAX HEALTH UP", "FIRE RATE UP",
            "REGENERATION", "SHIELD", "MULTI SHOT", "ROCKET LAUNCHER",
            "LASER RAY", "CHAIN LIGHTNING", "DRONE", "DRONE BOOST",
            "DRONE TWIN"};
        const auto index = static_cast<std::size_t>(id);

        if (index >= NAMES.size()) {
            return "?";
        }
        return NAMES[index];
    }

    inline std::string whole(const float value)
    {
        return std::to_string(static_cast<int>(std::lround(value)));
    }

    inline std::string percentLess(const float factor)
    {
        return whole((1.f - factor) * 100.f);
    }

    // What each augment does. The numbers come from the config, so the text
    // follows the game when a value changes. Each function knows a group of
    // augments and answers "" for the others.
    inline std::string statInfo(const rtype::AugmentId id)
    {
        switch (id) {
            case rtype::AugmentId::DamageUp:
                return "+" + whole(rtype::config::AUGMENT_DAMAGE_BONUS)
                    + " DAMAGE ON EVERY SHOT";
            case rtype::AugmentId::SpeedUp:
                return "+" + whole(rtype::config::AUGMENT_SPEED_BONUS)
                    + " SHIP SPEED";
            case rtype::AugmentId::MaxHpUp:
                return "+" + whole(rtype::config::AUGMENT_HP_BONUS)
                    + " HEALTH AND MAX HEALTH";
            case rtype::AugmentId::FireRateUp:
                return "SHOOT FASTER: " + percentLess(
                    rtype::config::AUGMENT_FIRE_COOLDOWN_FACTOR)
                    + "% LESS TIME BETWEEN SHOTS";
            default: return "";
        }
    }

    inline std::string skillInfo(const rtype::AugmentId id)
    {
        switch (id) {
            case rtype::AugmentId::Regeneration:
                return "HEALS " + whole(rtype::config::REGENERATION_HP)
                    + " HP EVERY " + whole(rtype::config::REGENERATION_INTERVAL)
                    + " S. BUY AGAIN: HEAL MORE";
            case rtype::AugmentId::Shield:
                return "BLOCKS A HIT, THEN "
                    + whole(rtype::config::SHIELD_COOLDOWN)
                    + " S TO RECHARGE. BUY AGAIN: FASTER";
            case rtype::AugmentId::MultiShot:
                return "FIRES " + whole(rtype::config::MULTISHOT_BULLETS)
                    + " BULLETS AT ONCE. EACH PURCHASE ADDS "
                    + whole(rtype::config::MULTISHOT_EXTRA_BULLETS);
            default: return "";
        }
    }

    // The three secondary weapons go in the slot fired with X.
    inline std::string weaponInfo(const rtype::AugmentId id)
    {
        switch (id) {
            case rtype::AugmentId::Rocket:
                return "SECONDARY (X): EXPLODING ROCKET, "
                    + whole(rtype::config::ROCKET_DAMAGE_FACTOR)
                    + "X DAMAGE IN AREA";
            case rtype::AugmentId::Laser:
                return "SECONDARY (X): HOLD "
                    + whole(rtype::config::LASER_CHARGE_TIME)
                    + " S, RELEASE: RAY OF "
                    + whole(rtype::config::LASER_DAMAGE_FACTOR)
                    + "X DAMAGE";
            case rtype::AugmentId::Lightning:
                return "SECONDARY (X): LIGHTNING JUMPING BETWEEN "
                    + whole(rtype::config::LIGHTNING_JUMPS) + " ENEMIES";
            default: return "";
        }
    }

    inline std::string droneInfo(const rtype::AugmentId id)
    {
        switch (id) {
            case rtype::AugmentId::Drone:
                return "A DRONE SHOOTS BY ITSELF, WITH HALF YOUR DAMAGE";
            case rtype::AugmentId::DroneBoost:
                return "DRONE LEVEL 2: IT FIRES FASTER";
            case rtype::AugmentId::DroneTwin:
                return "DRONE LEVEL 3: IT FIRES 2 BULLETS AT ONCE";
            default: return "";
        }
    }

    inline std::string augmentInfo(const rtype::AugmentId id)
    {
        for (const std::string &info : {statInfo(id), skillInfo(id),
            weaponInfo(id), droneInfo(id)}) {
            if (!info.empty()) {
                return info;
            }
        }
        return "";
    }

    inline std::string rarityName(const rtype::Rarity rarity)
    {
        switch (rarity) {
            case rtype::Rarity::Common: return "COMMON";
            case rtype::Rarity::Rare: return "RARE";
            case rtype::Rarity::Epic: return "EPIC";
            case rtype::Rarity::Legendary: return "LEGENDARY";
        }
        return "?";
    }

    inline Engine::Color rarityColor(const rtype::Rarity rarity)
    {
        switch (rarity) {
            case rtype::Rarity::Common: return {170, 170, 170, 255};
            case rtype::Rarity::Rare: return {80, 150, 255, 255};
            case rtype::Rarity::Epic: return {190, 90, 255, 255};
            case rtype::Rarity::Legendary: return {255, 165, 0, 255};
        }
        return {255, 255, 255, 255};
    }

    inline Engine::Color rarityColor(const rtype::AugmentId id)
    {
        const std::optional<rtype::AugmentDef> def = rtype::findAugment(id);

        if (!def) {
            return {255, 255, 255, 255};
        }
        return rarityColor(def->rarity);
    }
}
