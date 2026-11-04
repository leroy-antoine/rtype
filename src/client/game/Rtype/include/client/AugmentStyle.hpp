#pragma once

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
            case rtype::AugmentId::Count: break;
        }
        return "?";
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
