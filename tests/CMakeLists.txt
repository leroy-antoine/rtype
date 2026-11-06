add_executable(rtype_tests
        main.cpp
        GameTests.cpp
        AugmentTests.cpp
        MapTests.cpp
        ShopTests.cpp
        ChargeTests.cpp
        SecondaryTests.cpp
        RocketTests.cpp
        LaserTests.cpp
        LightningTests.cpp
        DroneTests.cpp
        SettingsTests.cpp
        DifficultyTests.cpp
        ProtocolTests.cpp
        MapJsonTests.cpp
        CustomMapTests.cpp
        CustomMapListTests.cpp
        ${CMAKE_SOURCE_DIR}/src/client/game/Rtype/src/CustomMapList.cpp)
target_include_directories(rtype_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/client/game/Rtype/include
        ${CMAKE_SOURCE_DIR}/engine/window/include)
target_link_libraries(rtype_tests PRIVATE rtype::common rtype::game rtype::protocol rtype::maps rtype::mapjson)
target_compile_definitions(rtype_tests PRIVATE
        CUSTOM_MAPS_DIR="${CMAKE_SOURCE_DIR}/src/client/game/Rtype/CustomMaps")

add_test(NAME rtype_tests COMMAND rtype_tests)
