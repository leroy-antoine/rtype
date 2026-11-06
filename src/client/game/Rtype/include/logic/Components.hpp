#pragma once
#include <cstdint>
#include <vector>
#include "engine/core/Entity.hpp"
#include "Config.hpp"
#include "Kinds.hpp"

namespace rtype {
    struct PlayerInput {
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
        bool fire = false;
        bool alt = false;
    };

    struct Speed {
        float value = config::PLAYER_SPEED;
    };

    struct Health {
        int hp = config::PLAYER_HP;
    };

    struct MaxHealth {
        int hp = config::PLAYER_HP;
    };

    struct Damage {
        int value = config::PLAYER_DAMAGE;
    };

    struct MultiShot {
        int bullets = config::MULTISHOT_BULLETS;
    };

    struct Shield {
        float cooldown = config::SHIELD_COOLDOWN;
        float timer = 0.f;
    };

    struct Regeneration {
        int hp = config::REGENERATION_HP;
        float interval = config::REGENERATION_INTERVAL;
        float timer = config::REGENERATION_INTERVAL;
    };

    // Temps pendant lequel un joueur maintient la touche de tir.
    struct Charge {
        float time = 0.f;
    };

    // Une balle qui traverse ses cibles, chacune n'etant touchee qu'une fois.
    struct Pierce {
        std::vector<Engine::Entity> hit;
    };

    // L'arme secondaire d'un joueur, tiree avec la touche X.
    struct SecondaryWeapon {
        WeaponKind kind = WeaponKind::Rocket;
        float cooldown = 0.f;
        float timer = 0.f;
        float charge = 0.f;
    };

    // Une roquette : elle explose au contact dans un rayon `radius`.
    struct Missile {
        float radius = 0.f;
    };

    // Le niveau du drone d'un joueur (0 : pas de drone).
    struct DroneLevel {
        int level = 0;
    };

    // Le drone lui-meme : son proprietaire et le temps avant son prochain tir.
    struct DroneBody {
        Engine::Entity owner{};
        float timer = 0.f;
    };

    struct Weapon {
        float cooldown = config::PLAYER_FIRE_COOLDOWN;
        float timer = 0.f;
    };

    struct PlayerTag {
        int slot = 0;
    };

    // Le boss : il entre, s'arrete, puis fait des va-et-vient.
    struct ObstacleTag {
        ObstacleKind kind = ObstacleKind::Rock;
    };

    // Un emetteur laser cree et detruit son faisceau au rythme de la musique.
    struct LaserCycle {
        float age = 0.f;
        bool hasBeam = false;
        Engine::Entity beam{};
    };

    // Une frappe annoncee : zone rouge inoffensive, puis rayon mortel.
    struct StrikeSpec {
        float x = 0.f;
        float y = 0.f;
        float width = 0.f;
        float height = 0.f;
        float warn = 1.f;
        float active = 0.f;
        float vy = 0.f;
    };

    struct Strike {
        float time = 0.f;
        StrikeSpec spec;
        bool deadly = false;
    };

    enum class LeviathanMove { Rest, Columns, Sweep, Aim, Windup, Dash, Return };

    struct LeviathanState {
        LeviathanMove move = LeviathanMove::Rest;
        float timer = 1.5f;
        float swing = 0.f;
        int step = 0;
    };

    // Une tourelle tire sur le joueur le plus proche toutes les interval s.
    struct TurretFire {
        float interval = 1.f;
        float timer = 0.f;
    };

    // Va-et-vient vertical d'un obstacle autour de sa hauteur de depart.
    struct ObstacleMotion {
        float baseY = 0.f;
        float amplitude = 0.f;
        float omega = 0.f;
        float age = 0.f;
    };

    // Ce que les portails ont change chez un joueur.
    struct PlayerEffects {
        bool inverted = false;
        float speedFactor = 1.f;
        Engine::Entity lastPortal{};
    };

    struct BossType {
        BossKind kind = BossKind::Standard;
    };

    struct BossTag {
        float age = 0.f;
        float summonTimer = 0.f;
    };

    struct MonsterTag {
        int slot = 0;
    };

    struct MonsterType {
        MonsterKind kind = MonsterKind::Basic;
    };

    struct MonsterState {
        float age = 0.f;
        bool locked = false;
        bool summoned = false;
    };

    struct ProjectileTag {
    };

    // Qui a tire la balle : le tir allie ne touche pas son propre tireur.
    struct Shooter {
        Engine::Entity owner;
    };

    enum class TeamId { Player, Monster };

    struct Team {
        TeamId id = TeamId::Player;
    };

    // Les valeurs sont envoyees sur le reseau : ajouter a la fin, avant Count.

    enum class SpriteId {
        Player, Monster, PlayerBullet, MonsterBullet, Explosion, Boss, Coin,
        Tank, Kamikaze, Elite, Rocket, Bomber,
        DasherBoss, SummonerBoss, FortressBoss,
        RocketMissile, Bomb, AlienShard, RockWall, SpikeWall,
        ReversePortal, FastPortal, NormalPortal, LaserPost, TurretBlock, MineBall, BeamOn, BeamWarn, Meteor,
        LeviathanBolt, PlayerRocket, PlayerLaser, PlayerBoltDown, PlayerBoltUp,
        PlayerDrone,
        Count
    };

    constexpr bool isBossSprite(const SpriteId id)
    {
        return id == SpriteId::Boss || id == SpriteId::DasherBoss
            || id == SpriteId::SummonerBoss || id == SpriteId::FortressBoss;
    }

    struct Renderable {
        SpriteId id = SpriteId::Player;
    };

    struct Lifetime {
        float duration = config::EXPLOSION_DURATION;
        float remaining = config::EXPLOSION_DURATION;
    };

    struct NetId {
        std::uint32_t value = 0;
    };

    struct coins {
        std::size_t value = 1;
    };

    struct CoinCounter {
        std::size_t total = 0;
    };

    struct coinCounter {
        long int quantity = 0;
    };
}
