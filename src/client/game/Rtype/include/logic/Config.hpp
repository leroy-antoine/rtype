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
    constexpr bool PLAYERS_INVINCIBLE = false;
    constexpr bool PLAYERS_DIE_OUT_OF_MAP = true; // -> Si le joueur sort de la carte, il clamse

    constexpr float TICK_RATE = 60.f;
    constexpr float FIXED_DT = 1.f / TICK_RATE;

    constexpr std::size_t MAX_PLAYERS = 4;
    // Delai avant le retour d'un joueur mort.
    constexpr float RESPAWN_DELAY = 3.f;
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
    // Tir charge : en dessous de CHARGE_TAP_LIMIT un appui tire un coup
    // normal ; au-dela la jauge monte jusqu'a CHARGE_FULL_TIME.
    constexpr float CHARGE_TAP_LIMIT = 0.3f;
    constexpr float CHARGE_FULL_TIME = 1.2f;
    constexpr float CHARGE_MIN_FACTOR = 2.f;
    constexpr float CHARGE_MAX_FACTOR = 6.f;
    constexpr float CHARGE_MIN_SIZE = 2.f;
    constexpr float CHARGE_MAX_SIZE = 4.f;
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
    constexpr float BOSS_WIDTH = 336.f;
    constexpr float BOSS_HEIGHT = 336.f;
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
    constexpr float SUMMON_INTERVAL = 3.5f;
    constexpr int SUMMON_COUNT = 4;
    constexpr int SUMMON_MAX_ALIVE = 12;
    constexpr float SUMMON_SPACING = 110.f;
    constexpr float LEVIATHAN_BURST_PERIOD = 7.f;
    constexpr float LEVIATHAN_BURST_DURATION = 2.f;
    constexpr float LEVIATHAN_BURST_COOLDOWN = 0.35f;
    constexpr std::size_t ESCORT_MAX = 3;
    constexpr float ESCORT_DELAY = 1.f;
    constexpr int MINI_HP_FACTOR = 3;
    constexpr float MINI_SIZE_FACTOR = 1.6f;
    constexpr float MINI_DAMAGE_FACTOR = 1.5f;
    constexpr float MINI_COOLDOWN_FACTOR = 0.7f;
    constexpr float LEVIATHAN_DASH_MAX = 2.f;
    constexpr float LEVIATHAN_RETURN_MAX = 4.f;
    constexpr float LEVIATHAN_MARGIN_Y = 40.f;
    constexpr float LEVIATHAN_CORRECTION = 6.f;
    constexpr float LEVIATHAN_AIM_SPEED = 900.f;
    constexpr float LEVIATHAN_AIM_TIME = 0.45f;
    constexpr float LEVIATHAN_WINDUP = 0.7f;
    constexpr float LEVIATHAN_DASH_SPEED = 1700.f;
    constexpr float LEVIATHAN_RETURN_SPEED = 450.f;
    constexpr float LEVIATHAN_DASH_END_X = 220.f;
    constexpr float LEVIATHAN_SWAY[3] = {1.1f, 1.4f, 1.8f};
    constexpr float LEVIATHAN_REST[3] = {1.8f, 1.2f, 0.6f};
    constexpr float LEVIATHAN_WARN[3] = {1.1f, 1.0f, 0.9f};
    constexpr float LEVIATHAN_SWEEP_TIME[3] = {2.8f, 2.4f, 2.0f};
    constexpr float COLUMN_WIDTH = 120.f;
    constexpr float COLUMN_ACTIVE = 0.4f;
    constexpr float COLUMN_FIRST = 150.f;
    constexpr float COLUMN_RANGE = 1600.f;
    constexpr float SWEEP_GAP = 280.f;
    constexpr float SWEEP_MARGIN = 60.f;
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

    // Les prix de base sont ceux d'une partie a 4 joueurs ; moins il y a de
    // joueurs, moins la caisse commune se remplit et moins c'est cher.
    constexpr std::array<float, MAX_PLAYERS> PRICE_BY_PLAYERS = {
        0.5f, 0.7f, 0.85f, 1.f};
    constexpr std::size_t AUGMENT_DAMAGE_PRICE = 40;
    constexpr std::size_t AUGMENT_SPEED_PRICE = 40;
    constexpr std::size_t AUGMENT_HP_PRICE = 75;
    constexpr std::size_t AUGMENT_FIRE_RATE_PRICE = 150;
    constexpr std::size_t AUGMENT_REGENERATION_PRICE = 110;
    constexpr std::size_t AUGMENT_SHIELD_PRICE = 190;
    constexpr std::size_t AUGMENT_MULTISHOT_PRICE = 300;
    constexpr std::size_t AUGMENT_ROCKET_PRICE = 260;
    constexpr std::size_t AUGMENT_LASER_PRICE = 340;
    constexpr std::size_t AUGMENT_LIGHTNING_PRICE = 340;
    constexpr float ROCKET_COOLDOWN = 1.2f;
    constexpr float ROCKET_SPEED = 450.f;
    constexpr float ROCKET_WIDTH = 30.f;
    constexpr float ROCKET_HEIGHT = 12.f;
    constexpr float ROCKET_RADIUS = 120.f;
    constexpr float ROCKET_FUSE = 60.f;
    constexpr int ROCKET_DAMAGE_FACTOR = 3;
    constexpr float LASER_CHARGE_TIME = 2.f;
    constexpr float LASER_COOLDOWN = 6.f;
    constexpr float LASER_HEIGHT = 40.f;
    constexpr float LASER_DURATION = 0.4f;
    constexpr int LASER_DAMAGE_FACTOR = 8;
    constexpr std::size_t AUGMENT_DRONE_PRICE = 225;
    constexpr std::size_t AUGMENT_DRONE_BOOST_PRICE = 300;
    constexpr std::size_t AUGMENT_DRONE_TWIN_PRICE = 375;
    constexpr int DRONE_MAX_LEVEL = 3;
    constexpr float DRONE_OFFSET_X = 60.f;
    constexpr float DRONE_OFFSET_Y = 50.f;
    constexpr float DRONE_FOLLOW = 12.f;
    constexpr float DRONE_SIZE = 24.f;
    constexpr float DRONE_RANGE = MAP_WIDTH;
    constexpr int DRONE_DAMAGE_DIVISOR = 2;
    constexpr float DRONE_SPREAD = 0.08f;
    constexpr float LIGHTNING_COOLDOWN = 0.8f;
    constexpr float LIGHTNING_RANGE = 600.f;
    constexpr float LIGHTNING_LANE = 40.f;
    constexpr float LIGHTNING_JUMP = 100.f;
    constexpr int LIGHTNING_JUMPS = 4;
    constexpr float LIGHTNING_DAMAGE_FACTOR = 1.5f;
    constexpr float LIGHTNING_FALLOFF = 0.8f;
    constexpr float LIGHTNING_DURATION = 0.15f;
    constexpr float LIGHTNING_MIN_SIZE = 6.f;

    constexpr int MULTISHOT_BULLETS = 3;
    constexpr int MULTISHOT_EXTRA_BULLETS = 2;
    constexpr float MULTISHOT_SPREAD = 0.15f;
    constexpr float SHIELD_COOLDOWN = 8.f;
    constexpr float SHIELD_COOLDOWN_FACTOR = 0.75f;
    constexpr int REGENERATION_HP = 5;
    constexpr float REGENERATION_INTERVAL = 2.f;

    constexpr int COMMON_WEIGHT = 60;
    constexpr int COMMON_WEIGHT_PER_LEVEL = -2;
    constexpr int COMMON_BOSS_BONUS = -20;
    constexpr int RARE_WEIGHT = 25;
    constexpr int RARE_WEIGHT_PER_LEVEL = 1;
    constexpr int RARE_BOSS_BONUS = 5;
    constexpr int EPIC_WEIGHT = 12;
    constexpr int EPIC_WEIGHT_PER_LEVEL = 1;
    constexpr int EPIC_BOSS_BONUS = 10;
    constexpr int LEGENDARY_WEIGHT = 3;
    constexpr int LEGENDARY_WEIGHT_PER_LEVEL = 1;
    constexpr int LEGENDARY_BOSS_BONUS = 5;
    constexpr int MIN_RARITY_WEIGHT = 1;
    constexpr std::size_t SHOP_OFFER_COUNT = 3;
    constexpr float SHOP_COUNTDOWN = 5.f;

    constexpr float OBSTACLE_TILE = 64.f;
    constexpr float PORTAL_WIDTH = 90.f;
    constexpr float SWEEPER_WIDTH = 90.f;
    constexpr float FAST_SPEED_FACTOR = 1.5f;
    constexpr float SPEED_PERCENT = 100.f;
    constexpr float SCROLL_SPEED = 120.f; //-> Vitesse de défilement de la carte
    constexpr float OBSTACLE_SPEED = SCROLL_SPEED;
    // Death runs a 120 BPM : un temps dure 0,5 s.
    constexpr float BEAT = 0.5f;
    constexpr float LASER_PERIOD = 4.f * BEAT;
    constexpr float LASER_OFF = 0.6f;
    constexpr float LASER_WARN = 0.4f;
    constexpr float LASER_WIDTH = 70.f;
    constexpr float TURRET_INTERVAL = 2.f * BEAT;
    constexpr float TURRET_SHOT_SPEED = 380.f;
    constexpr float TURRET_SHOT_SIZE = 26.f;
    constexpr float TURRET_MUZZLE = 64.f;      // -> Distance du centre de la tourelle ou le tir apparait (hors du bloc)
    constexpr int TURRET_DAMAGE = 15;
    constexpr float MINE_SIZE = 56.f;
    constexpr int SPAWN_SEARCH = 8;          // -> Essais au-dessus et en dessous pour trouver un passage libre
    constexpr float SPAWN_STEP = 60.f;
    constexpr float SPAWN_RETRY = 0.1f;
    constexpr float METEOR_SIZE = 30.f;
    constexpr float METEOR_SPEED = 760.f;
    constexpr int METEOR_DAMAGE = 20;
    constexpr float HAZARD_LASER_SLOW = 8.f;  // -> Delai entre deux lasers de boss (premier boss)
    constexpr float HAZARD_LASER_FAST = 3.2f; // -> ... et pour le dernier boss
    constexpr float HAZARD_METEOR_SLOW = 2.f;
    constexpr float HAZARD_METEOR_FAST = 0.55f;
    constexpr float RESTART_LEAD = 800.f;  // -> x du premier obstacle apres une mort en death run
    constexpr float BOSS_SPAWN_SCROLL = 2400.f;  // -> Distance defilee (2400 / 120 = 20 s de jeu) a laquelle le boss arrive.

    constexpr std::size_t COIN_MIN_VALUE = 1;
    constexpr std::size_t COIN_MAX_VALUE = 15;
    constexpr float COIN_WIDTH = 32.f;
    constexpr float COIN_HEIGHT = 32.f;
}
