cmake_minimum_required(VERSION 3.0)
project(FTB)

set(EXE_NAME "App")
add_executable(${EXE_NAME} Main/main.cpp FileWrapper/FileWrapper.cpp ../../SharedCode/Block/Block.cpp ../../SharedCode/BinaryFile/BinaryFile.cpp)
add_definitions(-std=c++11)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED graphics window system)
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	target_link_libraries(${EXE_NAME} ${SFML_LIBRARIES})
endif()

