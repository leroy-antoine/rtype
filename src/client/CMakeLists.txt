add_executable(r-type_client
        main.cpp
        game/App.cpp
        game/Controls.cpp
        game/GameView.cpp
        game/Replica.cpp
        game/SpriteBank.cpp
        network/Network.cpp)
target_include_directories(r-type_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(r-type_client PRIVATE rtype::common rtype::game rtype::protocol engine::window engine::audio)

add_custom_command(TARGET r-type_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/games/rtype/assets
                $<TARGET_FILE_DIR:r-type_client>/assets)