#pragma once

#include <cassert>
#include <cstddef>

namespace Engine {
    class FixedTimestep
    {
    public:
        explicit FixedTimestep(float step, std::size_t maxSteps = 5) : _step(step), _maxSteps(maxSteps)
        {
            assert(step > 0 && "FixedTimestep: step must be positive");
            assert(maxSteps > 0 && "FixedTimestep: maxSteps must be positive");
        }

        std::size_t advance(float elapsed)
        {
            std::size_t steps = 0;

            _accumulator += elapsed;
            if (_accumulator > _step * _maxSteps) {
                _accumulator = _step * _maxSteps;
            }
            while (_accumulator >= _step) {
                _accumulator -= _step;
                ++steps;
            }
            return steps;
        }

        float step() const
        {
            return _step;
        }
    private:
        float _step;
        std::size_t _maxSteps;
        float _accumulator = 0;
    };
}
