#include <core/containers/array.h>
#include "allocators.h"

GTEST_TEST(array, clear_destroy_but_keep_allocated_memory_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    i32 destructor_called[2] = {false, false};
    array_type array;
    array.reserve(2);
    const type *const buffer = array.data();
    array.emplace_back(10, &destructor_called[0]);
    array.emplace_back(20, &destructor_called[1]);

    GTEST_ASSERT_NE(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 2u);
    GTEST_ASSERT_EQ(array.max_count(), 2u);
    GTEST_ASSERT_EQ(array[0].id(), 10);
    GTEST_ASSERT_EQ(*array[0].destructor_counter(), 0);
    GTEST_ASSERT_EQ(array[1].id(), 20);
    GTEST_ASSERT_EQ(*array[1].destructor_counter(), 0);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear();

    GTEST_ASSERT_NE(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.data(), buffer);
    GTEST_ASSERT_EQ(array.count(), 0u);
    GTEST_ASSERT_EQ(array.max_count(), 2u);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
    GTEST_ASSERT_TRUE(destructor_called[0]);
    GTEST_ASSERT_TRUE(destructor_called[1]);
}

GTEST_TEST(array, clear_destroy_but_keep_allocated_memory_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    array.reserve(2);
    const type *const buffer = array.data();
    array.emplace_back(10);
    array.emplace_back(20);

    GTEST_ASSERT_NE(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 2u);
    GTEST_ASSERT_EQ(array.max_count(), 2u);
    GTEST_ASSERT_EQ(array[0], type(10));
    GTEST_ASSERT_EQ(array[1], type(20));
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear();

    GTEST_ASSERT_NE(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.data(), buffer);
    GTEST_ASSERT_EQ(array.count(), 0u);
    GTEST_ASSERT_EQ(array.max_count(), 2u);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, clear_empty_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    GTEST_ASSERT_EQ(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 0u);
    GTEST_ASSERT_EQ(array.max_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear();
    GTEST_ASSERT_EQ(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 0u);
    GTEST_ASSERT_EQ(array.max_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, clear_empty_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    GTEST_ASSERT_EQ(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 0u);
    GTEST_ASSERT_EQ(array.max_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear();
    GTEST_ASSERT_EQ(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 0u);
    GTEST_ASSERT_EQ(array.max_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, clear_shrink_destroy_but_keep_allocated_memory_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    i32 destructor_called[2] = {false, false};
    array_type array;
    array.reserve(2);
    array.emplace_back(10, &destructor_called[0]);
    array.emplace_back(20, &destructor_called[1]);

    GTEST_ASSERT_NE(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 2u);
    GTEST_ASSERT_EQ(array.max_count(), 2u);
    GTEST_ASSERT_EQ(array[0].id(), 10);
    GTEST_ASSERT_EQ(*array[1].destructor_counter(), 0);
    GTEST_ASSERT_EQ(array[1].id(), 20);
    GTEST_ASSERT_EQ(*array[1].destructor_counter(), 0);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear_shrink();

    GTEST_ASSERT_EQ(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 0u);
    GTEST_ASSERT_EQ(array.max_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 1u);
    GTEST_ASSERT_TRUE(destructor_called[0]);
    GTEST_ASSERT_TRUE(destructor_called[1]);
}

GTEST_TEST(array, clear_shrink_destroy_but_keep_allocated_memory_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    array.reserve(2);
    array.emplace_back(10);
    array.emplace_back(20);

    GTEST_ASSERT_NE(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 2u);
    GTEST_ASSERT_EQ(array.max_count(), 2u);
    GTEST_ASSERT_EQ(array[0], type(10));
    GTEST_ASSERT_EQ(array[1], type(20));
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear_shrink();

    GTEST_ASSERT_EQ(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 0u);
    GTEST_ASSERT_EQ(array.max_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 1u);
}

GTEST_TEST(array, clear_shrink_empty_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    GTEST_ASSERT_EQ(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 0u);
    GTEST_ASSERT_EQ(array.max_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear_shrink();
    GTEST_ASSERT_EQ(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 0u);
    GTEST_ASSERT_EQ(array.max_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, clear_shrink_empty_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    GTEST_ASSERT_EQ(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 0u);
    GTEST_ASSERT_EQ(array.max_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear_shrink();
    GTEST_ASSERT_EQ(array.data(), nullptr);
    GTEST_ASSERT_EQ(array.count(), 0u);
    GTEST_ASSERT_EQ(array.max_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
}