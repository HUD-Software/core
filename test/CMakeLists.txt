set(test_project_name test_core)

FILE(GLOB_RECURSE src CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/test/*.cpp" "${CMAKE_SOURCE_DIR}/test/*.h")


add_executable(${test_project_name} ${src})

# set_target_properties(${test_project_name} PROPERTIES
#                       UNITY_BUILD_MODE BATCH
#                       UNITY_BUILD_BATCH_SIZE 8
#                       )

# set_target_properties(${test_project_name} PROPERTIES UNITY_BUILD ON UNITY_BUILD_MODE BATCH)

set_target_properties(${test_project_name}
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS OFF
)
# MSVC /constexpr:steps2147483647 /constexpr:depth32767
# GCC -fconstexpr-ops-limit=2147483647  -fconstexpr-depth=2147483647
# ClANG -fconstexpr-steps=2147483647 -fconstexpr-depth=2147483647
# Clang-cl -Xclang -fconstexpr-steps=2147483647
if(MSVC)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		target_compile_options( ${test_project_name} PRIVATE /Zc:__cplusplus /bigobj /permissive- /EHsc /utf-8 /arch:AVX2 -Xclang -fconstexpr-steps=2147483647)
	else()
		target_compile_options( ${test_project_name} PRIVATE /Zc:__cplusplus /bigobj /permissive- /EHsc /utf-8 /arch:AVX2 /constexpr:steps2147483647 /constexpr:depth32767)
	endif()
	# Define them only weh compiling with 64 bits
	target_compile_definitions(${lib_name} PRIVATE __SSE__ __SSE2__ __SSSE3__ __SSE4_1__ __SSE4_2__ __AVX__ __AVX2__)
elseif(EMSCRIPTEN)
	# Uncomment to generate an HTML (JS must be commented)
	# set_target_properties(${test_project_name} PROPERTIES SUFFIX ".html")
	# Uncomment to generate an JS file testable with NodeJS (HTML must be commented)
	set_target_properties(${test_project_name} PROPERTIES SUFFIX ".js")
	target_link_options(${test_project_name} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-sASSERTIONS> "-sENVIRONMENT=[\"web\",\"node\"]" -sABORTING_MALLOC=0 -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=134217728)
	target_compile_options( ${test_project_name} PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8 main.cpp -msse4.2 -msimd128 -fconstexpr-ops-limit=2147483647 -fconstexpr-steps=2147483647)
else()
	# GCC
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		target_compile_options( ${test_project_name} PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8 main.cpp -msse4.2 -fconstexpr-ops-limit=2147483647  -fconstexpr-depth=2147483647)
	# Clang
	else(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		target_compile_options( ${test_project_name} PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8 main.cpp -msse4.2 -fconstexpr-steps=2147483647 -fconstexpr-depth=2147483647)
	# Unkown
	else()
		message(FATAL_ERROR "Unkown compiler")
	endif()
endif()

get_target_property(core_type ${lib_name} TYPE)

target_precompile_headers(${test_project_name} PRIVATE precompiled.h)

# Add defines
target_compile_definitions(
	${test_project_name}
	PRIVATE
	_HAS_EXCEPTIONS=0
	_CRT_SECURE_NO_WARNINGS
	_UNICODE
	UNICODE
	HD_ABORT_ON_ASSERT
	HD_GLOBAL_NAMESPACE_TYPES
	# Don't use TEST() ans ASSERT_*() macro from google_test which is a too generic name
	GTEST_DONT_DEFINE_TEST=1
	GTEST_DONT_DEFINE_ASSERT_TRUE=1
	GTEST_DONT_DEFINE_ASSERT_FALSE=1
	GTEST_DONT_DEFINE_ASSERT_EQ=1
	GTEST_DONT_DEFINE_ASSERT_NE=1
	GTEST_DONT_DEFINE_ASSERT_LE=1
	GTEST_DONT_DEFINE_ASSERT_LT=1
	GTEST_DONT_DEFINE_ASSERT_GE=1
	GTEST_DONT_DEFINE_ASSERT_GT=1
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
target_link_libraries(${test_project_name} PRIVATE ${lib_name})
if (core_type STREQUAL SHARED_LIBRARY)
	# If we load shared library define HD_CORE_DLL_IMPORT
    target_compile_definitions(${test_project_name} PRIVATE HD_CORE_DLL_IMPORT)
	# Copy the shared library next to ${test_project_name} binary
	add_custom_command(TARGET ${test_project_name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${lib_name}> $<TARGET_FILE_DIR:${test_project_name}>)
endif()

# Add Google test dependency
message("Fetching google-test...")
include(FetchContent)
FetchContent_Declare(
	google_test 
	GIT_REPOSITORY  https://github.com/google/googletest.git
	GIT_TAG         v1.13.0
)
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCED)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(gtest_build_samples OFF CACHE BOOL "" FORCE)
set(gtest_build_tests OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(google_test)
target_link_libraries( ${test_project_name} PRIVATE gtest )
set_target_properties(gtest PROPERTIES CXX_STANDARD 20)
include(GoogleTest)
gtest_discover_tests(${test_project_name})

# Add Intel slice-by-8 dependency
message("Fetching HUD-Software/slice-by-8...")
include(FetchContent)
FetchContent_Declare(
    slice_by_8 
    GIT_REPOSITORY  https://github.com/HUD-Software/slice-by-8.git
    GIT_TAG         main
)
FetchContent_MakeAvailable(slice_by_8)

target_link_libraries( ${test_project_name} PRIVATE slice_by_8 )

# Add Intel cityhash dependency
message("Fetching HUD-Software/cityhash...")
include(FetchContent)
FetchContent_Declare(
    cityhash
    GIT_REPOSITORY  https://github.com/HUD-Software/cityhash.git
    GIT_TAG         main
)
FetchContent_MakeAvailable(cityhash)

target_link_libraries( ${test_project_name} PRIVATE cityhash )

# Add the executable test
if(EMSCRIPTEN)
	add_test(NAME ${lib_name} COMMAND node ${test_project_name}.js --gtest_output=xml:${test_project_name}_report.xml --extra-verbose --gtest_break_on_failure)
else()
	add_test(NAME ${lib_name} COMMAND ${test_project_name} --gtest_output=xml:${test_project_name}_report.xml --extra-verbose --gtest_break_on_failure)
endif()

if(SANITIZER)
	include(../sanitizer.cmake)
	enable_sanitizer(${test_project_name} ${lib_name})
endif()

if(COVERAGE)
	include(../coverage.cmake)
	enable_coverage(${test_project_name} ${lib_name})
endif()
