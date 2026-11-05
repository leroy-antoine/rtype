#pragma once

#include "engine/core/Registry.hpp"
#include "Components.hpp"
#include "Difficulty.hpp"
#include "Queries.hpp"
#include "Waves.hpp"

namespace rtype {
    inline void scaleEnemy(Engine::Registry &reg, const Engine::Entity e,
        const DifficultyScale &scale)
    {
        Health &health = reg.get<Health>(e);
        Weapon &weapon = reg.get<Weapon>(e);

        health.hp = scaledAmount(health.hp, scale.enemyHp);
        weapon.cooldown /= scale.enemyFireRate;
        weapon.timer /= scale.enemyFireRate;
        if (reg.has<BossTag>(e)) {
            reg.add(e, MaxHealth{health.hp});
        }
        reg.add(e, DifficultyApplied{});
    }

    inline bool isMiniKind(const MonsterKind kind)
    {
        return kind == MonsterKind::MiniTank
            || kind == MonsterKind::MiniElite
            || kind == MonsterKind::MiniBomber;
    }

    // Plus ou moins d'ennemis dans une vague ; les mini-boss restent seuls.
    inline WaveStep densified(WaveStep wave, const float density)
    {
        if (!isMiniKind(wave.kind)) {
            wave.count = scaledAmount(wave.count, density);
        }
        return wave;
    }

    // Chaque monstre est ajuste une seule fois, a son apparition.
    inline void scaleNewEnemies(Engine::Registry &reg)
    {
        const DifficultyScale &scale = activeScale(reg);

        for (const Engine::Entity e : collectWith<MonsterTag>(reg)) {
            if (!reg.has<DifficultyApplied>(e)) {
                scaleEnemy(reg, e, scale);
            }
        }
    }
}
