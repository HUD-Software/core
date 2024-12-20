cmake_minimum_required(VERSION 3.24)

set(test_project_name test_core)

set( src
	allocator/aligned_heap_allocator.cpp
	allocator/allocation.cpp
	allocator/heap_allocator.cpp
	array/array_add.cpp
	array/array_assign_copy.cpp
	array/array_add_no_construct.cpp
	array/array_add_to_ref.cpp
	array/array_assignment.cpp
	array/array_clear.cpp
	array/array_comparison.cpp
	array/array_constructors.cpp
	array/array_copy_assign_operator.cpp
	array/array_copy_constructors.cpp
	array/array_destructor.cpp
	array/array_emplace_at.cpp
	array/array_emplace_at_to_ref.cpp
	array/array_emplace_back.cpp
	array/array_emplace_back_to_ref.cpp
	array/array_iteration.cpp
	array/array_misc.cpp
	array/array_move_assign_operator.cpp
	array/array_move_assign.cpp
	array/array_move_constructors.cpp
	array/array_remove_at.cpp
	array/array_reserve.cpp
	array/array_resize.cpp
	array/array_search.cpp
	array/array_shrink_to_fit.cpp
	array/array_swap.cpp
	atomic/atomic_add.cpp
	atomic/atomic_assign.cpp
	atomic/atomic_cast.cpp
	atomic/atomic_compare_exchange.cpp
	atomic/atomic_constructor.cpp
	atomic/atomic_decrement.cpp
	atomic/atomic_exchange.cpp
	atomic/atomic_increment.cpp
	atomic/atomic_load.cpp
	atomic/atomic_store.cpp
	atomic/atomic_sub.cpp
	atomics/atomics_compare_and_set.cpp
	atomics/atomics_compare_and_swap.cpp
	atomics/atomics_exchange.cpp
	atomics/atomics_fetch_add_sub.cpp
	atomics/atomics_load.cpp
	atomics/atomics_store.cpp
	atomics/atomics_thread_fence.cpp
	bits/bits_leading_zero.cpp
	bits/bits_reverse.cpp
	bits/bits_rotate.cpp
	bits/bits_trailing_zero.cpp
	bits/bits.cpp
	compressed_pair/compressed_pair_constructors.cpp
	compressed_pair/compressed_pair_misc.cpp
	compressed_pair/compressed_pair_swap.cpp
	compressed_pair/compressed_pair_assignments.cpp
	compressed_pair/compressed_pair_comparison.cpp
	hash/city_hash.cpp
	hash/crc32.cpp
	hash/hash_32.cpp
	hash/hash_64.cpp
	hashmap/hashmap_insert.cpp
	hashmap/hashmap_constructors.cpp
	hashmap/hashmap_misc.cpp
	hashmap/hashmap_find.cpp
	hashset/hashset_insert.cpp
	hashset/hashset_constructors.cpp
	hashset/hashset_misc.cpp
	i128/i128_assign.cpp
	i128/i128_cast.cpp
	i128/i128_comparisons.cpp
	i128/i128_constructors.cpp
	i128/i128_operations.cpp
	i128/i128_misc.cpp
	iterators/random_access_iterator.cpp
	memory/memory_alignment.cpp
	memory/memory_allocation.cpp
	memory/memory_compare.cpp
	memory/memory_construct.cpp
	memory/memory_copy_assign.cpp
	memory/memory_copy_construct.cpp
	memory/memory_copy_move.cpp
	memory/memory_default_construct.cpp
	memory/memory_destroy.cpp
	memory/memory_fast_move_or_copy_construct_array_then_destroy.cpp
	memory/memory_move_or_copy_assign.cpp
	memory/memory_move_or_copy_construct_then_destroy.cpp
	memory/memory_move_or_copy_construct_then_destroy_backward.cpp
	memory/memory_set_zero.cpp
	optional/optional_assignment.cpp
	optional/optional_comparison.cpp
	optional/optional_constructors.cpp
	optional/optional_emplace.cpp
	optional/optional_misc.cpp
	optional/optional_swap.cpp
	pair/pair_assignments.cpp
	pair/pair_comparison.cpp
	pair/pair_constructors.cpp
	pair/pair_misc.cpp
	pair/pair_swap.cpp
	shared_pointer/multi_thread/shared_pointer_multi_thread_assignments.cpp
	shared_pointer/multi_thread/shared_pointer_multi_thread_comparison.cpp
	shared_pointer/multi_thread/shared_pointer_multi_thread_constructors.cpp
	shared_pointer/multi_thread/shared_pointer_multi_thread_destructor.cpp
	shared_pointer/multi_thread/shared_pointer_multi_thread_misc.cpp
	shared_pointer/multi_thread/shared_pointer_multi_thread_swap.cpp
	shared_pointer/multi_thread_array/multi_thread_array_assignments.cpp
	shared_pointer/multi_thread_array/multi_thread_array_comparison.cpp
	shared_pointer/multi_thread_array/multi_thread_array_constructors.cpp
	shared_pointer/multi_thread_array/multi_thread_array_destructor.cpp
	shared_pointer/multi_thread_array/multi_thread_array_misc.cpp
	shared_pointer/multi_thread_array/multi_thread_array_swap.cpp
	shared_pointer/single_thread/single_thread_assignments.cpp
	shared_pointer/single_thread/single_thread_comparison.cpp
	shared_pointer/single_thread/single_thread_constructors.cpp
	shared_pointer/single_thread/single_thread_destructor.cpp
	shared_pointer/single_thread/single_thread_misc.cpp
	shared_pointer/single_thread/single_thread_swap.cpp
	shared_pointer/single_thread_array/single_thread_array_assignments.cpp
	shared_pointer/single_thread_array/single_thread_array_comparison.cpp
	shared_pointer/single_thread_array/single_thread_array_constructors.cpp
	shared_pointer/single_thread_array/single_thread_array_destructor.cpp
	shared_pointer/single_thread_array/single_thread_array_misc.cpp
	shared_pointer/single_thread_array/single_thread_array_swap.cpp
	templates/templates_exchange.cpp
	templates/templates_bit_cast.cpp
	templates/templates_declval.cpp
	templates/templates_default_deleter.cpp
	templates/templates_equal.cpp
	templates/templates_forward.cpp
	templates/templates_greater.cpp
	templates/templates_greater_equal.cpp
	templates/templates_integer_sequence.cpp
	templates/templates_less.cpp
	templates/templates_less_equal.cpp
	templates/templates_lexicographical_compare.cpp
	templates/templates_max.cpp
	templates/templates_min.cpp
	templates/templates_move.cpp
	templates/templates_select_deleter_pointer_type.cpp
	templates/templates_swap.cpp
	traits/traits_add_const.cpp
	traits/traits_add_cv.cpp
	traits/traits_add_lvalue_reference.cpp
	traits/traits_add_pointer.cpp
	traits/traits_add_rvalue_reference.cpp
	traits/traits_add_volatile.cpp
	traits/traits_conjunction.cpp
	traits/traits_apply_cv.cpp
	traits/traits_common_type.cpp
	traits/traits_conditional.cpp
	traits/traits_decay.cpp
	traits/traits_enable_if.cpp
	traits/traits_extent.cpp
	traits/traits_has_unique_object_representations.cpp
	traits/traits_integral_constant.cpp
	traits/traits_is_one_of_types.cpp
	traits/traits_is_arithmetic.cpp
	traits/traits_is_array.cpp
	traits/traits_is_assignable.cpp
	traits/traits_is_base_of.cpp
	traits/traits_is_bitwise_comparable.cpp
	traits/traits_is_bitwise_convertible.cpp
	traits/traits_is_bitwise_copy_assignable.cpp
	traits/traits_is_bitwise_copy_constructible.cpp
	traits/traits_is_bitwise_move_assignable.cpp
	traits/traits_is_bitwise_move_constructible.cpp
	traits/traits_is_bool.cpp
	traits/traits_is_bounded_array.cpp
	traits/traits_is_class.cpp
	traits/traits_is_comparable_with_equal.cpp
	traits/traits_is_comparable_with_greater.cpp
	traits/traits_is_comparable_with_greater_equal.cpp
	traits/traits_is_comparable_with_less.cpp
	traits/traits_is_comparable_with_less_equal.cpp
	traits/traits_is_comparable_with_not_equal.cpp
	traits/traits_is_const.cpp
	traits/traits_is_constant_evaluated.cpp
	traits/traits_is_constructible.cpp
	traits/traits_is_convertible.cpp
	traits/traits_is_copy_assignable.cpp
	traits/traits_is_copy_constructible.cpp
	traits/traits_is_default_constructible..cpp
	traits/traits_is_derived_from.cpp
	traits/traits_is_destructible.cpp
	traits/traits_is_empty.cpp
	traits/traits_is_enum.cpp
	traits/traits_is_explicitly_constructible.cpp
	traits/traits_is_explicitly_copy_constructible.cpp
	traits/traits_is_explicitly_default_constructible.cpp
	traits/traits_is_explicitly_move_constructible.cpp
	traits/traits_is_floating_point.cpp
	traits/traits_is_function.cpp
	traits/traits_is_implicitly_constructible.cpp
	traits/traits_is_implicitly_copy_constructible.cpp
	traits/traits_is_implicitly_default_constructible.cpp
	traits/traits_is_implicitly_move_constructible.cpp
	traits/traits_is_integral.cpp
	traits/traits_is_lvalue_reference.cpp
	traits/traits_is_member_function_pointer.cpp
	traits/traits_is_member_object_pointer.cpp
	traits/traits_is_member_pointer.cpp
	traits/traits_is_move_assignable.cpp
	traits/traits_is_move_constructible.cpp
	traits/traits_is_nothrow_assignable.cpp
	traits/traits_is_nothrow_constructible.cpp
	traits/traits_is_nothrow_copy_assignable.cpp
	traits/traits_is_nothrow_copy_constructible.cpp
	traits/traits_is_nothrow_default_constructible.cpp
	traits/traits_is_nothrow_destructible.cpp
	traits/traits_is_nothrow_move_assignable.cpp
	traits/traits_is_nothrow_move_constructible.cpp
	traits/traits_is_nothrow_swappable.cpp
	traits/traits_is_not_same.cpp
	traits/traits_is_nullptr.cpp
	traits/traits_is_object.cpp
	traits/traits_is_pointer.cpp
	traits/traits_is_reference.cpp
	traits/traits_is_referenceable.cpp
	traits/traits_is_rvalue_reference.cpp
	traits/traits_is_same.cpp
	traits/traits_is_same_size.cpp
	traits/traits_is_scalar.cpp
	traits/traits_is_signed.cpp
	traits/traits_is_standard_layout.cpp
	traits/traits_is_swappable.cpp
	traits/traits_is_trivial.cpp
	traits/traits_is_trivially_assignable.cpp
	traits/traits_is_trivially_constructible.cpp
	traits/traits_is_trivially_copyable.cpp
	traits/traits_is_trivially_copy_assignable.cpp
	traits/traits_is_trivially_copy_constructible.cpp
	traits/traits_is_trivially_default_constructible.cpp
	traits/traits_is_trivially_destructible.cpp
	traits/traits_is_trivially_move_assignable.cpp
	traits/traits_is_trivially_move_constructible.cpp
	traits/traits_is_trivially_swappable.cpp
	traits/traits_is_unbounded_array.cpp
	traits/traits_is_union.cpp
	traits/traits_is_unsigned.cpp
	traits/traits_is_void.cpp
	traits/traits_is_volatile.cpp
	traits/traits_make_signed.cpp
	traits/traits_make_unsigned.cpp
	traits/traits_negation.cpp
	traits/traits_disjunction.cpp
	traits/traits_remove_all_extents.cpp
	traits/traits_remove_const.cpp
	traits/traits_remove_cv.cpp
	traits/traits_remove_cv_ref.cpp
	traits/traits_remove_extent.cpp
	traits/traits_remove_pointer.cpp
	traits/traits_remove_reference.cpp
	traits/traits_remove_volatile.cpp
	traits/traits_underlying_type.cpp
	traits/traits_void_t.cpp
	tuple/tuple_assignments.cpp
	tuple/tuple_comparison.cpp
	tuple/tuple_constructors.cpp
	tuple/tuple_misc.cpp
	tuple/tuple_swap.cpp
	tuple/tuple_element.cpp
	tuple/tuple_size.cpp
	u128/u128_assign.cpp
	u128/u128_cast.cpp
	u128/u128_comparisons.cpp
	u128/u128_constructors.cpp
	u128/u128_operations.cpp
	u128/u128_misc.cpp
	unique_pointer/array/unique_pointer_array_assignments.cpp
	unique_pointer/array/unique_pointer_array_comparison.cpp
	unique_pointer/array/unique_pointer_array_constructors.cpp
	unique_pointer/array/unique_pointer_array_destructor.cpp
	unique_pointer/array/unique_pointer_array_misc.cpp
	unique_pointer/array/unique_pointer_array_swap.cpp
	unique_pointer/single/unique_pointer_single_assignments.cpp
	unique_pointer/single/unique_pointer_single_comparison.cpp
	unique_pointer/single/unique_pointer_single_constructors.cpp
	unique_pointer/single/unique_pointer_single_destructor.cpp
	unique_pointer/single/unique_pointer_single_misc.cpp
	unique_pointer/single/unique_pointer_single_swap.cpp
	weak_pointer/multi_thread/weak_pointer_multi_thread_constructors.cpp
	weak_pointer/multi_thread/weak_pointer_multi_thread_assignments.cpp
	weak_pointer/multi_thread/weak_pointer_multi_thread_destructor.cpp
	weak_pointer/multi_thread/weak_pointer_multi_thread_swap.cpp
	weak_pointer/multi_thread_array/weak_pointer_multi_thread_array_constructors.cpp
	weak_pointer/multi_thread_array/weak_pointer_multi_thread_array_assignments.cpp
	weak_pointer/multi_thread_array/weak_pointer_multi_thread_array_destructor.cpp
	weak_pointer/multi_thread_array/weak_pointer_multi_thread_array_swap.cpp
	weak_pointer/single_thread/weak_pointer_single_thread_constructors.cpp
	weak_pointer/single_thread/weak_pointer_single_thread_assignments.cpp
	weak_pointer/single_thread/weak_pointer_single_thread_destructor.cpp
	weak_pointer/single_thread/weak_pointer_single_thread_misc.cpp
	weak_pointer/single_thread/weak_pointer_single_thread_swap.cpp
	weak_pointer/single_thread_array/weak_pointer_single_thread_array_constructors.cpp
	weak_pointer/single_thread_array/weak_pointer_single_thread_array_assignments.cpp
	weak_pointer/single_thread_array/weak_pointer_single_thread_array_destructor.cpp
	weak_pointer/single_thread_array/weak_pointer_single_thread_array_misc.cpp
	weak_pointer/single_thread_array/weak_pointer_single_thread_array_swap.cpp
	aligned_buffer.cpp
	character.cpp
	compilation.cpp
	cstring.cpp
	debugger.cpp
	main.cpp
	math.cpp
	precompiled.h
	slice.cpp
	types.cpp
	uuid.cpp
)

