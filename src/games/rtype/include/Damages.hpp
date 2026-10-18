#pragma once

#include <iostream>
#include "engine/core/Registry.hpp"
#include "Components.hpp"

namespace rtype {
    inline void applyDamages(const Engine::Entity victim, const Engine::Entity attacker,
        Engine::Registry &reg)
    {
        if (!reg.has<Health>(victim) || !reg.has<Damage>(attacker)) {
            std::cerr << "ERROR : one of the two entities is missing a component (see Systmes.hpp line 27)" << std::endl;
            return;
        }
        reg.get<Health>(victim).hp -= reg.get<Damage>(attacker).value;
    }
}
