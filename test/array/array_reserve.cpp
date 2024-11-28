#include <core/containers/array.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(array, reserve_empty_to_zero_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.reserve(0);
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, reserve_empty_to_zero_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.reserve(0);
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, reserve_empty_to_size_allocate_only_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.reserve(2);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, reserve_empty_to_size_allocate_only_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.reserve(2);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, reserve_non_empty_to_more_size_allocate_only_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.reserve(2);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them, except moving them
    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    hud_assert_eq(array[0].id(), 10);
    hud_assert_eq(array[0].destructor_counter(), nullptr);
    hud_assert_eq(array[0].constructor_count(), 1u);
    hud_assert_eq(array[0].copy_constructor_count(), 0u);
    hud_assert_eq(array[0].move_constructor_count(), 0u);
    hud_assert_eq(array[0].copy_assign_count(), 0u);
    hud_assert_eq(array[0].move_assign_count(), 0u);

    hud_assert_eq(array[1].id(), 20);
    hud_assert_eq(array[1].destructor_counter(), nullptr);
    hud_assert_eq(array[1].constructor_count(), 1u);
    hud_assert_eq(array[1].copy_constructor_count(), 0u);
    hud_assert_eq(array[1].move_constructor_count(), 0u);
    hud_assert_eq(array[1].copy_assign_count(), 0u);
    hud_assert_eq(array[1].move_assign_count(), 0u);

    array.reserve(4);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 4u);
    hud_assert_eq(array.allocator().allocation_count(), 2u);
    hud_assert_eq(array.allocator().free_count(), 1u);

    hud_assert_eq(array[0].id(), 10);
    hud_assert_eq(array[0].destructor_counter(), nullptr);
    hud_assert_eq(array[0].constructor_count(), 1u);
    hud_assert_eq(array[0].copy_constructor_count(), 0u);
    hud_assert_eq(array[0].move_constructor_count(), 1u);
    hud_assert_eq(array[0].copy_assign_count(), 0u);
    hud_assert_eq(array[0].move_assign_count(), 0u);

    hud_assert_eq(array[1].id(), 20);
    hud_assert_eq(array[1].destructor_counter(), nullptr);
    hud_assert_eq(array[1].constructor_count(), 1u);
    hud_assert_eq(array[1].copy_constructor_count(), 0u);
    hud_assert_eq(array[1].move_constructor_count(), 1u);
    hud_assert_eq(array[1].copy_assign_count(), 0u);
    hud_assert_eq(array[1].move_assign_count(), 0u);
}

GTEST_TEST(array, reserve_non_empty_to_more_size_allocate_only_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.reserve(2);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them, except moving them
    array.emplace_back(10);
    array.emplace_back(20);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);
    hud_assert_eq(array[0], type(10));
    hud_assert_eq(array[1], type(20));

    array.reserve(4);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 4u);
    hud_assert_eq(array.allocator().allocation_count(), 2u);
    hud_assert_eq(array.allocator().free_count(), 1u);
    hud_assert_eq(array[0], type(10));
    hud_assert_eq(array[1], type(20));
}

