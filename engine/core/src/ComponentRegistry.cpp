#include <mutex>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include "engine/core/Component.hpp"

namespace {
    struct RegistryData {
        std::mutex mutex;
        std::vector<Engine::detail::ComponentInfo> infos;
        std::unordered_map<std::type_index, Engine::detail::ComponentId> ids;
    };

    RegistryData &data()
    {
        static RegistryData instance;

        return instance;
    }
}

namespace Engine {
    namespace detail {
        ComponentInfo ComponentRegistry::getInfo(ComponentId id)
        {
            RegistryData &registry = data();
            std::lock_guard lock(registry.mutex);

            return registry.infos.at(id.id);
        }

        ComponentId ComponentRegistry::registerType(std::type_index type, ComponentInfo info)
        {
            RegistryData &registry = data();
            std::lock_guard lock(registry.mutex);
            auto found = registry.ids.find(type);
            ComponentId id;

            if (found != registry.ids.end()) {
                return found->second;
            }
            id = {registry.infos.size()};
            registry.infos.push_back(info);
            registry.ids.emplace(type, id);
            return id;
        }
    }
}
