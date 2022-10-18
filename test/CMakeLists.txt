cmake_minimum_required(VERSION 3.18)

# Set project name
project(test_core
		DESCRIPTION "HUD Engine Core tests"
		LANGUAGES CXX
)

add_executable(test_core 
	array/add.cpp
	array/add_no_construct.cpp
	array/add_to_ref.cpp
	array/allocators.h
	array/assignment.cpp
	array/clear.cpp
	array/comparison.cpp
	array/constructors.cpp
	array/destructor.cpp
	array/emplace_at.cpp
	array/emplace_at_to_ref.cpp
	array/emplace_back.cpp
	array/emplace_back_to_ref.cpp
	array/iteration.cpp
	array/misc.cpp
	array/remove_at.cpp
	array/reserve.cpp
	array/resize.cpp
	array/search.cpp
	array/shrink_to_fit.cpp
	array/swap.cpp
	memory/alignment.cpp
	memory/allocation.cpp
	memory/compare.cpp
	memory/construct.cpp
	memory/copy_assign.cpp
	memory/copy_construct.cpp
	memory/copy_move.cpp
	memory/default_construct.cpp
	memory/destroy.cpp
	memory/fast_move_or_copy_construct_array_then_destroy.cpp
	memory/move_or_copy_assign.cpp
	memory/move_or_copy_construct_then_destroy.cpp
	memory/move_or_copy_construct_then_destroy_backward.cpp
	memory/reverse.cpp
	memory/rotate.cpp
	memory/set_zero.cpp
	misc/default_constructible_type.h
	misc/for_each_type.h
	misc/for_each_value.h
	misc/leak_guard.h
	misc/non_bitwise_copy_assignable_type.h
	misc/non_bitwise_copy_constructible_type.h
	misc/non_bitwise_move_assignable_type.h
	misc/non_bitwise_move_constructible_type.h
	misc/non_bitwise_type.h
	misc/non_default_constructible_type.h
	misc/set_bool_to_true_when_destroyed.h
	optional/assignment.cpp
	optional/comparison.cpp
	optional/constructors.cpp
	optional/emplace.cpp
	optional/misc.cpp
	optional/swap.cpp
	pair/assignments.cpp
	pair/comparison.cpp
	pair/constructors.cpp
	pair/misc.cpp
	pair/swap.cpp
	shared_pointer/multi_thread/assignments.cpp
	shared_pointer/multi_thread/comparison.cpp
	shared_pointer/multi_thread/constructors.cpp
	shared_pointer/multi_thread/destructor.cpp
	shared_pointer/multi_thread/misc.cpp
	shared_pointer/multi_thread/swap.cpp
	shared_pointer/multi_thread_array/assignments.cpp
	shared_pointer/multi_thread_array/comparison.cpp
	shared_pointer/multi_thread_array/constructors.cpp
	shared_pointer/multi_thread_array/destructor.cpp
	shared_pointer/multi_thread_array/misc.cpp
	shared_pointer/multi_thread_array/swap.cpp
	shared_pointer/single_thread/assignments.cpp
	shared_pointer/single_thread/comparison.cpp
	shared_pointer/single_thread/constructors.cpp
	shared_pointer/single_thread/destructor.cpp
	shared_pointer/single_thread/misc.cpp
	shared_pointer/single_thread/swap.cpp
	shared_pointer/single_thread_array/assignments.cpp
	shared_pointer/single_thread_array/comparison.cpp
	shared_pointer/single_thread_array/constructors.cpp
	shared_pointer/single_thread_array/destructor.cpp
	shared_pointer/single_thread_array/misc.cpp
	shared_pointer/single_thread_array/swap.cpp
	templates/bit_cast.cpp
	templates/declval.cpp
	templates/default_deleter.cpp
	templates/equal.cpp
	templates/forward.cpp
	templates/greater.cpp
	templates/greater_equal.cpp
	templates/integer_sequence.cpp
	templates/less.cpp
	templates/less_equal.cpp
	templates/lexicographical_compare.cpp
	templates/max.cpp
	templates/min.cpp
	templates/move.cpp
	templates/select_deleter_pointer_type.cpp
	templates/swap.cpp
	traits/add_const.cpp
	traits/add_cv.cpp
	traits/add_lvalue_reference.cpp
	traits/add_pointer.cpp
	traits/add_rvalue_reference.cpp
	traits/add_volatile.cpp
	traits/conjunction.cpp
	traits/apply_cv.cpp
	traits/common_type.cpp
	traits/conditional.cpp
	traits/decay.cpp
	traits/enable_if.cpp
	traits/extent.cpp
	traits/has_unique_object_representations.cpp
	traits/integral_constant.cpp
	traits/is_any_type_in.cpp
	traits/is_arithmetic.cpp
	traits/is_array.cpp
	traits/is_assignable.cpp
	traits/is_base_of.cpp
	traits/is_bitwise_comparable.cpp
	traits/is_bitwise_convertible.cpp
	traits/is_bitwise_copy_assignable.cpp
	traits/is_bitwise_copy_constructible.cpp
	traits/is_bitwise_move_assignable.cpp
	traits/is_bitwise_move_constructible.cpp
	traits/is_bool.cpp
	traits/is_bounded_array.cpp
	traits/is_class.cpp
	traits/is_comparable_with_equal.cpp
	traits/is_comparable_with_greater.cpp
	traits/is_comparable_with_greater_equal.cpp
	traits/is_comparable_with_less.cpp
	traits/is_comparable_with_less_equal.cpp
	traits/is_comparable_with_not_equal.cpp
	traits/is_const.cpp
	traits/is_constant_evaluated.cpp
	traits/is_constructible.cpp
	traits/is_convertible.cpp
	traits/is_copy_assignable.cpp
	traits/is_copy_constructible.cpp
	traits/is_default_constructible..cpp
	traits/is_derived_from.cpp
	traits/is_destructible.cpp
	traits/is_enum.cpp
	traits/is_explicitly_constructible.cpp
	traits/is_explicitly_copy_constructible.cpp
	traits/is_explicitly_default_constructible.cpp
	traits/is_explicitly_move_constructible.cpp
	traits/is_floating_point.cpp
	traits/is_function.cpp
	traits/is_implicitly_constructible.cpp
	traits/is_implicitly_copy_constructible.cpp
	traits/is_implicitly_default_constructible.cpp
	traits/is_implicitly_move_constructible.cpp
	traits/is_integral.cpp
	traits/is_lvalue_reference.cpp
	traits/is_member_function_pointer.cpp
	traits/is_member_object_pointer.cpp
	traits/is_member_pointer.cpp
	traits/is_move_assignable.cpp
	traits/is_move_constructible.cpp
	traits/is_nothrow_assignable.cpp
	traits/is_nothrow_constructible.cpp
	traits/is_nothrow_copy_assignable.cpp
	traits/is_nothrow_copy_constructible.cpp
	traits/is_nothrow_default_constructible.cpp
	traits/is_nothrow_destructible.cpp
	traits/is_nothrow_move_assignable.cpp
	traits/is_nothrow_move_constructible.cpp
	traits/is_nothrow_swappable.cpp
	traits/is_not_same.cpp
	traits/is_nullptr.cpp
	traits/is_object.cpp
	traits/is_pointer.cpp
	traits/is_reference.cpp
	traits/is_referenceable.cpp
	traits/is_rvalue_reference.cpp
	traits/is_same.cpp
	traits/is_same_size.cpp
	traits/is_scalar.cpp
	traits/is_signed.cpp
	traits/is_standard_layout.cpp
	traits/is_swappable.cpp
	traits/is_trivial.cpp
	traits/is_trivially_assignable.cpp
	traits/is_trivially_constructible.cpp
	traits/is_trivially_copyable.cpp
	traits/is_trivially_copy_assignable.cpp
	traits/is_trivially_copy_constructible.cpp
	traits/is_trivially_default_constructible.cpp
	traits/is_trivially_destructible.cpp
	traits/is_trivially_move_assignable.cpp
	traits/is_trivially_move_constructible.cpp
	traits/is_trivially_swappable.cpp
	traits/is_unbounded_array.cpp
	traits/is_union.cpp
	traits/is_unsigned.cpp
	traits/is_void.cpp
	traits/is_volatile.cpp
	traits/make_signed.cpp
	traits/make_unsigned.cpp
	traits/negation.cpp
	traits/disjunction.cpp
	traits/remove_all_extents.cpp
	traits/remove_const.cpp
	traits/remove_cv.cpp
	traits/remove_cv_ref.cpp
	traits/remove_extent.cpp
	traits/remove_pointer.cpp
	traits/remove_reference.cpp
	traits/remove_volatile.cpp
	traits/underlying_type.cpp
	traits/void_t.cpp
	tuple/assignments.cpp
	tuple/comparison.cpp
	tuple/constructors.cpp
	tuple/misc.cpp
	tuple/swap.cpp
	tuple/tuple_element.cpp
	tuple/tuple_size.cpp
	unique_pointer/array/assignments.cpp
	unique_pointer/array/comparison.cpp
	unique_pointer/array/constructors.cpp
	unique_pointer/array/destructor.cpp
	unique_pointer/array/misc.cpp
	unique_pointer/array/swap.cpp
	unique_pointer/single/assignments.cpp
	unique_pointer/single/comparison.cpp
	unique_pointer/single/constructors.cpp
	unique_pointer/single/destructor.cpp
	unique_pointer/single/misc.cpp
	unique_pointer/single/swap.cpp
	aligned_buffer.cpp
	aligned_heap_allocator.cpp
	allocation.cpp
	atomics.cpp
	character.cpp
	compilation.cpp
	cstring.cpp
	hash.cpp
	heap_allocator.cpp
	main.cpp
	precompiled.h
	random_access_iterator.cpp
	slice.cpp
	types.cpp
	uuid.cpp
)

