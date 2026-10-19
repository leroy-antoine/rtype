#pragma once

#include <vector>
#include <utility>
#include <functional>
#include "engine/core/Entity.hpp"
#include "engine/core/Registry.hpp"

namespace Engine {
    class CommandBuffer
    {
    public:
        explicit CommandBuffer(Registry &registry) : _registry(registry)
        {
        }

        CommandBuffer(const CommandBuffer &) = delete;
        CommandBuffer &operator=(const CommandBuffer &) = delete;

        template<typename... Ts>
        Entity create(Ts... components)
        {
            Entity e;

            e = _registry.reserve();
            _commands.emplace_back([e, ... components = std::move(components)](Registry &registry) mutable {
                if (!registry.reserved(e)) {
                    return;
                }
                registry.materialize(e);
                (registry.add<Ts>(e, std::move(components)), ...);
            });
            return e;
        }

        void destroy(Entity e)
        {
            _commands.emplace_back([e](Registry &registry) {
                registry.destroy(e);
            });
        }

        template<typename T>
        void add(Entity e, T component = {})
        {
            _commands.emplace_back([e, component = std::move(component)](Registry &registry) mutable {
                if (registry.alive(e)) {
                    registry.add<T>(e, std::move(component));
                }
            });
        }

        template<typename T>
        void remove(Entity e)
        {
            _commands.emplace_back([e](Registry &registry) {
                registry.remove<T>(e);
            });
        }

        void apply()
        {
            _registry.checkNotIterating("CommandBuffer::apply");
            for (std::function<void(Registry &)> &command : _commands) {
                command(_registry);
            }
            _commands.clear();
        }
    private:
        Registry &_registry;
        std::vector<std::function<void(Registry &)>> _commands;
    };
}
