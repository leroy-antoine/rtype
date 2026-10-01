find_package(SFML 3 REQUIRED COMPONENTS Graphics)

add_executable(r-type_client main.cpp)
target_link_libraries(r-type_client PRIVATE rtype::common SFML::Graphics)
