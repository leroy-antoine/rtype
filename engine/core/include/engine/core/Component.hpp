#pragma once

#include <any>
#include <limits>
#include <bitset>
#include <vector>
#include <cassert>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <typeindex>
#include <type_traits>
#include "engine/core/Export.hpp"

namespace Engine {
    namespace detail {
        inline constexpr std::size_t MAX_COMPONENTS = 64;

        struct ComponentInfo {
            std::any (*makeStorage)();
            std::size_t (*size)(const std::any &data);
            void (*swapRemove)(std::any &data, std::size_t row);
            void (*moveRow)(std::any &src, std::size_t row, std::any &dst);
        };

        template<typename T>
        void swapRemove(std::vector<T> &vec, std::size_t row)
        {
            std::size_t last;

            assert(row < vec.size() && "swapRemove: row out of range");
            last = vec.size() - 1;
            if (row != last) {
                vec[row] = std::move(vec[last]);
            }
            vec.pop_back();
        }
    
        template<typename T>
        ComponentInfo infoOf()
        {
            static_assert(std::is_copy_constructible_v<T>,
                "Components must be copyable (std::any requirement)");
            static_assert(std::is_move_assignable_v<T>,
                "Components must be move assignable (swapRemove requirement)");

            return {
                []() -> std::any {
                    return std::vector<T>{};
                },
                [](const std::any &data) -> std::size_t {
                    return std::any_cast<const std::vector<T> &>(data).size();
                },
                [](std::any &data, std::size_t row) {
                    swapRemove(std::any_cast<std::vector<T> &>(data), row);
                },
                [](std::any &src, std::size_t row, std::any &dst) {
                    std::vector<T> &from = std::any_cast<std::vector<T> &>(src);
                    std::vector<T> &to = std::any_cast<std::vector<T> &>(dst);

                    to.push_back(std::move(from[row]));
                    swapRemove(from, row);
                }
            };
        }
    }

    using Signature = std::bitset<detail::MAX_COMPONENTS>;

    struct ComponentId {
        std::uint64_t id = std::numeric_limits<std::uint64_t>::max();

        bool operator==(const ComponentId &) const = default;
    };

    class ENGINE_CORE_EXPORT ComponentRegistry
    {
    public:
        template<typename T>
        static ComponentId getId()
        {
            return idOf<std::remove_cvref_t<T>>();
        }

        static detail::ComponentInfo getInfo(ComponentId id);
    private:
        template<typename T>
        static ComponentId idOf()
        {
            static const ComponentId id = registerType(typeid(T), detail::infoOf<T>());

            return id;
        }

        static ComponentId registerType(std::type_index type, detail::ComponentInfo info);
    };
}
