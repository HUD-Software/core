cmake_minimum_required(VERSION 3.18)
# If operating system for which CMake to build is Windows set SDK version to 10
set(CMAKE_SYSTEM_VERSION 10.0)

set(lib_name core)

# Set project name
project(${lib_name}
		DESCRIPTION "HUD Engine Core"
		LANGUAGES CXX
)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)