add_library(engine_core
    src/ComponentRegistry.cpp
)
add_library(engine::core ALIAS engine_core)

target_include_directories(engine_core PUBLIC
    include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

include(GenerateExportHeader)
generate_export_header(engine_core
    BASE_NAME ENGINE_CORE
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/engine/core/Export.hpp
)

set_target_properties(engine_core PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(engine_core PUBLIC ENGINE_CORE_STATIC_DEFINE)
endif()
