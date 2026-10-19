find_package(SFML 3 REQUIRED COMPONENTS Audio)

add_library(engine_audio
        src/SoundManager.cpp
        src/AudioPlayer.cpp
        include/engine/audio/AudioPlayer.hpp
        include/engine/audio/AudioPlayerImpl.hpp
        include/engine/audio/SoundManager.hpp
        include/engine/audio/SoundManagerImpl.hpp
        include/engine/audio/Types.hpp
)
add_library(engine::audio ALIAS engine_audio)

target_include_directories(engine_audio PUBLIC
        include
        ${CMAKE_CURRENT_BINARY_DIR}/include
)
target_link_libraries(engine_audio PRIVATE SFML::Audio)

include(GenerateExportHeader)
generate_export_header(engine_audio
        BASE_NAME ENGINE_AUDIO
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/engine/audio/Export.hpp
)

set_target_properties(engine_audio PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(engine_audio PUBLIC ENGINE_AUDIO_STATIC_DEFINE)
endif()

