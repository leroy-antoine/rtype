cmake_minimum_required(VERSION 3.25)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake")
    message(FATAL_ERROR
        "vcpkg submodule is missing. Run:\n"
        "    git submodule update --init\n"
        "then configure again.")
endif()

project(rtype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Binaries go to build/binaries so every preset puts them in the same place.
# The generator expression stops multi-config generators (Visual Studio)
# from appending a Debug/ or Release/ subfolder.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_SOURCE_DIR}/build/binaries>")

add_subdirectory(src)
