add_library(rtype_common INTERFACE
        include/utils/vectors.hpp)
add_library(rtype::common ALIAS rtype_common)
target_include_directories(rtype_common INTERFACE include)

target_link_libraries(rtype_common INTERFACE engine::core)

add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(games)
