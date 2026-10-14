#pragma once

#include <vector>
#include <cassert>
#include <cstddef>
#include <utility>
#include <type_traits>
#include "engine/core/Entity.hpp"
#include "engine/core/Archetype.hpp"
#include "engine/core/Component.hpp"
#include "engine/core/EntityManager.hpp"

namespace Engine {
    class Registry
    {
    public:
        Registry()
        {
            _archetypes.emplace_back(detail::Signature{});
        }

        Registry(const Registry &) = delete;
        Registry &operator=(const Registry &) = delete;
        Registry(Registry &&) noexcept = default;
        Registry &operator=(Registry &&) noexcept = default;

        Entity create()
        {
            Entity e;
            std::size_t row;

            assert(_iterating == 0 && "create: structural change during each");
            e = _entities.create();
            row = _archetypes[EMPTY_ARCHETYPE].addEntity(e);
            if (e.getIndex() >= _records.size()) {
                _records.resize(e.getIndex() + 1);
            }
            _records[e.getIndex()] = {EMPTY_ARCHETYPE, row};
            return e;
        }

        bool alive(Entity e) const
        {
            return _entities.alive(e);
        }

        void destroy(Entity e)
        {
            EntityRecord record;
            Entity moved;

            assert(_iterating == 0 && "destroy: structural change during each");
            if (!alive(e)) {
                return;
            }
            record = _records[e.getIndex()];
            moved = _archetypes[record.archetype].swapRemove(record.row);
            if (moved != Entity{}) {
                _records[moved.getIndex()].row = record.row;
            }
            _entities.destroy(e);
        }

        template<typename T>
        T &add(Entity e, T component = {})
        {
            detail::ComponentId id = detail::ComponentRegistry::getId<T>();
            detail::Signature sig;
            std::size_t dst;

            assert(_iterating == 0 && "add: structural change during each");
            assert(alive(e) && "add: dead entity");
            if (has<T>(e)) {
                T &slot = get<T>(e);

                slot = std::move(component);
                return slot;
            }
            sig = _archetypes[_records[e.getIndex()].archetype].signature();
            sig.set(id.id);
            dst = getOrCreateArchetype(sig);
            moveEntity(e, dst);
            std::vector<T> &column = _archetypes[dst].getColumn<T>();

            column.push_back(std::move(component));
            return column.back();
        }

        template<typename T>
        void remove(Entity e)
        {
            detail::ComponentId id = detail::ComponentRegistry::getId<T>();
            detail::Signature sig;

            assert(_iterating == 0 && "remove: structural change during each");
            if (!has<T>(e)) {
                return;
            }
            sig = _archetypes[_records[e.getIndex()].archetype].signature();
            sig.reset(id.id);
            moveEntity(e, getOrCreateArchetype(sig));
        }

        template<typename T>
        bool has(Entity e) const
        {
            if (!alive(e)) {
                return false;
            }
            return _archetypes[_records[e.getIndex()].archetype].has(detail::ComponentRegistry::getId<T>());
        }

        template<typename T>
        T &get(Entity e)
        {
            const EntityRecord &record = _records[e.getIndex()];

            assert(has<T>(e) && "get: entity does not have this component");
            return _archetypes[record.archetype].getColumn<T>()[record.row];
        }

        template<typename T>
        const T &get(Entity e) const
        {
            const EntityRecord &record = _records[e.getIndex()];

            assert(has<T>(e) && "get: entity does not have this component");
            return _archetypes[record.archetype].getColumn<T>()[record.row];
        }

        template<typename... Ts, typename Func>
        void each(Func &&func)
        {
            detail::Signature required;
            IterationGuard guard(_iterating);

            (required.set(detail::ComponentRegistry::getId<Ts>().id), ...);
            for (detail::Archetype &arch : _archetypes) {
                if (arch.size() == 0 || (arch.signature() & required) != required) {
                    continue;
                }
                eachRow(arch.entities(), func, arch.getColumn<std::remove_cvref_t<Ts>>()...);
            }
        }
    private:
        struct EntityRecord {
            std::size_t archetype = 0;
            std::size_t row = 0;
        };

        struct IterationGuard {
            explicit IterationGuard(std::size_t &depth) : _depth(depth)
            {
                ++_depth;
            }

            ~IterationGuard()
            {
                --_depth;
            }

            IterationGuard(const IterationGuard &) = delete;
            IterationGuard &operator=(const IterationGuard &) = delete;
        private:
            std::size_t &_depth;
        };

        static constexpr std::size_t EMPTY_ARCHETYPE = 0;

        template<typename Func, typename... Columns>
        static void eachRow(const std::vector<Entity> &entities, Func &func, Columns &...columns)
        {
            for (std::size_t row = 0; row < entities.size(); ++row) {
                func(entities[row], columns[row]...);
            }
        }

        std::size_t getOrCreateArchetype(const detail::Signature &sig)
        {
            for (std::size_t i = 0; i < _archetypes.size(); ++i) {
                if (_archetypes[i].signature() == sig) {
                    return i;
                }
            }
            _archetypes.emplace_back(sig);
            return _archetypes.size() - 1;
        }

        void moveEntity(Entity e, std::size_t dst)
        {
            EntityRecord &record = _records[e.getIndex()];
            Entity moved = _archetypes[record.archetype].moveRowTo(record.row, _archetypes[dst]);

            if (moved != Entity{}) {
                _records[moved.getIndex()].row = record.row;
            }
            record = {dst, _archetypes[dst].size() - 1};
        }

        detail::EntityManager _entities;
        std::vector<detail::Archetype> _archetypes;
        std::vector<EntityRecord> _records;
        std::size_t _iterating = 0;
    };
}
