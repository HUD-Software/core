#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(vector, clear_destroy_but_keep_allocated_memory_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    i32 destructor_called[2] = {false, false};
    array_type vector;
    vector.reserve(2);
    const type *const buffer = vector.data();
    vector.emplace_back(10, &destructor_called[0]);
    vector.emplace_back(20, &destructor_called[1]);

    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector[0].id(), 10);
    hud_assert_eq(*vector[0].destructor_counter(), 0);
    hud_assert_eq(vector[1].id(), 20);
    hud_assert_eq(*vector[1].destructor_counter(), 0);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.clear();

    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.data(), buffer);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
    hud_assert_true(destructor_called[0]);
    hud_assert_true(destructor_called[1]);
}

GTEST_TEST(vector, clear_destroy_but_keep_allocated_memory_with_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    vector.reserve(2);
    const type *const buffer = vector.data();
    vector.emplace_back(10);
    vector.emplace_back(20);

    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector[0], type(10));
    hud_assert_eq(vector[1], type(20));
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.clear();

    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.data(), buffer);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
}

GTEST_TEST(vector, clear_empty_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.clear();
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
}

GTEST_TEST(vector, clear_empty_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.clear();
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
}
