cmake_minimum_required(VERSION 3.0)
project(BTF)

set(EXE_NAME "btf")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

file(GLOB SOURCES "*/*.cpp" "../../SharedCode/*/*.cpp")

add_executable(${EXE_NAME} ${SOURCES})

include("../../CMake_config/ConfigSFML.cmake")
