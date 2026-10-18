#pragma once

#include <bit>
#include <vector>
#include <cstdint>
#include <cstddef>

namespace Engine {
    namespace detail {
        class Signature
        {
        public:
            void set(std::size_t bit)
            {
                std::size_t word = bit / WORD_BITS;

                if (word >= _words.size()) {
                    _words.resize(word + 1, 0);
                }
                _words[word] |= mask(bit);
            }

            void reset(std::size_t bit)
            {
                std::size_t word = bit / WORD_BITS;

                if (word >= _words.size()) {
                    return;
                }
                _words[word] &= ~mask(bit);
                while (!_words.empty() && _words.back() == 0) {
                    _words.pop_back();
                }
            }

            bool test(std::size_t bit) const
            {
                std::size_t word = bit / WORD_BITS;

                return word < _words.size() && (_words[word] & mask(bit)) != 0;
            }

            bool contains(const Signature &other) const
            {
                if (other._words.size() > _words.size()) {
                    return false;
                }
                for (std::size_t i = 0; i < other._words.size(); ++i) {
                    if ((_words[i] & other._words[i]) != other._words[i]) {
                        return false;
                    }
                }
                return true;
            }

            std::size_t count() const
            {
                std::size_t total = 0;

                for (std::uint64_t word : _words) {
                    total += static_cast<std::size_t>(std::popcount(word));
                }
                return total;
            }

            std::size_t size() const
            {
                return _words.size() * WORD_BITS;
            }

            bool operator==(const Signature &) const = default;
        private:
            static constexpr std::size_t WORD_BITS = 64;

            static std::uint64_t mask(std::size_t bit)
            {
                return std::uint64_t{1} << (bit % WORD_BITS);
            }

            std::vector<std::uint64_t> _words;
        };
    }
}