add_executable(${test_project_name} ${src})

# set_target_properties(${test_project_name} PROPERTIES
#                       UNITY_BUILD_MODE BATCH
#                       UNITY_BUILD_BATCH_SIZE 8
#                       )

# set_target_properties(${test_project_name} PROPERTIES UNITY_BUILD ON UNITY_BUILD_MODE BATCH)

set_target_properties(${test_project_name}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
if(MSVC)
	#target_compile_options( ${test_project_name} PRIVATE /Zc:__cplusplus /std:c++20 /bigobj /permissive- /Os /EHsc /GR- /utf-8)
	target_compile_options( ${test_project_name} PRIVATE /Zc:__cplusplus /std:c++20 /bigobj /permissive- /EHsc /utf-8 )
endif()

get_target_property(core_type ${lib_name} TYPE)


	# if(MSVC)
	# 	if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# 		# MSVC ASAN is limited
	# 		# https://devblogs.microsoft.com/cppblog/addresssanitizer-asan-for-windows-with-msvc/#compiling-with-asan-from-the-console
	# 		target_compile_options(${test_project_name} PRIVATE /fsanitize=address /INCREMENTAL:NO /Zi)
	# 		target_compile_options(${lib_name} PRIVATE /fsanitize=address /INCREMENTAL:NO /Zi)
	# 		target_compile_definitions(${test_project_name} PRIVATE _DISABLE_VECTOR_ANNOTATION)
	# 		get_filename_component(CMAKE_CXX_COMPILER_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)
	# 		add_custom_command(TARGET ${test_project_name} POST_BUILD 
	# 			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CXX_COMPILER_PATH}/clang_rt.asan_dbg_dynamic-x86_64.dll $<TARGET_FILE_DIR:${test_project_name}>
	# 			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CXX_COMPILER_PATH}/clang_rt.asan_dynamic-x86_64.dll $<TARGET_FILE_DIR:${test_project_name}>
	# 		)
	# 	endif()
	# 	if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	# 		message( error FATAL_ERROR "ASAN with Clang-cl is not supported")
	# 		# https://github.com/aminya/project_options/issues/138
	# 		# https://stackoverflow.com/questions/66531482/application-crashes-when-using-address-sanitizer-with-msvc
	# 		# https://devblogs.microsoft.com/cppblog/asan-for-windows-x64-and-debug-build-support/
	# 		# https://learn.microsoft.com/en-us/cpp/sanitizers/asan-runtime?view=msvc-170
	# 	endif()
	# elseif( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# # https://developers.redhat.com/blog/2021/05/05/memory-error-checking-in-c-and-c-comparing-sanitizers-and-valgrind
	# 	target_compile_options(${test_project_name} PRIVATE -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment)
	# 	target_link_options(${test_project_name} PRIVATE -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment)
	# 	target_compile_options(${lib_name} PRIVATE -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment)
	# 	target_link_options(${lib_name} PRIVATE -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment)
	# endif()
	# # if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

	# # 	# MSVC ASAN is limited
	# # 	# https://devblogs.microsoft.com/cppblog/addresssanitizer-asan-for-windows-with-msvc/#compiling-with-asan-from-the-console
	# # 	target_compile_options(${test_project_name} PRIVATE /fsanitize=address)
	# # 	target_compile_options(${lib_name} PRIVATE /fsanitize=address)
	# # 	target_compile_definitions(${test_project_name} PRIVATE _DISABLE_VECTOR_ANNOTATION)
	# # 	get_filename_component(CMAKE_CXX_COMPILER_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)
	# # 	add_custom_command(TARGET ${test_project_name} POST_BUILD 
	# # 		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CXX_COMPILER_PATH}/clang_rt.asan_dbg_dynamic-x86_64.dll $<TARGET_FILE_DIR:${test_project_name}>
	# # 		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CXX_COMPILER_PATH}/clang_rt.asan_dynamic-x86_64.dll $<TARGET_FILE_DIR:${test_project_name}>)
	# # endif()
	# # if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# # 	# https://developers.redhat.com/blog/2021/05/05/memory-error-checking-in-c-and-c-comparing-sanitizers-and-valgrind
	# # 	target_compile_options(${test_project_name} PRIVATE -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment)
	# # 	target_link_options(${test_project_name} PRIVATE -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment)
	# # 	target_compile_options(${lib_name} PRIVATE -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment)
	# # 	target_link_options(${lib_name} PRIVATE -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment)
	# # endif()

##endif()


#enable_coverage(${test_project_name} ${lib_name})

# if(COVERAGE AND MSVC)
# 	if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
# 		set(MSVC_CODECOVERAGE_CONSOLE_PATH "C:\\Program Files\\Microsoft Visual Studio\\2022\\Enterprise\\Common7\\IDE\\Extensions\\Microsoft\\CodeCoverage.Console\\Microsoft.CodeCoverage.Console.exe" CACHE STRING "Path to Microsoft.CodeCoverage.Console.exe")
# 		find_program(MSVC_CODECOVERAGE_CONSOLE_EXE ${MSVC_CODECOVERAGE_CONSOLE_PATH})
# 		if(NOT MSVC_CODECOVERAGE_CONSOLE_EXE)
# 			message(FATAL_ERROR "Code coverage on Windows need Microsoft.CodeCoverage.Console.exe available in Visual Studio 2022 17.3 Enterprise Edition")
# 		endif()

# 		target_link_options(${test_project_name} PRIVATE /PROFILE)
# 		add_custom_command(
# 			TARGET ${test_project_name} POST_BUILD
# 			COMMENT "Instrument and Collect ${test_project_name}.exe"
# 			COMMAND ${MSVC_CODECOVERAGE_CONSOLE_EXE} instrument Debug\\${test_project_name}.exe -s ..\\..\\test\\coverage.runsettings
# 			COMMAND ${MSVC_CODECOVERAGE_CONSOLE_EXE} collect Debug\\${test_project_name}.exe -o Debug\\coverage.msvc.cobertura -f cobertura -s ..\\..\\test\\coverage.runsettings
# 		)
# 	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
# 		target_compile_options(${test_project_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
# 		target_compile_options(${lib_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
		
# 		# Add clang lib path to libraries paths
# 		get_filename_component(CMAKE_CXX_COMPILER_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)
# 		target_link_directories(${test_project_name} PRIVATE "${CMAKE_CXX_COMPILER_PATH}\\..\\lib\\clang\\${CMAKE_CXX_COMPILER_VERSION}\\lib\\windows\\")


# 		add_custom_command( 
# 		 	TARGET ${test_project_name} POST_BUILD
# 		 	COMMENT "Run ${test_project_name}.exe"
# 			COMMAND Powershell.exe Invoke-WebRequest -Uri https://github.com/mozilla/grcov/releases/download/v0.8.13/grcov-x86_64-pc-windows-msvc.zip -OutFile ./Debug/grcov-x86_64-pc-windows-msvc.zip
# 			COMMAND Powershell.exe Expand-Archive -Path ./Debug/grcov-x86_64-pc-windows-msvc.zip -DestinationPath ./Debug/
# 		 	COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE="${lib_name}.profraw" ./Debug/${test_project_name}.exe
# 			COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-profdata merge -sparse ${lib_name}.profraw -o ${lib_name}.profdata
# 			COMMAND ./Debug/grcov.exe --llvm -t html -b ./Debug/ -s ./../../
# 					--llvm-path ${CMAKE_CXX_COMPILER_PATH}
# 					--branch
# 					--keep-only "src/*" 
# 					#--keep-only "test/*" 
# 					#--ignore "test/misc/*" 
# 					--excl-start "^.*LCOV_EXCL_START.*" 
# 					--excl-stop "^.*LCOV_EXCL_STOP.*" 
# 					--excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
# 					--excl-br-start "^.*LCOV_EXCL_START.*" 
# 					--excl-br-stop "^.*LCOV_EXCL_STOP.*" 
# 					--excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
# 					-o windows
# 					..
# 			COMMAND ./Debug/grcov.exe --llvm -t lcov -b ./Debug/ -s ./../../
# 					--llvm-path ${CMAKE_CXX_COMPILER_PATH}
# 					--branch
# 					--keep-only "src/*"
# 					#--keep-only "test/*" 
# 					#--ignore "test/misc/*" 
# 					--excl-start "^.*LCOV_EXCL_START.*" 
# 					--excl-stop "^.*LCOV_EXCL_STOP.*" 
# 					--excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
# 					--excl-br-start "^.*LCOV_EXCL_START.*" 
# 					--excl-br-stop "^.*LCOV_EXCL_STOP.*" 
# 					--excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
# 					-o coverage.windows.clang.lcov.info
# 					..
# 		)

# 	endif()
# elseif(COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
# 	target_compile_options(${test_project_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
# 	target_link_options(${test_project_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
# 	target_compile_options(${lib_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
# 	target_link_options(${lib_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)

# 	add_custom_command(
# 		TARGET ${test_project_name} POST_BUILD
# 		COMMENT "Run ${test_project_name}.exe"
# 		COMMAND curl -L https://github.com/mozilla/grcov/releases/latest/download/grcov-x86_64-unknown-linux-gnu.tar.bz2 | tar jxf -
# 		COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE="${lib_name}.profraw" ./${test_project_name}
# 		COMMAND llvm-profdata merge -sparse ${lib_name}.profraw -o ${lib_name}.profdata
# 		COMMAND ./grcov --llvm -t html -b . -s ./../../
# 				--llvm-path /usr/bin/
# 				--branch
# 				--keep-only "src/*" 
# 				#--keep-only "test/*" 
# 				#--ignore "test/misc/*" 
# 				--excl-start "^.*LCOV_EXCL_START.*" 
# 				--excl-stop "^.*LCOV_EXCL_STOP.*" 
# 				--excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
# 				--excl-br-start "^.*LCOV_EXCL_START.*" 
# 				--excl-br-stop "^.*LCOV_EXCL_STOP.*" 
# 				--excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
# 				-o ubuntu
# 				..
# 		COMMAND ./grcov --llvm -t lcov -b . -s ./../../
# 				--llvm-path /usr/bin/
# 				--branch
# 				--keep-only "src/*"
# 				#--keep-only "test/*" 
# 				#--ignore "test/misc/*" 
# 				--excl-start "^.*LCOV_EXCL_START.*" 
# 				--excl-stop "^.*LCOV_EXCL_STOP.*" 
# 				--excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
# 				--excl-br-start "^.*LCOV_EXCL_START.*" 
# 				--excl-br-stop "^.*LCOV_EXCL_STOP.*" 
# 				--excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
# 				-o coverage.linux.clang.lcov.info
# 				..
# 	)
# endif()
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
	#Don't use TEST() ans ASSERT_*() macro from google_test which is a too generic name
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

# Add Catch2 test dependency
message("Fetching Catch2...")
include(FetchContent)
FetchContent_Declare(
	catch2 
	GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
	GIT_TAG         v3.7.1
)
FetchContent_MakeAvailable(catch2)
target_link_libraries( ${test_project_name} PRIVATE Catch2::Catch2 )

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
#add_custom_command(TARGET ${test_project_name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest> $<TARGET_FILE_DIR:${test_project_name}>)

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
add_test(NAME ${lib_name} COMMAND ${test_project_name} --break)
#add_test(NAME ${lib_name} COMMAND ${test_project_name} --gtest_output=xml:test_core_report.xml --extra-verbose --gtest_break_on_failure)


if(SANITIZER)
	include(../sanitizer.cmake)
	enable_sanitizer(${test_project_name} ${lib_name})
endif()

if(COVERAGE)
	include(../coverage.cmake)
	enable_gcov_coverage(${test_project_name} ${lib_name})
endif()