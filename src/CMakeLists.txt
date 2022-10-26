cmake_minimum_required(VERSION 3.18)


# Define a grouping for source files in IDE project generation
# source_files variable is a list all files in Implementation and Interface directory
set( src
	impl/hash/crc32.cpp
)

#Linux implementations
if(UNIX AND NOT APPLE)
set( src ${src}
	impl/os_linux/debugger.cpp
)
endif()

set( interface
	core/allocators/aligned_heap_allocator.h
	core/allocators/allocation.h
	core/allocators/heap_allocator.h
	core/containers/aligned_buffer.h
	core/containers/array.h
	core/containers/optional.h
	core/containers/pair.h
	core/containers/shared_pointer.h
	core/containers/tuple.h
	core/containers/tuple_element.h
	core/containers/tuple_size.h
	core/containers/unique_pointer.h
	core/hash/city_hash.h
	core/hash/crc32.h
	core/iterators/random_access_iterator.h
	core/os_common/atomics.h
	core/os_common/character.h
	core/os_common/cstring.h
	core/os_common/memory.h
	core/os_common/types.h
	core/os_common/uuid.h
	core/os_linux/atomics.h
	core/os_linux/character.h
	core/os_linux/cstring.h
	core/os_linux/debugger.h
	core/os_linux/memory.h
	core/os_linux/types.h
	core/os_linux/uuid.h
	core/os_windows/atomics.h
	core/os_windows/atomics_intel.h
	core/os_windows/character.h
	core/os_windows/cstring.h
	core/os_windows/debugger.h
	core/os_windows/memory.h
	core/os_windows/types.h
	core/os_windows/uuid.h
	core/os_windows/windows_api.h
	core/templates/bit_cast.h
	core/templates/declval.h
	core/templates/default_deleter.h
	core/templates/equal.h
	core/templates/forward.h
	core/templates/greater.h
	core/templates/greater_equal.h
	core/templates/integer_sequence.h
	core/templates/less.h
	core/templates/less_equal.h
	core/templates/lexicographical_compare.h
	core/templates/max.h
	core/templates/min.h
	core/templates/move.h
	core/templates/select_deleter_pointer_type.h
	core/templates/swap.h
	core/traits/add_const.h
	core/traits/add_cv.h
	core/traits/add_lvalue_reference.h
	core/traits/add_pointer.h
	core/traits/add_rvalue_reference.h
	core/traits/add_volatile.h
	core/traits/conjunction.h
	core/traits/apply_cv.h
	core/traits/common_type.h
	core/traits/conditional.h
	core/traits/decay.h
	core/traits/enable_if.h
	core/traits/extent.h
	core/traits/has_unique_object_representations.h
	core/traits/integral_constant.h
	core/traits/is_one_of_types.h
	core/traits/is_arithmetic.h
	core/traits/is_array.h
	core/traits/is_assignable.h
	core/traits/is_base_of.h
	core/traits/is_bitwise_comparable.h
	core/traits/is_bitwise_convertible.h
	core/traits/is_bitwise_copyable.h
	core/traits/is_bitwise_copy_assignable.h
	core/traits/is_bitwise_copy_constructible.h
	core/traits/is_bitwise_move_assignable.h
	core/traits/is_bool.h
	core/traits/is_bounded_array.h
	core/traits/is_class.h
	core/traits/is_comparable_with_equal.h
	core/traits/is_comparable_with_greater.h
	core/traits/is_comparable_with_greater_equal.h
	core/traits/is_comparable_with_less.h
	core/traits/is_comparable_with_less_equal.h
	core/traits/is_comparable_with_not_equal.h
	core/traits/is_const.h
	core/traits/is_constant_evaluated.h
	core/traits/is_constructible.h
	core/traits/is_convertible.h
	core/traits/is_copy_assignable.h
	core/traits/is_copy_constructible.h
	core/traits/is_default_constructible.h
	core/traits/is_derived_from.h
	core/traits/is_destructible.h
	core/traits/is_enum.h
	core/traits/is_explicitly_constructible.h
	core/traits/is_explicitly_copy_constructible.h
	core/traits/is_explicitly_default_constructible.h
	core/traits/is_explicitly_move_constructible.h
	core/traits/is_floating_point.h
	core/traits/is_function.h
	core/traits/is_implicitly_constructible.h
	core/traits/is_implicitly_copy_constructible.h
	core/traits/is_implicitly_default_constructible.h
	core/traits/is_implicitly_move_constructible.h
	core/traits/is_integral.h
	core/traits/is_lvalue_reference.h
	core/traits/is_member_function_pointer.h
	core/traits/is_member_object_pointer.h
	core/traits/is_member_pointer.h
	core/traits/is_move_assignable.h
	core/traits/is_move_constructible.h
	core/traits/is_nothrow_assignable.h
	core/traits/is_nothrow_constructible.h
	core/traits/is_nothrow_copy_assignable.h
	core/traits/is_nothrow_copy_constructible.h
	core/traits/is_nothrow_default_constructible.h
	core/traits/is_nothrow_destructible.h
	core/traits/is_nothrow_move_assignable.h
	core/traits/is_nothrow_move_constructible.h
	core/traits/is_nothrow_swappable.h
	core/traits/is_not_same.h
	core/traits/is_nullptr.h
	core/traits/is_object.h
	core/traits/is_pointer.h
	core/traits/is_power_of_two.h
	core/traits/is_reference.h
	core/traits/is_referenceable.h
	core/traits/is_rvalue_reference.h
	core/traits/is_same.h
	core/traits/is_same_size.h
	core/traits/is_scalar.h
	core/traits/is_signed.h
	core/traits/is_standard_layout.h
	core/traits/is_swappable.h
	core/traits/is_trivial.h
	core/traits/is_trivially_assignable.h
	core/traits/is_trivially_constructible.h
	core/traits/is_trivially_copyable.h
	core/traits/is_trivially_copy_assignable.h
	core/traits/is_trivially_copy_constructible.h
	core/traits/is_trivially_default_constructible.h
	core/traits/is_trivially_destructible.h
	core/traits/is_trivially_move_assignable.h
	core/traits/is_trivially_move_constructible.h
	core/traits/is_trivially_swappable.h
	core/traits/is_unbounded_array.h
	core/traits/is_union.h
	core/traits/is_unsigned.h
	core/traits/is_void.h
	core/traits/is_volatile.h
	core/traits/make_signed.h
	core/traits/make_unsigned.h
	core/traits/negation.h
	core/traits/disjunction.h
	core/traits/remove_all_extents.h
	core/traits/remove_const.h
	core/traits/remove_cv.h
	core/traits/remove_cv_ref.h
	core/traits/remove_extent.h
	core/traits/remove_pointer.h
	core/traits/remove_reference.h
	core/traits/remove_volatile.h
	core/traits/underlying_type.h
	core/traits/void_t.h
	core/traits/void_t.h
	core/assert.h
	core/atomics.h
	core/character.h
	core/compilation.h
	core/compiler_defines.h
	core/containers
	core/cstring.h
	core/debugger.h
	core/defines.h
	core/hash
	core/hash.h
	core/iterators
	core/math.h
	core/memory.h
	core/minimal.h
	core/os_common
	core/os_linux
	core/os_windows
	core/slice.h
	core/string.h
	core/tag_init.h
	core/tag_in_place.h
	core/templates
	core/traits
	core/types.h
	core/uuid.h
)


source_group( TREE ${CMAKE_SOURCE_DIR} FILES ${interface} ${src})

# Add OSLayer to the project
add_library(core STATIC ${interface} ${src})

# Use C++ 20
set_target_properties(core
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
if(MSVC)
	target_compile_options( core PRIVATE /Zc:__cplusplus /std:c++20)
endif()

if(COVERAGE AND MSVC)
	target_link_options(core PRIVATE /PROFILE)
elseif(COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(core PRIVATE --coverage)
	target_link_options(core PRIVATE --coverage)
endif()


# Include Interface directory to inclusion path
target_include_directories(core PUBLIC .)

# Add defines
target_compile_definitions(
	core
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