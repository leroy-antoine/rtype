#pragma once

#include <algorithm>
#include <cmath>
#include "Config.hpp"
#include "Waves.hpp"

namespace rtype {
    // Une vague peut garder la ligne de depart ou prendre une forme :
    // fleche (le chef en tete), mur vertical, ou serpent qui ondule.
    inline constexpr float FORMATION_SPACING = 120.f;
    inline constexpr float FORMATION_WALL_SPACING = 170.f;
    inline constexpr float FORMATION_SINE_AMPLITUDE = 220.f;
    inline constexpr float FORMATION_SINE_STEP = 0.8f;
    inline constexpr float FORMATION_DELAY = 0.3f;

    struct Slot {
        float delay;
        float y;
    };

    inline float formationDelay(const WaveStep &wave)
    {
        if (wave.interval > 0.f) {
            return wave.interval;
        }
        return FORMATION_DELAY;
    }

    // Ecart entre deux monstres, reduit pour que toute la formation tienne
    // dans la zone de jeu.
    inline float fittedSpacing(const WaveStep &wave, const float spacing)
    {
        const float range = config::WAVE_MAX_Y - config::WAVE_MIN_Y;
        const float gaps = static_cast<float>(std::max(1, wave.count - 1));

        return std::min(spacing, range / gaps);
    }

    inline float fittedBase(const float base, const float half)
    {
        return std::clamp(base, config::WAVE_MIN_Y + half,
            config::WAVE_MAX_Y - half);
    }

    inline Slot formationSlot(const WaveStep &wave, const int index,
        const float base)
    {
        const float n = static_cast<float>(index);
        const float middle = static_cast<float>(wave.count - 1) / 2.f;
        const float wedge = fittedSpacing(wave, FORMATION_SPACING);
        const float wall = fittedSpacing(wave, FORMATION_WALL_SPACING);
        Slot slot{0.f, base};

        if (wave.shape == Shape::Wedge) {
            slot.delay = std::abs(n - middle) * formationDelay(wave);
            slot.y = fittedBase(base, middle * wedge) + (n - middle) * wedge;
        } else if (wave.shape == Shape::Column) {
            slot.y = fittedBase(base, middle * wall) + (n - middle) * wall;
        } else {
            slot.delay = n * formationDelay(wave);
            slot.y = fittedBase(base, FORMATION_SINE_AMPLITUDE)
                + FORMATION_SINE_AMPLITUDE * std::sin(n * FORMATION_SINE_STEP);
        }
        return slot;
    }
}
