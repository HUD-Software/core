cmake_minimum_required(VERSION 3.18)
# If operating system for which CMake to build is Windows set SDK version to 10
set(CMAKE_SYSTEM_VERSION 10.0)

# Set project name
project(Core
		DESCRIPTION "HUD Engine Core"
		LANGUAGES CXX
)

# Set the name of the target
set(target_name OSLayer)

# Define a grouping for source files in IDE project generation
# source_files variable is a list all files in Implementation and Interface directory
set( src
	src/hash/Crc32.cpp
)

#Linux implementations
if(UNIX AND NOT APPLE)
set( src ${implementations}
	src/os_linux/LinuxDebugger.cpp
)
endif()

set( interface
	src/core/allocators/aligned_heap_allocator.h
	src/core/allocators/allocation.h
	src/core/allocators/heap_allocator.h
	src/core/containers/aligned_buffer.h
	src/core/containers/array.h
	src/core/containers/optional.h
	src/core/containers/pair.h
	src/core/containers/shared_pointer.h
	src/core/containers/tuple.h
	src/core/containers/tuple_element.h
	src/core/containers/tuple_size.h
	src/core/containers/unique_pointer.h
	src/core/hash/city_hash.h
	src/core/hash/crc32.h
	src/core/iterators/random_access_iterator.h
	src/core/os_common/atomics.h
	src/core/os_common/character.h
	src/core/os_common/cstring.h
	src/core/os_common/memory.h
	src/core/os_common/types.h
	src/core/os_common/uuid.h
	src/core/os_linux/atomics.h
	src/core/os_linux/character.h
	src/core/os_linux/cstring.h
	src/core/os_linux/debugger.h
	src/core/os_linux/memory.h
	src/core/os_linux/types.h
	src/core/os_linux/uuid.h
	src/core/os_windows/atomics.h
	src/core/os_windows/atomics_intel.h
	src/core/os_windows/character.h
	src/core/os_windows/cstring.h
	src/core/os_windows/debugger.h
	src/core/os_windows/memory.h
	src/core/os_windows/types.h
	src/core/os_windows/uuid.h
	src/core/os_windows/windows_api.h
	src/core/templates/bit_cast.h
	src/core/templates/declval.h
	src/core/templates/default_deleter.h
	src/core/templates/equal.h
	src/core/templates/forward.h
	src/core/templates/greater.h
	src/core/templates/greater_equal.h
	src/core/templates/integer_sequence.h
	src/core/templates/less.h
	src/core/templates/less_equal.h
	src/core/templates/lexicographical_compare.h
	src/core/templates/max.h
	src/core/templates/min.h
	src/core/templates/move.h
	src/core/templates/select_deleter_pointer_type.h
	src/core/templates/swap.h
	src/core/traits/add_const.h
	src/core/traits/add_cv.h
	src/core/traits/add_lvalue_reference.h
	src/core/traits/add_pointer.h
	src/core/traits/add_rvalue_reference.h
	src/core/traits/add_volatile.h
	src/core/traits/conjunction.h
	src/core/traits/apply_cv.h
	src/core/traits/common_type.h
	src/core/traits/conditional.h
	src/core/traits/decay.h
	src/core/traits/enable_if.h
	src/core/traits/extent.h
	src/core/traits/has_unique_object_representations.h
	src/core/traits/integral_constant.h
	src/core/traits/is_any_type_in.h
	src/core/traits/is_arithmetic.h
	src/core/traits/is_array.h
	src/core/traits/is_assignable.h
	src/core/traits/is_base_of.h
	src/core/traits/is_bitwise_comparable.h
	src/core/traits/is_bitwise_convertible.h
	src/core/traits/is_bitwise_copy_assignable.h
	src/core/traits/is_bitwise_copy_constructible.h
	src/core/traits/is_bitwise_move_assignable.h
	src/core/traits/is_bool.h
	src/core/traits/is_bounded_array.h
	src/core/traits/is_class.h
	src/core/traits/is_comparable_with_equal.h
	src/core/traits/is_comparable_with_greater.h
	src/core/traits/is_comparable_with_greater_equal.h
	src/core/traits/is_comparable_with_less.h
	src/core/traits/is_comparable_with_less_equal.h
	src/core/traits/is_comparable_with_not_equal.h
	src/core/traits/is_const.h
	src/core/traits/is_constant_evaluated.h
	src/core/traits/is_constructible.h
	src/core/traits/is_convertible.h
	src/core/traits/is_copy_assignable.h
	src/core/traits/is_copy_constructible.h
	src/core/traits/is_default_constructible.h
	src/core/traits/is_derived_from.h
	src/core/traits/is_destructible.h
	src/core/traits/is_enum.h
	src/core/traits/is_explicitly_constructible.h
	src/core/traits/is_explicitly_copy_constructible.h
	src/core/traits/is_explicitly_default_constructible.h
	src/core/traits/is_explicitly_move_constructible.h
	src/core/traits/is_floating_point.h
	src/core/traits/is_function.h
	src/core/traits/is_implicitly_constructible.h
	src/core/traits/is_implicitly_copy_constructible.h
	src/core/traits/is_implicitly_default_constructible.h
	src/core/traits/is_implicitly_move_constructible.h
	src/core/traits/is_integral.h
	src/core/traits/is_lvalue_reference.h
	src/core/traits/is_member_function_pointer.h
	src/core/traits/is_member_object_pointer.h
	src/core/traits/is_member_pointer.h
	src/core/traits/is_move_assignable.h
	src/core/traits/is_move_constructible.h
	src/core/traits/is_nothrow_assignable.h
	src/core/traits/is_nothrow_constructible.h
	src/core/traits/is_nothrow_copy_assignable.h
	src/core/traits/is_nothrow_copy_constructible.h
	src/core/traits/is_nothrow_default_constructible.h
	src/core/traits/is_nothrow_destructible.h
	src/core/traits/is_nothrow_move_assignable.h
	src/core/traits/is_nothrow_move_constructible.h
	src/core/traits/is_nothrow_swappable.h
	src/core/traits/is_not_same.h
	src/core/traits/is_nullptr.h
	src/core/traits/is_object.h
	src/core/traits/is_pointer.h
	src/core/traits/is_power_of_two.h
	src/core/traits/is_reference.h
	src/core/traits/is_referenceable.h
	src/core/traits/is_rvalue_reference.h
	src/core/traits/is_same.h
	src/core/traits/is_same_size.h
	src/core/traits/is_scalar.h
	src/core/traits/is_signed.h
	src/core/traits/is_standard_layout.h
	src/core/traits/is_swappable.h
	src/core/traits/is_trivial.h
	src/core/traits/is_trivially_assignable.h
	src/core/traits/is_trivially_constructible.h
	src/core/traits/is_trivially_copyable.h
	src/core/traits/is_trivially_copy_assignable.h
	src/core/traits/is_trivially_copy_constructible.h
	src/core/traits/is_trivially_default_constructible.h
	src/core/traits/is_trivially_destructible.h
	src/core/traits/is_trivially_move_assignable.h
	src/core/traits/is_trivially_move_constructible.h
	src/core/traits/is_trivially_swappable.h
	src/core/traits/is_unbounded_array.h
	src/core/traits/is_union.h
	src/core/traits/is_unsigned.h
	src/core/traits/is_void.h
	src/core/traits/is_volatile.h
	src/core/traits/make_signed.h
	src/core/traits/make_unsigned.h
	src/core/traits/negation.h
	src/core/traits/disjunction.h
	src/core/traits/remove_all_extents.h
	src/core/traits/remove_const.h
	src/core/traits/remove_cv.h
	src/core/traits/remove_cv_ref.h
	src/core/traits/remove_extent.h
	src/core/traits/remove_pointer.h
	src/core/traits/remove_reference.h
	src/core/traits/remove_volatile.h
	src/core/traits/underlying_type.h
	src/core/traits/void_t.h
	src/core/traits/void_t.h
	src/core/assert.h
	src/core/atomics.h
	src/core/character.h
	src/core/compilation.h
	src/core/compiler_defines.h
	src/core/containers
	src/core/cstring.h
	src/core/debugger.h
	src/core/defines.h
	src/core/hash
	src/core/hash.h
	src/core/iterators
	src/core/math.h
	src/core/memory.h
	src/core/minimal.h
	src/core/os_common
	src/core/os_linux
	src/core/os_windows
	src/core/slice.h
	src/core/string.h
	src/core/tag_init.h
	src/core/tag_in_place.h
	src/core/templates
	src/core/traits
	src/core/types.h
	src/core/uuid.h
)

source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${interface} ${src})

# Add OSLayer to the project
add_library(Core STATIC ${interface} ${src})

# Use C++ 20
set(CMAKE_CXX_STANDARD 20)
if(MSVC)
	target_compile_options( Core PRIVATE /Zc:__cplusplus /std:c++20)
endif()

#Set the target under Runtime folder
set_target_properties(Core PROPERTIES FOLDER Runtime)

# Set output directory
set_target_properties(Core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/target/${CMAKE_SYSTEM_NAME}_${CMAKE_CXX_COMPILER_ID}_${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE})

# Include Interface directory to inclusion path
target_include_directories(Core PRIVATE src)

# Add defines
target_compile_definitions(
	Core
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
