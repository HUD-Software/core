#include <core/containers/array.h>
#include "allocators.h"

GTEST_TEST(array, clear_destroy_but_keep_allocated_memory_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    i32 destructor_called[2] = { false, false };
    array_type array;
    array.reserve(2);
    const type* const buffer = array.data();
    array.emplace_back(10, &destructor_called[0]);
    array.emplace_back(20, &destructor_called[1]);

    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array[0].id(), 10);
    ASSERT_EQ(*array[0].destructor_counter(), 0);
    ASSERT_EQ(array[1].id(), 20);
    ASSERT_EQ(*array[1].destructor_counter(), 0);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear();

    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.data(), buffer);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
    ASSERT_TRUE(destructor_called[0]);
    ASSERT_TRUE(destructor_called[1]);
}

GTEST_TEST(array, clear_destroy_but_keep_allocated_memory_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    array.reserve(2);
    const type* const buffer = array.data();
    array.emplace_back(10);
    array.emplace_back(20);

    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array[0], type(10));
    ASSERT_EQ(array[1], type(20));
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear();

    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.data(), buffer);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, clear_empty_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear();
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, clear_empty_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear();
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, clear_shrink_destroy_but_keep_allocated_memory_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    i32 destructor_called[2] = { false, false };
    array_type array;
    array.reserve(2);
    array.emplace_back(10, &destructor_called[0]);
    array.emplace_back(20, &destructor_called[1]);

    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array[0].id(), 10);
    ASSERT_EQ(*array[1].destructor_counter(), 0);
    ASSERT_EQ(array[1].id(), 20);
    ASSERT_EQ(*array[1].destructor_counter(), 0);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear_shrink();

    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 1u);
    ASSERT_TRUE(destructor_called[0]);
    ASSERT_TRUE(destructor_called[1]);
}

GTEST_TEST(array, clear_shrink_destroy_but_keep_allocated_memory_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    array.reserve(2);
    array.emplace_back(10);
    array.emplace_back(20);

    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array[0], type(10));
    ASSERT_EQ(array[1], type(20));
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear_shrink();

    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 1u);
}

GTEST_TEST(array, clear_shrink_empty_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear_shrink();
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
}

GTEST_TEST(array, clear_shrink_empty_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.clear_shrink();
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
}