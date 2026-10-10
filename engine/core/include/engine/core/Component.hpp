#pragma once

#include <cstdint>
#include <cstddef>
#include <limits>
#include <mutex>
#include <new>
#include <type_traits>
#include <utility>
#include <vector>

namespace Engine {
    struct ComponentId {
        std::uint64_t id = std::numeric_limits<std::uint64_t>::max();

        bool operator==(const ComponentId &) const = default;
    };

    struct ComponentInfo {
        std::size_t size;
        std::size_t align;
        void (*move)(void *dest, void *src);
        void (*destroy)(void *ptr);
    };

    template<typename T>
    ComponentInfo infoOf()
    {
        static_assert(std::is_nothrow_move_constructible_v<T>,
            "Components must have a noexcept move constructor");

        return {
            sizeof(T),
            alignof(T),
            [](void *dest, void *src) {
                T *obj = std::launder(static_cast<T *>(src));

                new (dest) T(std::move(*obj));
            },
            [](void *ptr) {
                std::launder(static_cast<T *>(ptr))->~T();
            }
        };
    }

    class ComponentRegistry
    {
    public:
        template<typename T>
        static ComponentId getId()
        {
            return idOf<std::remove_cvref_t<T>>();
        }

        static ComponentInfo getInfo(ComponentId id)
        {
            std::lock_guard lock(_mutex);

            return _infos.at(id.id);
        }
    private:
        template<typename T>
        static ComponentId idOf()
        {
            static const ComponentId id = registerType<T>();

            return id;
        }

        template<typename T>
        static ComponentId registerType()
        {
            std::lock_guard lock(_mutex);

            _infos.push_back(infoOf<T>());
            return {_infos.size() - 1};
        }

        inline static std::mutex _mutex;
        inline static std::vector<ComponentInfo> _infos;
    };
}
