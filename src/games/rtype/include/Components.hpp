#pragma once
namespace rtype {
    struct PlayerInput {
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
        bool fire = false;
    };

    struct Speed {
        float value = 100.f;
    };

    struct Health {
        int hp = 100;
    };

    struct Damage {
        int value = 10;
    };

    struct Weapon {
        float cooldown = 0.3f;
        float timer = 0.f;
    };

    struct PlayerTag {
        int slot = 0;
    };

    struct MonsterTag {};

    // Camp d'une entite : un projectile ne blesse que le camp adverse.
    enum class TeamId { Player, Monster };

    struct Team {
        TeamId id = TeamId::Player;
    };

    // Marque les projectiles : seules les entites avec Projectile + Damage infligent des degats au contact.
    struct Projectile {};
}

