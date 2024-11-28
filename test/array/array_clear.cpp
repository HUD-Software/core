#include <core/containers/array.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(array, clear_destroy_but_keep_allocated_memory_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    i32 destructor_called[2] = {false, false};
    array_type array;
    array.reserve(2);
    const type *const buffer = array.data();
    array.emplace_back(10, &destructor_called[0]);
    array.emplace_back(20, &destructor_called[1]);

    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array[0].id(), 10);
    hud_assert_eq(*array[0].destructor_counter(), 0);
    hud_assert_eq(array[1].id(), 20);
    hud_assert_eq(*array[1].destructor_counter(), 0);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.clear();

    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.data(), buffer);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);
    hud_assert_true(destructor_called[0]);
    hud_assert_true(destructor_called[1]);
}

GTEST_TEST(array, clear_destroy_but_keep_allocated_memory_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    array.reserve(2);
    const type *const buffer = array.data();
    array.emplace_back(10);
    array.emplace_back(20);

    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array[0], type(10));
    hud_assert_eq(array[1], type(20));
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.clear();

    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.data(), buffer);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, clear_empty_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.clear();
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, clear_empty_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.clear();
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, clear_shrink_destroy_but_keep_allocated_memory_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    i32 destructor_called[2] = {false, false};
    array_type array;
    array.reserve(2);
    array.emplace_back(10, &destructor_called[0]);
    array.emplace_back(20, &destructor_called[1]);

    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array[0].id(), 10);
    hud_assert_eq(*array[1].destructor_counter(), 0);
    hud_assert_eq(array[1].id(), 20);
    hud_assert_eq(*array[1].destructor_counter(), 0);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.clear_shrink();

    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 1u);
    hud_assert_true(destructor_called[0]);
    hud_assert_true(destructor_called[1]);
}

GTEST_TEST(array, clear_shrink_destroy_but_keep_allocated_memory_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    array.reserve(2);
    array.emplace_back(10);
    array.emplace_back(20);

    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 2u);
    hud_assert_eq(array[0], type(10));
    hud_assert_eq(array[1], type(20));
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.clear_shrink();

    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 1u);
}

GTEST_TEST(array, clear_shrink_empty_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.clear_shrink();
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, clear_shrink_empty_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    array.clear_shrink();
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);
}