# Configure SFML: (included into CMakeLists.txt in both parts)

if(WIN32)
	set(SFML_ROOT "C:\\Program Files (x86)\\SFML")	# Change if needed
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../CMake_config" ${CMAKE_MODULE_PATH})
find_package(SFML 2 COMPONENTS graphics REQUIRED)

if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	target_link_libraries(${EXE_NAME} ${SFML_LIBRARIES})
else()
	message("-- ERROR: Cannot find SFML")
endif()
