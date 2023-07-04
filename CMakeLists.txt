cmake_minimum_required(VERSION 3.18)
# If operating system for which CMake to build is Windows set SDK version to 10
set(CMAKE_SYSTEM_VERSION 10.0)

# Set project name
project(core
		DESCRIPTION "HUD Engine Core"
		LANGUAGES CXX
)

# Create variable CMAKE_CXX_COMPILER_PATH
get_filename_component(CMAKE_CXX_COMPILER_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)

include(coverage.cmake)
include(sanitizer.cmake)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)