#pragma once

#include <vector>
#include <limits>
#include <cassert>
#include <cstddef>
#include <utility>
#include "engine/core/Entity.hpp"
#include "engine/core/Column.hpp"
#include "engine/core/Component.hpp"
#include "engine/core/Signature.hpp"

namespace Engine {
    namespace detail {
        class Archetype
        {
        public:
            explicit Archetype(Signature sig) : _signature(std::move(sig))
            {
                _columnsIndex.assign(_signature.size(), NO_COLUMN);
                _columns.reserve(_signature.count());
                for (std::size_t i = 0; i < _signature.size(); ++i) {
                    if (_signature.test(i)) {
                        _columnsIndex[i] = _columns.size();
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
                return id.id < _columnsIndex.size() && _columnsIndex[id.id] != NO_COLUMN;
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
                    if (!_signature.test(i)) {
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
            static constexpr std::size_t NO_COLUMN = std::numeric_limits<std::size_t>::max();

            Signature _signature;
            std::vector<std::size_t> _columnsIndex;
            std::vector<Column> _columns;
            std::vector<Entity> _entities;
        };
    }
}
