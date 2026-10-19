#pragma once

#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace Engine {
    class FixedTimestep
    {
    public:
        explicit FixedTimestep(float step, std::size_t maxSteps = 5) : _step(step)
        {
            if (!std::isfinite(step) || step <= 0) {
                throw std::invalid_argument("FixedTimestep: step must be a positive finite number");
            }
            if (maxSteps == 0) {
                throw std::invalid_argument("FixedTimestep: maxSteps must be positive");
            }
            _maxAccumulator = _step * static_cast<float>(maxSteps);
        }

        std::size_t advance(float elapsed)
        {
            std::size_t steps = 0;

            _accumulator += elapsed;
            if (_accumulator > _maxAccumulator) {
                _accumulator = _maxAccumulator;
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

        float alpha() const
        {
            return _accumulator / _step;
        }
    private:
        float _step;
        float _maxAccumulator = 0;
        float _accumulator = 0;
    };
}
