find_package(SFML 3 REQUIRED COMPONENTS Graphics)
add_executable(build_test build_test.cpp)
target_link_libraries(build_test PRIVATE SFML::Graphics)
