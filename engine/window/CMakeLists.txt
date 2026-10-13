find_package(SFML 3 REQUIRED COMPONENTS Graphics)



add_library(engine_window
        src/Window.cpp
)
add_library(engine::window ALIAS engine_window)


target_include_directories(engine_window PUBLIC
        include
        ${CMAKE_CURRENT_BINARY_DIR}/include
)
target_link_libraries(engine_window PRIVATE SFML::Graphics)

include(GenerateExportHeader)
generate_export_header(engine_window
        BASE_NAME ENGINE_WINDOW
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/engine/window/Export.hpp
)

set_target_properties(engine_window PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(engine_window PUBLIC ENGINE_WINDOW_STATIC_DEFINE)
endif()