#pragma once

#include <iostream>
#include "engine/core/Registry.hpp"
#include "Components.hpp"

namespace rtype {
    inline bool absorbHit(Engine::Registry &reg, const Engine::Entity victim)
    {
        if (!reg.has<Shield>(victim) || reg.get<Shield>(victim).timer > 0.f) {
            return false;
        }
        Shield &shield = reg.get<Shield>(victim);

        shield.timer = shield.cooldown;
        return true;
    }

    inline void applyDamages(const Engine::Entity victim, const Engine::Entity attacker,
        Engine::Registry &reg)
    {
        if (!reg.has<Health>(victim) || !reg.has<Damage>(attacker)) {
            std::cerr << "applyDamages: victim has no Health or attacker has no Damage" << std::endl;
            return;
        }
        if (absorbHit(reg, victim)) {
            return;
        }
        reg.get<Health>(victim).hp -= reg.get<Damage>(attacker).value;
    }
}
