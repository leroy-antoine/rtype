add_library(engine_core INTERFACE)
add_library(engine::core ALIAS engine_core)
target_include_directories(engine_core INTERFACE include)
