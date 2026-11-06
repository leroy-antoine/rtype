add_executable(r-type_server
        main.cpp
        game/App.cpp
        room/Lobby.cpp
        room/Room.cpp)
target_include_directories(r-type_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(r-type_server PRIVATE rtype::common rtype::game rtype::protocol rtype::maps)