set_target_properties(test_core
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
if(MSVC)
	target_compile_options( test_core PRIVATE /Zc:__cplusplus /std:c++20 /bigobj)
endif()

if(COVERAGE AND MSVC)
	target_link_options(test_core PRIVATE /PROFILE)
	add_custom_command(
		TARGET test_core POST_BUILD
		COMMENT "Instrument and Collect test_core.exe"
		COMMAND ${MSVC_CODECOVERAGE_CONSOLE} instrument Debug\\test_core.exe -s ..\\..\\test\\coverage.runsettings
		COMMAND ${MSVC_CODECOVERAGE_CONSOLE} collect Debug\\test_core.exe -o Debug\\coverage -f cobertura -s ..\\..\\test\\coverage.runsettings
	)
endif()

target_precompile_headers(test_core PRIVATE precompiled.h)

# Add defines
target_compile_definitions(
	test_core
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
target_link_libraries(test_core PRIVATE core)

# Add Google test dependency
message("Fetching HUD-Software/google-test...")
include(FetchContent)
FetchContent_Declare(
	google_test 
	GIT_REPOSITORY  git@github.com:HUD-Software/google-test.git
	GIT_TAG         5306f1a0e51f6001c624588fafdb646bb377866c
)
FetchContent_MakeAvailable(google_test)

target_link_libraries( test_core PRIVATE gtest )

# Add Intel slice-by-8 dependency
message("Fetching HUD-Software/slice-by-8...")
include(FetchContent)
FetchContent_Declare(
    slice_by_8 
    GIT_REPOSITORY  git@github.com:HUD-Software/slice-by-8.git
    GIT_TAG         536f6c5a297ff6263c56944c9bb04358f5b0d7da
)
FetchContent_MakeAvailable(slice_by_8)

target_link_libraries( test_core PRIVATE slice_by_8 )

# Add Intel cityhash dependency
message("Fetching HUD-Software/cityhash...")
include(FetchContent)
FetchContent_Declare(
    cityhash
    GIT_REPOSITORY  git@github.com:HUD-Software/cityhash.git
    GIT_TAG         01bb23df43fecd290b398c2e4314dff4c0dc0672
)
FetchContent_MakeAvailable(cityhash)

target_link_libraries( test_core PRIVATE cityhash )

# Add the executable test
add_test(NAME core COMMAND test_core --gtest_output=xml:test_core_report.xml --extra-verbose --gtest_break_on_failure)
