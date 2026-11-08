add_subdirectory(game/Rtype)

add_executable(r-type_client
        main.cpp
        game/Rtype/src/App.cpp
        game/Rtype/src/Audio.cpp
        game/Rtype/src/Background.cpp
        game/Rtype/src/Font.cpp
        game/Rtype/src/Hud.cpp
        game/Rtype/src/HudLobby.cpp
        game/Rtype/src/HudVictory.cpp
        game/Rtype/src/LobbyScreen.cpp
        game/Rtype/src/CustomMapList.cpp
        game/Rtype/src/Controls.cpp
        game/Rtype/src/GameView.cpp
        game/Rtype/src/Prediction.cpp
        game/Rtype/src/Replica.cpp
        game/Rtype/src/ShopPanel.cpp
        game/Rtype/src/SpriteBank.cpp
        network/Network.cpp)
target_include_directories(r-type_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(r-type_client PRIVATE rtype::common rtype::game rtype::protocol rtype::mapjson engine::window engine::audio)

add_custom_command(TARGET r-type_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/client/game/Rtype/assets
                $<TARGET_FILE_DIR:r-type_client>/assets)

add_custom_command(TARGET r-type_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/client/game/Rtype/CustomMaps
                $<TARGET_FILE_DIR:r-type_client>/CustomMaps)
