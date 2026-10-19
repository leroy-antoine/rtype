find_package(asio CONFIG REQUIRED)

add_library(engine_network
    src/BinaryWriter.cpp
    src/BinaryReader.cpp
    src/Packet.cpp
    src/ReliableChannel.cpp
    src/Connection.cpp
    src/UdpSocket.cpp
    src/Server.cpp
)
add_library(engine::network ALIAS engine_network)

target_include_directories(engine_network PUBLIC
    include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)
target_link_libraries(engine_network PRIVATE asio::asio)

if(WIN32)
    target_compile_definitions(engine_network PRIVATE _WIN32_WINNT=0x0A00)
    target_link_libraries(engine_network PRIVATE ws2_32 mswsock)
endif()

include(GenerateExportHeader)
generate_export_header(engine_network
    BASE_NAME ENGINE_NETWORK
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/engine/network/Export.hpp
)

set_target_properties(engine_network PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(engine_network PUBLIC ENGINE_NETWORK_STATIC_DEFINE)
endif()
