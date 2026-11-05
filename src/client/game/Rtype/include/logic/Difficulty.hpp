#pragma once

#include <algorithm>
#include <cmath>
#include "engine/core/Registry.hpp"
#include "Components.hpp"
#include "Settings.hpp"

namespace rtype {
    // Les reglages de la partie, portes par une entite unique du registre.
    struct ActiveSettings {
        Settings value;
    };

    struct DifficultyApplied {
    };

    inline Engine::Entity createSettingsHolder(Engine::Registry &reg)
    {
        const Engine::Entity e = reg.create();

        reg.add(e, ActiveSettings{});
        return e;
    }

    inline void storeSettings(Engine::Registry &reg, const Settings &settings)
    {
        reg.each<ActiveSettings>(
            [&settings](const Engine::Entity, ActiveSettings &active) {
                active.value = settings;
            });
    }

    inline Settings activeSettings(Engine::Registry &reg)
    {
        Settings found;

        reg.each<ActiveSettings>(
            [&found](const Engine::Entity, const ActiveSettings &active) {
                found = active.value;
            });
        return found;
    }

    inline const DifficultyScale &activeScale(Engine::Registry &reg)
    {
        return scaleOf(activeSettings(reg));
    }

    // Une quantite multipliee puis arrondie, jamais ramenee a zero.
    inline int scaledAmount(const int base, const float factor)
    {
        if (base <= 0) {
            return base;
        }
        const long scaled = std::lround(static_cast<float>(base) * factor);

        return std::max(1, static_cast<int>(scaled));
    }

    inline float damageFactor(Engine::Registry &reg,
        const Engine::Entity victim, const Engine::Entity attacker)
    {
        if (!reg.has<Team>(victim) || !reg.has<Team>(attacker)) {
            return 1.f;
        }
        const TeamId from = reg.get<Team>(attacker).id;
        const TeamId to = reg.get<Team>(victim).id;

        if (from == TeamId::Player && to == TeamId::Monster) {
            return activeScale(reg).allyDamage;
        }
        if (from == TeamId::Monster && to == TeamId::Player) {
            return activeScale(reg).enemyDamage;
        }
        return 1.f;
    }
}
