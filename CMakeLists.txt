cmake_minimum_required(VERSION 3.18)
# If operating system for which CMake to build is Windows set SDK version to 10
set(CMAKE_SYSTEM_VERSION 10.0)

# Set project name
project(core
		DESCRIPTION "HUD Engine Core"
		LANGUAGES CXX
)

SET(COVERAGE OFF CACHE BOOL "Set to true to generate code coverage")

add_subdirectory(src)

enable_testing()
add_subdirectory(test)