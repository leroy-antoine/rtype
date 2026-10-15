#pragma once

#include <array>
#include <cstddef>

//  - distances en unites de la carte (1 unite = 1 pixel d'une carte 1920x1080)
//  - vitesses en unites par seconde
//  - durees en secondes
namespace rtype::config {
    constexpr float MAP_WIDTH = 1920.f;
    constexpr float MAP_HEIGHT = 1080.f;
    constexpr float MAP_ORIGIN = 0.f;
    // Les monstres apparaissent dans cette marge, a droite de la carte.
    constexpr float SPAWN_MARGIN = 100.f; // -> Spawn point des mobs, ils pop à droite de la carte
    constexpr float DESPAWN_MARGIN = 200.f; // -> Au delà de cette marge, les mobs et les projectiles depop
    constexpr bool PLAYERS_DIE_OUT_OF_MAP = true; // -> Si le joueur sort de la carte, il clamse

    constexpr float TICK_RATE = 60.f;
    constexpr float FIXED_DT = 1.f / TICK_RATE;

    constexpr std::size_t MAX_PLAYERS = 4;
    constexpr int PLAYER_HP = 100;
    constexpr float PLAYER_SPEED = 500.f;
    constexpr float PLAYER_WIDTH = 96.f;
    constexpr float PLAYER_HEIGHT = 48.f;
    constexpr int PLAYER_DAMAGE = 10;
    constexpr float PLAYER_START_X = 150.f;
    constexpr std::array<float, MAX_PLAYERS> PLAYER_START_Y = {
        270.f, 450.f, 630.f, 810.f // -> Hauteur de spawn des joueurs
    };

    constexpr float PLAYER_FIRE_COOLDOWN = 0.25f;
    constexpr float BULLET_SPEED = 900.f;
    constexpr float BULLET_WIDTH = 24.f;
    constexpr float BULLET_HEIGHT = 8.f;

    constexpr int MONSTER_HP = 30;
    constexpr float MONSTER_SPEED = 250.f;
    constexpr float MONSTER_WIDTH = 80.f;
    constexpr float MONSTER_HEIGHT = 80.f;
    constexpr int MONSTER_DAMAGE = 10;
    constexpr float MONSTER_FIRE_COOLDOWN = 2.f;

    constexpr int BOSS_HP = 1000;
    constexpr float BOSS_SPEED = 120.f;
    constexpr float BOSS_WIDTH = 400.f;
    constexpr float BOSS_HEIGHT = 400.f;
    constexpr int BOSS_DAMAGE = 20;
    constexpr float BOSS_FIRE_COOLDOWN = 0.8f;

    constexpr float SCROLL_SPEED = 120.f; //-> Vitesse de défilement de la carte
    constexpr float BOSS_SPAWN_SCROLL = 7200.f;  // -> Distance defilee (7200 / 120 = 60 s de jeu) a laquelle le boss arrive.
}
