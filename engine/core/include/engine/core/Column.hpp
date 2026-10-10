#pragma once

#include <cassert>
#include <cstddef>
#include <new>
#include <utility>
#include "Component.hpp"

namespace Engine {
    class Column
    {
    public:
        explicit Column(ComponentInfo info) : _info(info) {}

        ~Column()
        {
            release();
        }

        Column(const Column &) = delete;
        Column &operator=(const Column &) = delete;

        Column(Column &&other) noexcept
            : _info(other._info),
              _data(std::exchange(other._data, nullptr)),
              _count(std::exchange(other._count, 0)),
              _capacity(std::exchange(other._capacity, 0))
        {
        }

        Column &operator=(Column &&other) noexcept
        {
            if (this != &other) {
                release();
                _info = other._info;
                _data = std::exchange(other._data, nullptr);
                _count = std::exchange(other._count, 0);
                _capacity = std::exchange(other._capacity, 0);
            }
            return *this;
        }

        std::size_t size() const
        {
            return _count;
        }

        const ComponentInfo &info() const
        {
            return _info;
        }

        void *get(std::size_t row)
        {
            assert(row < _count && "Column::get: row out of range");
            return _data + row * _info.size;
        }

        const void *get(std::size_t row) const
        {
            assert(row < _count && "Column::get: row out of range");
            return _data + row * _info.size;
        }

        void pushMove(void *src)
        {
            if (_count == _capacity) {
                grow();
            }
            _info.move(_data + _count * _info.size, src);
            ++_count;
        }

        void swapRemove(std::size_t row)
        {
            std::size_t last;

            assert(row < _count && "Column::swapRemove: row out of range");
            last = _count - 1;
            _info.destroy(get(row));
            if (row != last) {
                _info.move(get(row), get(last));
                _info.destroy(get(last));
            }
            --_count;
        }
    private:
        static constexpr std::size_t MIN_CAPACITY = 8;

        void grow()
        {
            std::size_t newCapacity = _capacity == 0 ? MIN_CAPACITY : _capacity * 2;
            std::byte *newData = static_cast<std::byte *>(
                ::operator new(newCapacity * _info.size, std::align_val_t{_info.align}));

            for (std::size_t i = 0; i < _count; ++i) {
                _info.move(newData + i * _info.size, _data + i * _info.size);
                _info.destroy(_data + i * _info.size);
            }
            if (_data != nullptr) {
                ::operator delete(_data, std::align_val_t{_info.align});
            }
            _data = newData;
            _capacity = newCapacity;
        }

        void release() noexcept
        {
            for (std::size_t i = 0; i < _count; ++i) {
                _info.destroy(_data + i * _info.size);
            }
            if (_data != nullptr) {
                ::operator delete(_data, std::align_val_t{_info.align});
            }
            _data = nullptr;
            _count = 0;
            _capacity = 0;
        }

        ComponentInfo _info;
        std::byte *_data = nullptr;
        std::size_t _count = 0;
        std::size_t _capacity = 0;
    };
}
