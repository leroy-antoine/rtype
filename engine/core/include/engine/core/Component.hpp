#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>
#include <atomic>

namespace Engine {
    struct ComponentId {
        std::uint64_t id = std::numeric_limits<std::uint64_t>::max();

        static const ComponentId null;

        bool operator==(const ComponentId &) const = default;
    };

    inline constexpr ComponentId ComponentId::null{};

    class ComponentRegistry
    {
    public:
        template<typename T>
        static ComponentId getId()
        {
            return idOf<std::remove_cvref_t<T>>();
        }
    private:
        template<typename T>
        static ComponentId idOf()
        {
            static const ComponentId id{_next++};
            
            return id;
        }
        inline static std::atomic<std::uint64_t> _next = 0;
    };

    struct ComponentInfo {
        std::size_t size;
        
    };
}
