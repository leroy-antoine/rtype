//
// Created by antoine on 10/16/26.
//

#pragma once
#include <cmath>
#include <vector>
#include "BossDefs.hpp"
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

    inline const ProjectileDef &projectileOf(const Engine::Registry &reg,
        const Engine::Entity shooter)
    {
        if (reg.has<BossTag>(shooter)) {
            return bossDef(reg.get<BossType>(shooter).kind).projectile;
        }
        if (reg.has<MonsterType>(shooter)) {
            return monsterDef(reg.get<MonsterType>(shooter).kind).projectile;
        }
        return BASIC.projectile;
    }

    // Chaque type de monstre tire son propre projectile.
    inline BulletLook bulletLook(const Engine::Registry &reg,
        const Engine::Entity shooter, const Team &team)
    {
        if (team.id == TeamId::Player) {
            return {SpriteId::PlayerBullet, config::BULLET_WIDTH,
                config::BULLET_HEIGHT};
        }
        const ProjectileDef &projectile = projectileOf(reg, shooter);

        return {projectile.sprite, projectile.width, projectile.height};
    }

    // Une salve vers la gauche : `shots` balles reparties en eventail.
    inline std::vector<gameLogic::Vec2> volley(const ProjectileDef &projectile)
    {
        std::vector<gameLogic::Vec2> shots;
        const float middle = static_cast<float>(projectile.shots - 1) / 2.f;

        for (int i = 0; i < projectile.shots; ++i) {
            const float angle = (static_cast<float>(i) - middle)
                * projectile.spread;

            shots.push_back({-std::cos(angle) * projectile.speed,
                std::sin(angle) * projectile.speed});
        }
        return shots;
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
}
