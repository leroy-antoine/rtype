#pragma once
#include "Config.hpp"

namespace rtype {
    struct PlayerInput {
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
        bool fire = false;
    };

    struct Speed {
        float value = config::PLAYER_SPEED;
    };

    struct Health {
        int hp = config::PLAYER_HP;
    };

    struct Damage {
        int value = config::PLAYER_DAMAGE;
    };

    struct Weapon {
        float cooldown = config::PLAYER_FIRE_COOLDOWN;
        float timer = 0.f;
    };

    struct PlayerTag {
        int slot = 0;
    };

    struct MonsterTag {
        int slot = 0;
    };

    struct ProjectileTag {
    };

    enum class TeamId { Player, Monster };

    struct Team {
        TeamId id = TeamId::Player;
    };

    enum class SpriteId { Player, Monster, PlayerBullet, MonsterBullet, Count };

    struct Renderable {
        SpriteId id = SpriteId::Player;
    };
}
