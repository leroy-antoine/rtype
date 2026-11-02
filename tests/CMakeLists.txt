add_executable(rtype_tests
        main.cpp
        GameTests.cpp
        AugmentTests.cpp
        MapTests.cpp
        ProtocolTests.cpp)
target_include_directories(rtype_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rtype_tests PRIVATE rtype::common rtype::game rtype::protocol rtype::maps)

add_test(NAME rtype_tests COMMAND rtype_tests)
