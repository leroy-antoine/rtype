#pragma once

#include <cassert>
#include <cstddef>
#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Components.hpp"
#include "Config.hpp"

namespace rtype {
    inline Engine::Entity createPlayer(Engine::Registry &reg, const int slot)
    {
        Engine::Entity e = reg.create();
        const std::size_t index = static_cast<std::size_t>(slot);

        assert(slot >= 0 && index < config::MAX_PLAYERS);
        reg.add(e, PlayerTag{slot});
        reg.add(e, PlayerInput{});
        reg.add(e, gameLogic::Vec2Pos{{config::PLAYER_START_X,
            config::PLAYER_START_Y[index]}});
        reg.add(e, gameLogic::Vec2Direction{});
        reg.add(e, gameLogic::Vec2Velocity{});
        reg.add(e, gameLogic::Vec2Hitbox{{config::PLAYER_WIDTH,
            config::PLAYER_HEIGHT}});
        reg.add(e, Speed{config::PLAYER_SPEED});
        reg.add(e, Health{config::PLAYER_HP});
        reg.add(e, Weapon{config::PLAYER_FIRE_COOLDOWN});
        reg.add(e, Damage{config::PLAYER_DAMAGE});
        reg.add(e, Team{TeamId::Player});
        return e;
    }

    inline Engine::Entity createMonster(Engine::Registry &reg, const int slot)
    {
        Engine::Entity e = reg.create();

        reg.add(e, MonsterTag{slot});
        reg.add(e, gameLogic::Vec2Pos{0, 0});
        reg.add(e, gameLogic::Vec2Direction{});
        reg.add(e, gameLogic::Vec2Velocity{});
        reg.add(e, gameLogic::Vec2Hitbox{{config::MONSTER_WIDTH,
            config::MONSTER_HEIGHT}});
        reg.add(e, Speed{config::MONSTER_SPEED});
        reg.add(e, Health{config::MONSTER_HP});
        reg.add(e, Weapon{config::MONSTER_FIRE_COOLDOWN});
        reg.add(e, Damage{config::MONSTER_DAMAGE});
        reg.add(e, Team{TeamId::Monster});
        return e;
    }
}
