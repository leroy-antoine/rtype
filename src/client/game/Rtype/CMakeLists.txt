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
        include/logic/Game.hpp)
add_library(rtype::game ALIAS rtype_game)
target_include_directories(rtype_game INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(rtype_game INTERFACE engine::core)
