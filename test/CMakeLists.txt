cmake_minimum_required(VERSION 3.18)

# Set project name
project(Test
		DESCRIPTION "HUD Engine Core tests"
		LANGUAGES CXX
)

set( src
	src/hash/crc32.cpp
)

set(CMAKE_CXX_STANDARD 20)
if(MSVC)
	target_compile_options( Core PRIVATE /Zc:__cplusplus /std:c++20)
endif()

add_executable(Test main.cpp)

# Add defines
target_compile_definitions(
	Core
	PRIVATE
	_HAS_EXCEPTIONS=0
	_CRT_SECURE_NO_WARNINGS
	_UNICODE
	UNICODE
	HD_ABORT_ON_ASSERT
	HD_GLOBAL_NAMESPACE_TYPES
	# Debug specific compiler flags
	$<$<CONFIG:Debug>:HD_DEBUG>
	# Release specific compiler flags
	$<$<CONFIG:Release>:HD_RELEASE>
	# MinSizeRel specific compiler flags
	$<$<CONFIG:MinSizeRel>:HD_RELEASE>
	# DebugOptimized specific compiler flags
	$<$<CONFIG:RelWithDebInfo>:HD_DEBUGOPTIMIZED>
)

# Add Core dependency
add_dependencies( Test Core)
target_link_libraries(Test PRIVATE Core)

# Add Google test dependencys
include(FetchContent)
FetchContent_Declare(
    googletest 
    GIT_REPOSITORY  git@github.com:HUD-Software/google-test.git
    GIT_TAG         e6f4409b1dbda2bdd95d7012d9b5431edfa43907 # v1.12.1
)
FetchContent_MakeAvailable(googletest)

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   GIT_REPOSITORY git@github.com:HUD-Software/google-test.git
#   GIT_TAG        v1.12.1
# )
# FetchContent_GetProperties(googletest)
# if (NOT googletest_POPULATED)
#     FetchContent_Populate(googletest)
# endif ()
# add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

# add_dependencies( Test googletest)
target_link_libraries( Test PRIVATE gtest )

add_test(NAME Core COMMAND Test --gtest_output=xml:core_test_report.xml --extra-verbose --gtest_break_on_failure)

# #add ThirdParty's dependencies
# add_dependencies( ${target_name} SliceBy8)
# target_link_libraries( ${target_name} PRIVATE SliceBy8)
# add_dependencies( ${target_name} CityHash)
# target_link_libraries( ${target_name} PRIVATE CityHash)
