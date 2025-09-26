# Define a grouping for source files in IDE project generation
# source_files variable is a list all files in Implementation and Interface directory
FILE(GLOB_RECURSE src CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.h")
FILE(GLOB_RECURSE interface CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/interface/*.h")


source_group( TREE ${CMAKE_SOURCE_DIR} FILES ${interface} ${src})

# Add OSLayer to the project
add_library(${lib_name} STATIC ${interface} ${src})

# Use C++ 20
set_target_properties(${lib_name}
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS OFF
)

if(MSVC)
	target_compile_options( ${lib_name} PRIVATE /Zc:__cplusplus /bigobj /permissive- /EHsc /utf-8 /arch:AVX2 )
	# Define them only weh compiling with 64 bits
	target_compile_definitions(${lib_name} PRIVATE __SSE__ __SSE2__ __SSSE3__ __AVX__ __AVX2__)
elseif(EMSCRIPTEN)
	target_compile_options( ${lib_name} PRIVATE -msse4.2 -msimd128)
else()
 	target_compile_options( ${lib_name} PRIVATE -msse4.2)
endif()


# Include Interface directory to inclusion path
target_include_directories(${lib_name} PUBLIC ../interface)


target_precompile_headers(${lib_name} PRIVATE precompiled.h)

# Add defines
target_compile_definitions(
	${lib_name}
	PRIVATE
	_HAS_EXCEPTIONS=0
	_CRT_SECURE_NO_WARNINGS
	_UNICODE
	UNICODE
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

# If we build as shared library define HD_CORE_DLL_EXPORT
if(BUILD_SHARED_LIBS)
    target_compile_definitions(${lib_name} PRIVATE HD_CORE_DLL_EXPORT)
endif()