#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

namespace rtype {
    enum class Difficulty : std::uint8_t { Easy, Normal, Hard, Brutal };

    constexpr std::size_t DIFFICULTY_COUNT = 4;

    // Multiplicateurs appliques par un niveau de difficulte.
    struct DifficultyScale {
        float economy;
        float enemyHp;
        float enemyDamage;
        float allyDamage;
        float enemyFireRate;
        float density;
        int lives;
        int shopLevelBonus;
        int extraOffers;
    };

    using ScaleTable = std::array<DifficultyScale, DIFFICULTY_COUNT>;

    constexpr ScaleTable DIFFICULTY_SCALES = {{
        {1.25f, 0.75f, 0.75f, 1.25f, 0.75f, 0.75f, 5, -1, 0},
        {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 3, 0, 0},
        {0.85f, 1.5f, 1.25f, 0.85f, 1.25f, 1.25f, 2, 1, 0},
        {0.75f, 2.f, 1.5f, 0.75f, 1.5f, 1.5f, 1, 2, 1},
    }};

    inline constexpr std::array<const char *, DIFFICULTY_COUNT>
        DIFFICULTY_NAMES = {"EASY", "NORMAL", "HARD", "BRUTAL"};

    // L'ordre d'affichage des classements : du plus dur au plus facile.
    inline constexpr std::array<Difficulty, DIFFICULTY_COUNT>
        DIFFICULTY_BY_RANK = {Difficulty::Brutal, Difficulty::Hard,
            Difficulty::Normal, Difficulty::Easy};

    struct Settings {
        Difficulty difficulty = Difficulty::Normal;
        bool friendlyFire = false;
    };

    constexpr bool isValidDifficulty(const std::uint8_t value)
    {
        return value < DIFFICULTY_COUNT;
    }

    // La difficulte voisine, en restant entre Facile et Brutal.
    constexpr Difficulty shiftedDifficulty(const Difficulty difficulty,
        const int delta)
    {
        const int last = static_cast<int>(DIFFICULTY_COUNT) - 1;
        const int next = static_cast<int>(difficulty) + delta;

        return static_cast<Difficulty>(std::clamp(next, 0, last));
    }

    constexpr const DifficultyScale &scaleOf(const Difficulty difficulty)
    {
        return DIFFICULTY_SCALES[static_cast<std::size_t>(difficulty)];
    }

    constexpr const DifficultyScale &scaleOf(const Settings &settings)
    {
        return scaleOf(settings.difficulty);
    }
}
