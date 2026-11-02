//
// Created by antoine on 10/16/26.
//

#pragma once
#include <cmath>
#include <vector>
#include "Components.hpp"
#include "Config.hpp"
#include "engine/core/Entity.hpp"
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"

namespace rtype {
    // Apparence d'un projectile : son sprite et sa taille.
    struct BulletLook {
        SpriteId sprite;
        float width;
        float height;
    };

    inline bool isShooter(const Engine::Registry &reg, const Engine::Entity e)
    {
        return reg.has<gameLogic::Vec2Pos>(e)
            && reg.has<gameLogic::Vec2Hitbox>(e)
            && reg.has<Team>(e)
            && reg.has<Damage>(e);
    }

    inline float bulletDirection(const Team &team)
    {
        if (team.id == TeamId::Player) {
            return 1.f;
        }
        return -1.f;
    }

    // Les balles du boss sont plus grosses que les autres.
    inline BulletLook bulletLook(const Engine::Registry &reg,
        const Engine::Entity shooter, const Team &team)
    {
        if (reg.has<BossTag>(shooter)) {
            return {SpriteId::MonsterBullet, config::BOSS_BULLET_SIZE,
                config::BOSS_BULLET_SIZE};
        }
        if (team.id == TeamId::Player) {
            return {SpriteId::PlayerBullet, config::BULLET_WIDTH,
                config::BULLET_HEIGHT};
        }
        return {SpriteId::MonsterBullet, config::BULLET_WIDTH,
            config::BULLET_HEIGHT};
    }

    // La balle part du bord du tireur, centree sur sa hauteur.
    inline gameLogic::Vec2Pos bulletStartPos(const gameLogic::Vec2Pos &pos,
        const gameLogic::Vec2Hitbox &box, const BulletLook &look,
        const float direction)
    {
        float x = pos.x - look.width;
        const float y = pos.y + box.y / 2.f - look.height / 2.f;

        if (direction > 0.f) {
            x = pos.x + box.x;
        }
        return gameLogic::Vec2Pos{{x, y}};
    }

    inline std::vector<gameLogic::Vec2> fan(const int count, const float spread,
        const float speed, const float direction)
    {
        const float middle = (count - 1) / 2.f;
        std::vector<gameLogic::Vec2> shots;

        for (int i = 0; i < count; ++i) {
            const float angle = (i - middle) * spread;

            shots.push_back({direction * std::cos(angle) * speed,
                std::sin(angle) * speed});
        }
        return shots;
    }
}
