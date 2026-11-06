#pragma once

#include <cstdint>
#include <vector>
#include "Settings.hpp"

namespace rtype {
    enum class Phase : std::uint8_t { Playing, Victory, Defeat, Lobby, Shop };

    // Ce que le HUD d'un joueur affiche.
    struct PlayerStatus {
        int slot = 0;
        int hp = 0;
        bool alive = false;
        bool ready = false;
        int lives = 0;
    };

    struct GameStatus {
        Phase phase = Phase::Playing;
        bool paused = false;
        int bossHp = 0;    // -> 0 si le boss n'est pas la
        int bossMaxHp = 0;
        std::uint32_t coins = 0;
        std::uint32_t elapsedMs = 0;
        std::uint8_t level = 0;
        std::uint8_t map = 0;
        std::uint16_t deaths = 0;
        Settings settings;
        std::vector<PlayerStatus> players;
        float countdown = 0.f;
    };
}
