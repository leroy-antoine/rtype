#pragma once

#include <vector>
#include <utility>
#include <functional>
#include "engine/core/Registry.hpp"
#include "engine/core/CommandBuffer.hpp"

namespace Engine {
    class Scheduler
    {
    public:
        using System = std::function<void(Registry &, CommandBuffer &, float)>;

        explicit Scheduler(Registry &registry) : _registry(registry), _commands(registry)
        {
        }

        void add(System system)
        {
            _systems.push_back(std::move(system));
        }

        void run(float dt)
        {
            for (System &system : _systems) {
                system(_registry, _commands, dt);
                _commands.apply();
            }
        }
    private:
        Registry &_registry;
        std::vector<System> _systems;
        CommandBuffer _commands;
    };
}
