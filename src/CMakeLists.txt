add_library(rtype_common INTERFACE
        include/utils/Vectors.hpp)
add_library(rtype::common ALIAS rtype_common)
target_include_directories(rtype_common INTERFACE include)

target_link_libraries(rtype_common INTERFACE engine::core)

add_library(rtype_protocol INTERFACE
        include/rtype/Protocol.hpp)
add_library(rtype::protocol ALIAS rtype_protocol)
target_include_directories(rtype_protocol INTERFACE include)

target_link_libraries(rtype_protocol INTERFACE rtype::common rtype::game engine::network)

add_subdirectory(server)
add_subdirectory(client)
