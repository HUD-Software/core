# Define a grouping for source files in IDE project generation
# source_files variable is a list all files in Implementation and Interface directory
set( src
	hash/crc32.cpp
	debugger.cpp
)

set( interface
	../interface/core/algorithms/find.h
	../interface/core/algorithms/sort.h
	../interface/core/allocators/aligned_heap_allocator.h
	../interface/core/allocators/memory_allocation.h
	../interface/core/allocators/allocator_traits.h
	../interface/core/allocators/heap_allocator.h
	../interface/core/atomics/atomics_common.h
	../interface/core/atomics/atomics_gcc.h
	../interface/core/atomics/atomics_msvc_intel.h
	../interface/core/atomics/atomics_msvc.h
	../interface/core/bits/bits_common.h
	../interface/core/bits/bits_linux.h
	../interface/core/bits/bits_windows.h
	../interface/core/bits/bits.h
	../interface/core/character/character.h
	../interface/core/containers/aligned_buffer.h
	../interface/core/containers/array.h
	../interface/core/containers/compressed_pair.h
	../interface/core/containers/hashmap.h
	../interface/core/containers/hashset.h
	../interface/core/containers/optional.h
	../interface/core/containers/pair.h
	../interface/core/containers/shared_pointer.h
	../interface/core/containers/tuple_element.h
	../interface/core/containers/tuple_size.h
	../interface/core/containers/tuple.h
	../interface/core/containers/unique_pointer.h
	../interface/core/hash/city_hash.h
	../interface/core/hash/crc32.h
	../interface/core/iterators/random_access_iterator.h
	../interface/core/math/math.h
	../interface/core/memory/memory.h
	../interface/core/os_windows/windows_api.h
	../interface/core/string/cstring/cstring_common.h
	../interface/core/string/cstring/cstring_linux.h
	../interface/core/string/cstring/cstring_windows.h
	../interface/core/string/cstring/cstring.h
	../interface/core/templates/bit_cast.h
	../interface/core/templates/declval.h
	../interface/core/templates/default_deleter.h
	../interface/core/templates/equal.h
	../interface/core/templates/exchange.h
	../interface/core/templates/forward.h
	../interface/core/templates/greater.h
	../interface/core/templates/greater_equal.h
	../interface/core/templates/integer_sequence.h
	../interface/core/templates/less.h
	../interface/core/templates/less_equal.h
	../interface/core/templates/lexicographical_compare.h
	../interface/core/templates/max.h
	../interface/core/templates/min.h
	../interface/core/templates/move.h
	../interface/core/templates/select_deleter_pointer_type.h
	../interface/core/templates/swap.h
	../interface/core/templates/tag_in_place.h
	../interface/core/templates/tag_init.h
	../interface/core/templates/tag_in_place.h
	../interface/core/traits/add_const.h
	../interface/core/traits/add_cv.h
	../interface/core/traits/add_lvalue_reference.h
	../interface/core/traits/add_pointer.h
	../interface/core/traits/add_rvalue_reference.h
	../interface/core/traits/add_volatile.h
	../interface/core/traits/conjunction.h
	../interface/core/traits/apply_cv.h
	../interface/core/traits/common_type.h
	../interface/core/traits/conditional.h
	../interface/core/traits/decay.h
	../interface/core/traits/enable_if.h
	../interface/core/traits/extent.h
	../interface/core/traits/has_unique_object_representations.h
	../interface/core/traits/integral_constant.h
	../interface/core/traits/is_one_of_types.h
	../interface/core/traits/is_arithmetic.h
	../interface/core/traits/is_array.h
	../interface/core/traits/is_assignable.h
	../interface/core/traits/is_base_of.h
	../interface/core/traits/is_bitwise_comparable.h
	../interface/core/traits/is_bitwise_convertible.h
	../interface/core/traits/is_bitwise_copyable.h
	../interface/core/traits/is_bitwise_copy_assignable.h
	../interface/core/traits/is_bitwise_copy_constructible.h
	../interface/core/traits/is_bitwise_move_assignable.h
	../interface/core/traits/is_bool.h
	../interface/core/traits/is_bounded_array.h
	../interface/core/traits/is_class.h
	../interface/core/traits/is_comparable_with_equal_operator.h
	../interface/core/traits/is_comparable_with_greater_operator.h
	../interface/core/traits/is_comparable_with_greater_equal_operator.h
	../interface/core/traits/is_comparable_with_less_operator.h
	../interface/core/traits/is_comparable_with_less_equal_operator.h
	../interface/core/traits/is_comparable_with_not_equal_operator.h
	../interface/core/traits/is_const.h
	../interface/core/traits/is_constant_evaluated.h
	../interface/core/traits/is_constructible.h
	../interface/core/traits/is_convertible.h
	../interface/core/traits/is_copy_assignable.h
	../interface/core/traits/is_copy_constructible.h
	../interface/core/traits/is_default_constructible.h
	../interface/core/traits/is_derived_from.h
	../interface/core/traits/is_destructible.h
	../interface/core/traits/is_empty.h
	../interface/core/traits/is_enum.h
	../interface/core/traits/is_explicitly_constructible.h
	../interface/core/traits/is_explicitly_copy_constructible.h
	../interface/core/traits/is_explicitly_default_constructible.h
	../interface/core/traits/is_explicitly_move_constructible.h
	../interface/core/traits/is_floating_point.h
	../interface/core/traits/is_function.h
	../interface/core/traits/is_implicitly_constructible.h
	../interface/core/traits/is_implicitly_copy_constructible.h
	../interface/core/traits/is_implicitly_default_constructible.h
	../interface/core/traits/is_implicitly_move_constructible.h
	../interface/core/traits/is_integral.h
	../interface/core/traits/is_lvalue_reference.h
	../interface/core/traits/is_member_function_pointer.h
	../interface/core/traits/is_member_object_pointer.h
	../interface/core/traits/is_member_pointer.h
	../interface/core/traits/is_move_assignable.h
	../interface/core/traits/is_move_constructible.h
	../interface/core/traits/is_nothrow_assignable.h
	../interface/core/traits/is_nothrow_constructible.h
	../interface/core/traits/is_nothrow_copy_assignable.h
	../interface/core/traits/is_nothrow_copy_constructible.h
	../interface/core/traits/is_nothrow_default_constructible.h
	../interface/core/traits/is_nothrow_destructible.h
	../interface/core/traits/is_nothrow_move_assignable.h
	../interface/core/traits/is_nothrow_move_constructible.h
	../interface/core/traits/is_nothrow_swappable.h
	../interface/core/traits/is_not_same.h
	../interface/core/traits/is_nullptr.h
	../interface/core/traits/is_object.h
	../interface/core/traits/is_pointer.h
	../interface/core/traits/is_power_of_two.h
	../interface/core/traits/is_reference.h
	../interface/core/traits/is_referenceable.h
	../interface/core/traits/is_rvalue_reference.h
	../interface/core/traits/is_same.h
	../interface/core/traits/is_same_size.h
	../interface/core/traits/is_scalar.h
	../interface/core/traits/is_signed.h
	../interface/core/traits/is_standard_layout.h
	../interface/core/traits/is_swappable.h
	../interface/core/traits/is_trivial.h
	../interface/core/traits/is_trivially_assignable.h
	../interface/core/traits/is_trivially_constructible.h
	../interface/core/traits/is_trivially_copyable.h
	../interface/core/traits/is_trivially_copy_assignable.h
	../interface/core/traits/is_trivially_copy_constructible.h
	../interface/core/traits/is_trivially_default_constructible.h
	../interface/core/traits/is_trivially_destructible.h
	../interface/core/traits/is_trivially_move_assignable.h
	../interface/core/traits/is_trivially_move_constructible.h
	../interface/core/traits/is_trivially_swappable.h
	../interface/core/traits/is_unbounded_array.h
	../interface/core/traits/is_union.h
	../interface/core/traits/is_unsigned.h
	../interface/core/traits/is_void.h
	../interface/core/traits/is_volatile.h
	../interface/core/traits/make_signed.h
	../interface/core/traits/make_unsigned.h
	../interface/core/traits/negation.h
	../interface/core/traits/disjunction.h
	../interface/core/traits/remove_all_extents.h
	../interface/core/traits/remove_const.h
	../interface/core/traits/remove_cv.h
	../interface/core/traits/remove_cv_ref.h
	../interface/core/traits/remove_extent.h
	../interface/core/traits/remove_pointer.h
	../interface/core/traits/remove_reference.h
	../interface/core/traits/remove_volatile.h
	../interface/core/traits/underlying_type.h
	../interface/core/traits/void_t.h
	../interface/core/traits/void_t.h
	../interface/core/types/types_common.h
	../interface/core/types/types_linux.h
	../interface/core/types/types_windows.h
	../interface/core/types/types.h
	../interface/core/uuid/uuid_common.h
	../interface/core/uuid/uuid_linux.h
	../interface/core/uuid/uuid_windows.h
	../interface/core/uuid/uuid.h
	../interface/core/assert.h
	../interface/core/atomics.h
	../interface/core/character.h
	../interface/core/compilation.h
	../interface/core/compiler_defines.h
	../interface/core/containers
	../interface/core/cstring.h
	../interface/core/debugger.h
	../interface/core/defines.h
	../interface/core/hash.h
	../interface/core/limits.h
	../interface/core/i128.h
	../interface/core/iterators
	../interface/core/math.h
	../interface/core/memory.h
	../interface/core/minimal.h
	../interface/core/slice.h
	../interface/core/string.h
	../interface/core/templates
	../interface/core/traits
	../interface/core/types.h
	../interface/core/uuid.h
)


source_group( TREE ${CMAKE_SOURCE_DIR} FILES ${interface} ${src})

# Add OSLayer to the project
add_library(${lib_name} STATIC ${interface} ${src})

# Use C++ 20
set_target_properties(${lib_name}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

if(MSVC)
	target_compile_options( ${lib_name} PRIVATE /Zc:__cplusplus /std:c++20 /bigobj /permissive- /EHsc /utf-8 /arch:AVX2)
	target_compile_definitions(${lib_name} PRIVATE __SSE4_2__)
elseif(EMSCRIPTEN)
	target_compile_options( ${lib_name} PRIVATE -msse4.2 -msimd128 -std=c++20)
else()
 	target_compile_options( ${lib_name} PRIVATE -msse4.2 -std=c++20)
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