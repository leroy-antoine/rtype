#pragma once

#include <any>
#include <cstddef>
#include <vector>
#include "engine/core/Component.hpp"

namespace Engine {
    namespace detail {
        class Column
        {
        public:
            explicit Column(ComponentInfo info) : _info(info), _data(info.makeStorage()) {}
        
            Column(const Column &) = delete;
            Column &operator=(const Column &) = delete;
            Column(Column &&) noexcept = default;
            Column &operator=(Column &&) noexcept = default;
        
            std::size_t size() const
            {
                return _info.size(_data);
            }
        
            void swapRemove(std::size_t row)
            {
                _info.swapRemove(_data, row);
            }
        
            void moveRowTo(std::size_t row, Column &dst)
            {
                _info.moveRow(_data, row, dst._data);
            }

            template<typename T>
            std::vector<T> &as()
            {
                return std::any_cast<std::vector<T> &>(_data);
            }
            
            template<typename T>
            const std::vector<T> &as() const
            {
                return std::any_cast<const std::vector<T> &>(_data);
            }
        private:
            ComponentInfo _info;
            std::any _data;
        };
    }
}
