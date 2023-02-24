cmake_minimum_required(VERSION 3.18)
# If operating system for which CMake to build is Windows set SDK version to 10
set(CMAKE_SYSTEM_VERSION 10.0)

# Set project name
project(core
		DESCRIPTION "HUD Engine Core"
		LANGUAGES CXX
)


# Coverage configuration and build type
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "Flags used by the C++ compiler during coverage builds.")
set(CMAKE_C_FLAGS_COVERAGE "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "Flags used by the C compiler during coverage builds.")
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS_DEBUG}" CACHE STRING "Flags used for linking binaries during coverage builds.")
set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" CACHE STRING "Flags used by the shared libraries linker during coverage builds.")
mark_as_advanced(
	CMAKE_CXX_FLAGS_COVERAGE
	CMAKE_C_FLAGS_COVERAGE
	CMAKE_EXE_LINKER_FLAGS_COVERAGE
	CMAKE_SHARED_LINKER_FLAGS_COVERAGE)

set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Coverage.")
if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES Debug Release MinSizeRel RelWithDebInfo Coverage)
	set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)
endif()

# Create variable CMAKE_CXX_COMPILER_PATH
get_filename_component(CMAKE_CXX_COMPILER_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)