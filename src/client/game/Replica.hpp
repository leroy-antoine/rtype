#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include "engine/core/Registry.hpp"
#include "Events.hpp"

namespace client {
    class Replica {
    public:
        void apply(const rtype::EntitySpawned &spawn);
        void apply(const rtype::EntityDestroyed &destroy);
        void apply(const std::vector<rtype::EntityState> &states);
        void update(float dt);

        Engine::Registry &registry();

    private:
        void apply(const rtype::EntityState &state);

        Engine::Registry _reg;
        std::unordered_map<std::uint32_t, Engine::Entity> _entities;
    };
}
