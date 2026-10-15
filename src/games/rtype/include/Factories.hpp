#pragma once

#include "engine/core/Registry.hpp"
#include "utils/Vectors.hpp"
#include "Components.hpp"

namespace rtype {
    inline Engine::Entity createPlayer(Engine::Registry &reg, const int slot)
    {
        Engine::Entity e = reg.create();

        reg.add(e, PlayerTag{slot});
        reg.add(e, PlayerInput{});
        reg.add(e, gameLogic::Vec2Pos{0, 0});
        reg.add(e, gameLogic::Vec2Direction{});
        reg.add(e, gameLogic::Vec2Velocity{});
        reg.add(e, gameLogic::Vec2Hitbox{{32.f, 16.f}});
        reg.add(e, Speed{300.f});
        reg.add(e, Health{});
        reg.add(e, Weapon{});
        reg.add(e, Damage{});
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
        reg.add(e, gameLogic::Vec2Hitbox{{32.f, 16.f}});
        reg.add(e, Speed{300.f});
        reg.add(e, Health{});
        reg.add(e, Weapon{});
        reg.add(e, Damage{});
        reg.add(e, Team{TeamId::Monster});
        return e;
    }
}

