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
    constexpr bool PLAYERS_INVINCIBLE = true;
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

    constexpr float EXPLOSION_DURATION = 0.4f;
    constexpr float HIT_EXPLOSION_SIZE = 48.f;

    constexpr int MONSTER_HP = 30;
    constexpr float MONSTER_SPEED = 250.f;
    constexpr float MONSTER_WIDTH = 80.f;
    constexpr float MONSTER_HEIGHT = 80.f;
    constexpr int MONSTER_DAMAGE = 10;
    constexpr float MONSTER_FIRE_COOLDOWN = 2.f;
    constexpr float MONSTER_MAX_SPAWN_Y = MAP_HEIGHT - MONSTER_HEIGHT;

    constexpr int BOSS_HP = 600;
    constexpr float BOSS_SPEED = 120.f;        // -> Vitesse d'entree
    constexpr float BOSS_WIDTH = 480.f;
    constexpr float BOSS_HEIGHT = 264.f;
    constexpr int BOSS_DAMAGE = 20;
    constexpr float BOSS_FIRE_COOLDOWN = 0.9f;
    constexpr float DASHER_MIN_X = 700.f;
    constexpr float DASHER_MARGIN_Y = 60.f;
    constexpr float DASHER_FREQUENCY_X = 0.9f;
    constexpr float DASHER_FREQUENCY_Y = 1.4f;
    constexpr float DASHER_CORRECTION = 6.f;
    constexpr float ZIGZAG_AMPLITUDE = 160.f;
    constexpr float ZIGZAG_FREQUENCY = 2.2f;
    constexpr float DIVE_DISTANCE = 900.f;
    constexpr float ROCKET_CHARGE_TIME = 0.8f;
    constexpr float SUMMON_INTERVAL = 5.f;
    constexpr int SUMMON_COUNT = 3;
    constexpr int SUMMON_MAX_ALIVE = 6;
    constexpr float SUMMON_SPACING = 110.f;
    constexpr float FORTRESS_SWAY_AMPLITUDE = 300.f;
    constexpr float FORTRESS_SWAY_FREQUENCY = 0.5f;
    constexpr float FORTRESS_BURST_PERIOD = 7.f;
    constexpr float FORTRESS_BURST_DURATION = 2.f;
    constexpr float FORTRESS_BURST_COOLDOWN = 0.35f;
    constexpr float KILL_SKIP_SECONDS = 0.5f;
    constexpr float KILL_SKIP_MAX_FRACTION = 0.4f;
    constexpr float BOSS_RIGHT_MARGIN = 60.f;
    constexpr float BOSS_EXPLOSION_MIN_SIZE = 200.f;
    constexpr float BOSS_STOP_X =
        MAP_WIDTH - BOSS_WIDTH - BOSS_RIGHT_MARGIN; // -> Il s'arrete la
    constexpr float BOSS_SWAY_AMPLITUDE = 280.f; // -> Va-et-vient vertical
    constexpr float BOSS_SWAY_FREQUENCY = 0.8f;
    constexpr int BOSS_FAN_BULLETS = 5;        // -> Balles par tir (eventail)
    constexpr float BOSS_FAN_ANGLE = 0.3f;     // -> Ecart (rad) entre 2 balles
    constexpr float BOSS_BULLET_SPEED = 480.f;
    constexpr float BOSS_BULLET_SIZE = 40.f;

    constexpr float MS_PER_SECOND = 1000.f;
    constexpr float RESTART_DELAY = 6.f; // -> Secondes avant la nouvelle partie

    // Hauteur (haut du monstre) tiree au hasard pour une vague.
    constexpr float WAVE_MIN_Y = 100.f;
    constexpr float WAVE_MAX_Y = MAP_HEIGHT - 300.f;

    constexpr int AUGMENT_DAMAGE_BONUS = 5;
    constexpr float AUGMENT_FIRE_COOLDOWN_FACTOR = 0.8f;
    constexpr float AUGMENT_SPEED_BONUS = 75.f;
    constexpr int AUGMENT_HP_BONUS = 25;

    constexpr int AUGMENT_DAMAGE_PRICE = 50;
    constexpr int AUGMENT_SPEED_PRICE = 50;
    constexpr int AUGMENT_HP_PRICE = 100;
    constexpr int AUGMENT_FIRE_RATE_PRICE = 200;

    constexpr float OBSTACLE_TILE = 64.f;
    constexpr float SCROLL_SPEED = 120.f; //-> Vitesse de défilement de la carte
    constexpr float OBSTACLE_SPEED = SCROLL_SPEED;
    constexpr float BOSS_SPAWN_SCROLL = 2400.f;  // -> Distance defilee (2400 / 120 = 20 s de jeu) a laquelle le boss arrive.

    constexpr std::size_t COIN_MIN_VALUE = 1;
    constexpr std::size_t COIN_MAX_VALUE = 15;
    constexpr float COIN_WIDTH = 32.f;
    constexpr float COIN_HEIGHT = 32.f;
}
