#pragma once

#include <cstdint>
#include <limits>
#include <cassert>
#include <vector>

namespace Engine {
    struct Entity {
        std::uint64_t id = std::numeric_limits<std::uint64_t>::max();

        bool operator==(const Entity &) const = default;

        std::uint32_t getIndex() const
        {
            return static_cast<std::uint32_t>(this->id & 0xFFFFFFFF);
        }

        std::uint32_t getGeneration() const
        {
            return static_cast<std::uint32_t>(this->id >> 32);
        }
    };

    class EntityManager
    {
    public:
        Entity create()
        {
            std::uint32_t index;
            std::uint32_t generation;

            if (_freeList.empty()) {
                assert(_generations.size() < std::numeric_limits<std::uint32_t>::max());
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
            std::uint32_t index = e.getIndex();
            std::uint32_t generation = e.getGeneration();

            return (index <_generations.size() && _generations[index] == generation);
        }
        
        void destroy(Entity e)
        {
            std::uint32_t index;
            
            if (!alive(e)) {
                return;
            }
            index = e.getIndex();
            ++_generations[index];
            _freeList.push_back(index);
        }
    private:
        std::vector<std::uint32_t> _generations;
        std::vector<std::uint32_t> _freeList;
    };
}
