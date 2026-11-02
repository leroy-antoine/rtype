#pragma once

#include <cstdint>
#include <vector>

namespace rtype {
    enum class Phase : std::uint8_t { Playing, Victory, Defeat };

    // Ce que le HUD d'un joueur affiche.
    struct PlayerStatus {
        int slot = 0;
        int hp = 0;
        bool alive = false;
    };

    struct GameStatus {
        Phase phase = Phase::Playing;
        int bossHp = 0;    // -> 0 si le boss n'est pas la
        int bossMaxHp = 0;
        std::uint32_t coins = 0;
        std::uint32_t elapsedMs = 0;
        std::vector<PlayerStatus> players;
    };
}
