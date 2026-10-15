#ifndef RTYPE_SYSTEMS_HPP
#define RTYPE_SYSTEMS_HPP

#include <vector>
#include "engine/core/Registry.hpp"
#include "Components.hpp"

namespace rtype {
    inline void deathSystem(Engine::Registry &reg)
    {
        std::vector<Engine::Entity> dead;

        reg.each<Health>([&dead](Engine::Entity e, const Health &health) {
            if (health.hp <= 0) {
                dead.push_back(e);
            }
        });
        for (Engine::Entity e : dead) {
            reg.destroy(e);
        }
    }
}

#endif //RTYPE_SYSTEMS_HPP
