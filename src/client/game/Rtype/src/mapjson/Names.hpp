#pragma once

#include <array>
#include "logic/Kinds.hpp"
#include "logic/Waves.hpp"
#include "logic/Zones.hpp"
#include "Reader.hpp"

// The words a map creator writes in the JSON, and what they mean.
namespace rtype::mapjson {
    using MK = MonsterKind;

    inline constexpr std::array<Name<MK>, 9> MONSTER_NAMES = {{
        {"Basic", MK::Basic}, {"Tank", MK::Tank}, {"Kamikaze", MK::Kamikaze},
        {"Elite", MK::Elite}, {"Rocket", MK::Rocket}, {"Bomber", MK::Bomber},
        {"MiniTank", MK::MiniTank}, {"MiniElite", MK::MiniElite},
        {"MiniBomber", MK::MiniBomber}}};

    inline constexpr std::array<Name<BossKind>, 4> BOSS_NAMES = {{
        {"Standard", BossKind::Standard}, {"Dasher", BossKind::Dasher},
        {"Summoner", BossKind::Summoner}, {"Fortress", BossKind::Fortress}}};

    inline constexpr std::array<Name<Shape>, 4> SHAPE_NAMES = {{
        {"Line", Shape::Line}, {"Wedge", Shape::Wedge},
        {"Column", Shape::Column}, {"Sine", Shape::Sine}}};

    inline constexpr std::array<Name<Zone>, 7> ZONE_NAMES = {{
        {"DeepSpace", Zone::DeepSpace}, {"Nebula", Zone::Nebula},
        {"Asteroids", Zone::Asteroids}, {"Wreckage", Zone::Wreckage},
        {"Core", Zone::Core}, {"Warp", Zone::Warp}, {"Arena", Zone::Arena}}};

    // What a gate is made of.
    inline constexpr std::array<Name<ObstacleKind>, 2> WALL_NAMES = {{
        {"Rock", ObstacleKind::Rock}, {"Spike", ObstacleKind::Spike}}};
}
