add_library(rtype_maps STATIC
        map1.cpp)
add_library(rtype::maps ALIAS rtype_maps)
target_include_directories(rtype_maps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(rtype_maps PUBLIC rtype::game)
