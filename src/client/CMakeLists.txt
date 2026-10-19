add_executable(r-type_client main.cpp)
target_link_libraries(r-type_client PRIVATE rtype::common rtype::game engine::window)

add_custom_command(TARGET r-type_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/games/rtype/assets
                $<TARGET_FILE_DIR:r-type_client>/assets)
