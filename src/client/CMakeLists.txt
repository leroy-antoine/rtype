add_executable(r-type_client main.cpp)
target_link_libraries(r-type_client PRIVATE rtype::common engine::window)
