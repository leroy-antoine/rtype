#pragma once

#include <algorithm>
#include <array>
#include <vector>
#include "Config.hpp"

namespace rtype {
    // Les vies restantes de chaque joueur et le delai avant son retour.
    class LifeBook {
    public:
        static constexpr int SLOTS = static_cast<int>(config::MAX_PLAYERS);

        void refill(const int lives)
        {
            _lives.fill(lives);
            _down.fill(false);
            _timers.fill(0.f);
        }

        void setLives(const int slot, const int lives)
        {
            _lives[index(slot)] = lives;
        }

        [[nodiscard]] int lives(const int slot) const
        {
            return _lives[index(slot)];
        }

        [[nodiscard]] bool isDown(const int slot) const
        {
            return _down[index(slot)];
        }

        void lose(const int slot)
        {
            _lives[index(slot)] = std::max(0, lives(slot) - 1);
            _down[index(slot)] = true;
            _timers[index(slot)] = config::RESPAWN_DELAY;
        }

        void revive(const int slot)
        {
            _down[index(slot)] = false;
            _timers[index(slot)] = 0.f;
        }

        void grantOne(const int slot)
        {
            _lives[index(slot)] = std::max(1, lives(slot));
        }

        // Les joueurs dont le delai est ecoule et qui ont encore des vies.
        std::vector<int> tick(const float dt)
        {
            std::vector<int> ready;

            for (int slot = 0; slot < SLOTS; ++slot) {
                if (isDown(slot) && lives(slot) > 0 && countDown(slot, dt)) {
                    ready.push_back(slot);
                }
            }
            return ready;
        }

    private:
        static std::size_t index(const int slot)
        {
            return static_cast<std::size_t>(slot);
        }

        bool countDown(const int slot, const float dt)
        {
            float &timer = _timers[index(slot)];

            timer -= dt;
            return timer <= 0.f;
        }

        std::array<int, SLOTS> _lives{};
        std::array<bool, SLOTS> _down{};
        std::array<float, SLOTS> _timers{};
    };
}
