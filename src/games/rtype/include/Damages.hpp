#pragma once

#include <iostream>
#include "engine/core/Registry.hpp"
#include "Components.hpp"

namespace rtype {
    inline void applyDamages(const Engine::Entity victim, const Engine::Entity attacker,
        Engine::Registry &reg)
    {
        if (!reg.has<Health>(victim) || !reg.has<Damage>(attacker)) {
            std::cerr << "applyDamages: victim has no Health or attacker has no Damage" << std::endl;
            return;
        }
        reg.get<Health>(victim).hp -= reg.get<Damage>(attacker).value;
    }
}
