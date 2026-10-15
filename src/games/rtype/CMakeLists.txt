add_library(rtype_game INTERFACE
        include/Components.hpp
        include/Movements.hpp
        include/Factories.hpp
        include/Damages.hpp
        include/Colisions.hpp
        include/Systems.hpp)
add_library(rtype::game ALIAS rtype_game)
target_include_directories(rtype_game INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(rtype_game INTERFACE engine::core)
