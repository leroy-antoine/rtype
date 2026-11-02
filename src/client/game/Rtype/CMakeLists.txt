add_library(rtype_game INTERFACE
        include/logic/Components.hpp
        include/logic/Waves.hpp
        include/logic/Boss.hpp
        include/logic/Status.hpp
        include/logic/Movements.hpp
        include/logic/Factories.hpp
        include/logic/Damages.hpp
        include/logic/Colisions.hpp
        include/logic/Queries.hpp
        include/logic/Systems.hpp
        include/logic/Config.hpp
        include/logic/Bullet.hpp
        include/logic/Monsters.hpp
        include/logic/Events.hpp
        include/logic/Replication.hpp
        include/logic/Game.hpp
        include/logic/Collectibles.hpp
        include/logic/Levels.hpp
        include/logic/LevelBuilder.hpp
        include/logic/Builder.hpp
        include/logic/Kinds.hpp
        include/logic/MonsterDefs.hpp
        include/logic/BossDefs.hpp
        include/logic/MapList.hpp
        include/logic/MapHelpers.hpp
        include/logic/Leaderboard.hpp
        include/logic/MonsterBehavior.hpp
        include/logic/LevelSkip.hpp)
add_library(rtype::game ALIAS rtype_game)
target_include_directories(rtype_game INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(rtype_game INTERFACE engine::core)

add_library(rtype_maps STATIC
        src/maps/map1.cpp
        src/maps/map2.cpp
        src/maps/map3.cpp)
add_library(rtype::maps ALIAS rtype_maps)
target_link_libraries(rtype_maps PUBLIC rtype::game)