GTEST_TEST(array, reserve_less_than_max_count_but_more_than_count_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.reserve(4);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 4u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them
    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 4u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    hud_assert_eq(array[0].id(), 10);
    hud_assert_eq(array[0].destructor_counter(), nullptr);
    hud_assert_eq(array[0].constructor_count(), 1u);
    hud_assert_eq(array[0].copy_constructor_count(), 0u);
    hud_assert_eq(array[0].move_constructor_count(), 0u);
    hud_assert_eq(array[0].copy_assign_count(), 0u);
    hud_assert_eq(array[0].move_assign_count(), 0u);

    hud_assert_eq(array[1].id(), 20);
    hud_assert_eq(array[1].destructor_counter(), nullptr);
    hud_assert_eq(array[1].constructor_count(), 1u);
    hud_assert_eq(array[1].copy_constructor_count(), 0u);
    hud_assert_eq(array[1].move_constructor_count(), 0u);
    hud_assert_eq(array[1].copy_assign_count(), 0u);
    hud_assert_eq(array[1].move_assign_count(), 0u);

    // Reserve less but more than
    array.reserve(2);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 4u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    hud_assert_eq(array[0].id(), 10);
    hud_assert_eq(array[0].destructor_counter(), nullptr);
    hud_assert_eq(array[0].constructor_count(), 1u);
    hud_assert_eq(array[0].copy_constructor_count(), 0u);
    hud_assert_eq(array[0].move_constructor_count(), 0u);
    hud_assert_eq(array[0].copy_assign_count(), 0u);
    hud_assert_eq(array[0].move_assign_count(), 0u);

    hud_assert_eq(array[1].id(), 20);
    hud_assert_eq(array[1].destructor_counter(), nullptr);
    hud_assert_eq(array[1].constructor_count(), 1u);
    hud_assert_eq(array[1].copy_constructor_count(), 0u);
    hud_assert_eq(array[1].move_constructor_count(), 0u);
    hud_assert_eq(array[1].copy_assign_count(), 0u);
    hud_assert_eq(array[1].move_assign_count(), 0u);
}

GTEST_TEST(array, reserve_less_than_max_count_but_more_than_count_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.reserve(4);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 4u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them
    array.emplace_back(10);
    array.emplace_back(20);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 4u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);
    hud_assert_eq(array[0], type(10));
    hud_assert_eq(array[1], type(20));

    // Reserve less but more than
    array.reserve(2);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 4u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);
    hud_assert_eq(array[0], type(10));
    hud_assert_eq(array[1], type(20));
}

GTEST_TEST(array, reserve_less_than_count_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.reserve(2);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them, except moving them
    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    hud_assert_eq(array[0].id(), 10);
    hud_assert_eq(array[0].destructor_counter(), nullptr);
    hud_assert_eq(array[0].constructor_count(), 1u);
    hud_assert_eq(array[0].copy_constructor_count(), 0u);
    hud_assert_eq(array[0].move_constructor_count(), 0u);
    hud_assert_eq(array[0].copy_assign_count(), 0u);
    hud_assert_eq(array[0].move_assign_count(), 0u);

    hud_assert_eq(array[1].id(), 20);
    hud_assert_eq(array[1].destructor_counter(), nullptr);
    hud_assert_eq(array[1].constructor_count(), 1u);
    hud_assert_eq(array[1].copy_constructor_count(), 0u);
    hud_assert_eq(array[1].move_constructor_count(), 0u);
    hud_assert_eq(array[1].copy_assign_count(), 0u);
    hud_assert_eq(array[1].move_assign_count(), 0u);

    array.reserve(1);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    hud_assert_eq(array[0].id(), 10);
    hud_assert_eq(array[0].destructor_counter(), nullptr);
    hud_assert_eq(array[0].constructor_count(), 1u);
    hud_assert_eq(array[0].copy_constructor_count(), 0u);
    hud_assert_eq(array[0].move_constructor_count(), 0u);
    hud_assert_eq(array[0].copy_assign_count(), 0u);
    hud_assert_eq(array[0].move_assign_count(), 0u);

    hud_assert_eq(array[1].id(), 20);
    hud_assert_eq(array[1].destructor_counter(), nullptr);
    hud_assert_eq(array[1].constructor_count(), 1u);
    hud_assert_eq(array[1].copy_constructor_count(), 0u);
    hud_assert_eq(array[1].move_constructor_count(), 0u);
    hud_assert_eq(array[1].copy_assign_count(), 0u);
    hud_assert_eq(array[1].move_assign_count(), 0u);
}

GTEST_TEST(array, reserve_less_than_count_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.reserve(2);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them, except moving them
    array.emplace_back(10);
    array.emplace_back(20);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);
    hud_assert_eq(array[0], type(10));
    hud_assert_eq(array[1], type(20));

    array.reserve(1);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);
    hud_assert_eq(array[0], type(10));
    hud_assert_eq(array[1], type(20));
}