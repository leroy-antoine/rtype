#pragma once

#include <array>
#include "Config.hpp"
#include "Kinds.hpp"

namespace rtype {
    // Une vague : count monstres lances quand le niveau a defile de at.
    // Le i-eme monstre apparait i * interval secondes apres le premier, a la
    // hauteur y + i * dy. Si y < 0, chaque monstre a sa propre hauteur tiree
    // au hasard (dy est alors ignore).
    // Tous les monstres de la vague sont du type `kind`.
    struct WaveStep {
        float at;
        int count;
        float y;
        float dy;
        float interval;
        MonsterKind kind = MonsterKind::Basic;
    };

    // Instant (en secondes de jeu) converti en distance defilee.
    constexpr float atSecond(const float seconds)
    {
        return seconds * config::SCROLL_SPEED;
    }

    constexpr float RANDOM_Y = -1.f;

    // Le boss arrive a BOSS_SPAWN_SCROLL (20 s).
    inline constexpr std::array<WaveStep, 4> WAVES = {{
        {atSecond(2.f),  4, RANDOM_Y, 0.f,   0.7f},
        {atSecond(7.f),  3, 300.f,    250.f, 0.f},
        {atSecond(11.f), 5, RANDOM_Y, 0.f,   0.5f},
        {atSecond(15.f), 4, 200.f,    220.f, 0.f},
    }};
}
