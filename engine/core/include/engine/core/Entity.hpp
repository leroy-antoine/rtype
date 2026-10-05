#pragma once

#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>

namespace Engine {
    struct Entity {
        std::uint64_t id = std::numeric_limits<std::uint64_t>::max();

        static const Entity null;

        bool operator==(const Entity &) const = default;
    };

    inline constexpr Entity Entity::null{};

    class EntityManager
    {
    public:
        EntityManager() = default;
        ~EntityManager() = default;

        static constexpr std::uint32_t getIndex(Entity e)
        {
            return static_cast<std::uint32_t>(e.id & 0xFFFFFFFF);
        }

        static constexpr std::uint32_t getGeneration(Entity e)
        {
            return static_cast<std::uint32_t>(e.id >> 32);
        }

        Entity create()
        {
            std::uint32_t index;
            std::uint32_t generation;

            if (_freeList.empty()) {
                if (_generations.size() >= std::numeric_limits<std::uint32_t>::max()) {
                    throw std::length_error("EntityManager: entity limit reached");
                }
                index = static_cast<std::uint32_t>(_generations.size());
                _generations.push_back(0);
            } else {
                index = _freeList.back();
                _freeList.pop_back();
            }
            generation = _generations[index];
            return {(static_cast<std::uint64_t>(generation) << 32) | index};
        }

        bool alive(Entity e) const
        {
            std::uint32_t index = getIndex(e);
            std::uint32_t generation = getGeneration(e);

            return (index <_generations.size() && _generations.at(index) == generation);
        }
        
        void destroy(Entity e)
        {
            std::uint32_t index;
            
            if (!alive(e)) {
                return;
            }
            index = getIndex(e);
            ++_generations.at(index);
            _freeList.push_back(index);
        }
    private:
        std::vector<std::uint32_t> _generations;
        std::vector<std::uint32_t> _freeList;
    };
}