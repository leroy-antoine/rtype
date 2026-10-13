#pragma once

#include <array>
#include <vector>
#include <limits>
#include <cassert>
#include <cstdint>
#include <cstddef>
#include "engine/core/Entity.hpp"
#include "engine/core/Column.hpp"
#include "engine/core/Component.hpp"

namespace Engine {
    namespace detail {
        class Archetype
        {
        public:
            explicit Archetype(Signature sig) : _signature(sig)
            {
                _columnsIndex.fill(NO_COLUMN);
                _columns.reserve(sig.count());
                for (std::size_t i = 0; i < sig.size(); ++i) {
                    if (sig[i]) {
                        _columnsIndex[i] = static_cast<std::uint8_t>(_columns.size());
                        _columns.emplace_back(ComponentRegistry::getInfo(ComponentId{i}));
                    }
                }
            }

            Archetype(const Archetype &) = delete;
            Archetype &operator=(const Archetype &) = delete;
            Archetype(Archetype &&) noexcept = default;
            Archetype &operator=(Archetype &&) noexcept = default;

            const Signature &signature() const
            {
                return _signature;
            }

            std::size_t size() const
            {
                return _entities.size();
            }

            bool isConsistent() const
            {
                if (_columns.size() != _signature.count()) {
                    return false;
                }
                for (const Column &col : _columns) {
                    if (col.size() != size()) {
                        return false;
                    }
                }
                return true;
            }

            const std::vector<Entity> &entities() const
            {
                return _entities;
            }

            bool has(ComponentId id) const
            {
                assert(id.id < MAX_COMPONENTS && "has: invalid component id");
                return _columnsIndex[id.id] != NO_COLUMN;
            }

            Column &column(ComponentId id)
            {
                assert(has(id) && "column: component not in archetype");
                return _columns[_columnsIndex[id.id]];
            }

            const Column &column(ComponentId id) const
            {
                assert(has(id) && "column: component not in archetype");
                return _columns[_columnsIndex[id.id]];
            }

            template<typename T>
            std::vector<T> &getColumn()
            {
                ComponentId id = ComponentRegistry::getId<T>();

                return column(id).as<T>();
            }

            template<typename T>
            const std::vector<T> &getColumn() const
            {
                ComponentId id = ComponentRegistry::getId<T>();

                return column(id).as<T>();
            }

            std::size_t addEntity(Entity e)
            {
                _entities.push_back(e);
                return _entities.size() - 1;
            }

            Entity swapRemove(std::size_t row)
            {
                assert(isConsistent() && "swapRemove: archetype has an incomplete row");
                assert(row < size() && "swapRemove: row out of range");
                for (Column &col : _columns) {
                    col.swapRemove(row);
                }
                detail::swapRemove(_entities, row);
                return row < size() ? _entities[row] : Entity{};
            }

            Entity moveRowTo(std::size_t row, Archetype &dst)
            {
                ComponentId id;

                assert(&dst != this && "moveRowTo: source and destination are the same archetype");
                assert(isConsistent() && "moveRowTo: source archetype has an incomplete row");
                assert(dst.isConsistent() && "moveRowTo: destination archetype has an incomplete row");
                assert(row < size() && "moveRowTo: row out of range");
                for (std::size_t i = 0; i < _signature.size(); ++i) {
                    if (!_signature[i]) {
                        continue;
                    }
                    id = {i};
                    if (dst.has(id)) {
                        column(id).moveRowTo(row, dst.column(id));
                    } else {
                        column(id).swapRemove(row);
                    }
                }
                dst._entities.push_back(_entities[row]);
                detail::swapRemove(_entities, row);
                return row < size() ? _entities[row] : Entity{};
            }
        private:
            static constexpr std::uint8_t NO_COLUMN = std::numeric_limits<std::uint8_t>::max();

            Signature _signature;
            std::array<std::uint8_t, MAX_COMPONENTS> _columnsIndex;
            std::vector<Column> _columns;
            std::vector<Entity> _entities;
        };
    }
}
