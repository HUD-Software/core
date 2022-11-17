#include <core/containers/array.h>
#include "allocators.h"

GTEST_TEST(array, swap_empty_array_do_nothing)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type a, b;
    GTEST_ASSERT_EQ(a.data(), nullptr);
    GTEST_ASSERT_EQ(a.count(), 0u);
    GTEST_ASSERT_EQ(a.max_count(), 0u);
    GTEST_ASSERT_EQ(a.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(a.allocator().free_count(), 0u);
    GTEST_ASSERT_EQ(b.data(), nullptr);
    GTEST_ASSERT_EQ(b.count(), 0u);
    GTEST_ASSERT_EQ(b.max_count(), 0u);
    GTEST_ASSERT_EQ(b.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(b.allocator().free_count(), 0u);

    a.swap(b);

    GTEST_ASSERT_EQ(a.data(), nullptr);
    GTEST_ASSERT_EQ(a.count(), 0u);
    GTEST_ASSERT_EQ(a.max_count(), 0u);
    GTEST_ASSERT_EQ(a.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(a.allocator().free_count(), 0u);
    GTEST_ASSERT_EQ(b.data(), nullptr);
    GTEST_ASSERT_EQ(b.count(), 0u);
    GTEST_ASSERT_EQ(b.max_count(), 0u);
    GTEST_ASSERT_EQ(b.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(b.allocator().free_count(), 0u);

    swap(a, b);

    GTEST_ASSERT_EQ(a.data(), nullptr);
    GTEST_ASSERT_EQ(a.count(), 0u);
    GTEST_ASSERT_EQ(a.max_count(), 0u);
    GTEST_ASSERT_EQ(a.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(a.allocator().free_count(), 0u);
    GTEST_ASSERT_EQ(b.data(), nullptr);
    GTEST_ASSERT_EQ(b.count(), 0u);
    GTEST_ASSERT_EQ(b.max_count(), 0u);
    GTEST_ASSERT_EQ(b.allocator().allocation_count(), 0u);
    GTEST_ASSERT_EQ(b.allocator().free_count(), 0u);
}

GTEST_TEST(array, swap_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    i32 a_destructor_called[2] = { false, false };
    array_type a;
    a.reserve(4);
    a.emplace_back(10, &a_destructor_called[0]);
    a.emplace_back(20, &a_destructor_called[1]);

    GTEST_ASSERT_NE(a.data(), nullptr);
    GTEST_ASSERT_EQ(a.count(), 2u);
    GTEST_ASSERT_EQ(a.max_count(), 4u);
    GTEST_ASSERT_EQ(a[0].id(), 10);
    GTEST_ASSERT_EQ(*a[0].destructor_counter(), 0);
    GTEST_ASSERT_EQ(a[1].id(), 20);
    GTEST_ASSERT_EQ(*a[1].destructor_counter(), 0);
    GTEST_ASSERT_EQ(a.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(a.allocator().free_count(), 0u);

    i32 b_destructor_called[3] = { false, false, false };
    array_type b;
    b.reserve(5);
    b.emplace_back(100, &b_destructor_called[0]);
    b.emplace_back(200, &b_destructor_called[1]);
    b.emplace_back(300, &b_destructor_called[1]);

    GTEST_ASSERT_NE(b.data(), nullptr);
    GTEST_ASSERT_EQ(b.count(), 3u);
    GTEST_ASSERT_EQ(b.max_count(), 5u);
    GTEST_ASSERT_EQ(b[0].id(), 100);
    GTEST_ASSERT_EQ(*b[0].destructor_counter(), 0);
    GTEST_ASSERT_EQ(b[1].id(), 200);
    GTEST_ASSERT_EQ(*b[1].destructor_counter(), 0);
    GTEST_ASSERT_EQ(b[2].id(), 300);
    GTEST_ASSERT_EQ(*b[2].destructor_counter(), 0);
    GTEST_ASSERT_EQ(b.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(b.allocator().free_count(), 0u);

    a.swap(b);

    GTEST_ASSERT_NE(a.data(), nullptr);
    GTEST_ASSERT_EQ(a.count(), 3u);
    GTEST_ASSERT_EQ(a.max_count(), 5u);
    GTEST_ASSERT_EQ(a[0].id(), 100);
    GTEST_ASSERT_EQ(*a[0].destructor_counter(), 0);
    GTEST_ASSERT_EQ(a[1].id(), 200);
    GTEST_ASSERT_EQ(*a[1].destructor_counter(), 0);
    GTEST_ASSERT_EQ(a[2].id(), 300);
    GTEST_ASSERT_EQ(*a[2].destructor_counter(), 0);
    GTEST_ASSERT_EQ(a.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(a.allocator().free_count(), 0u);

    GTEST_ASSERT_NE(b.data(), nullptr);
    GTEST_ASSERT_EQ(b.count(), 2u);
    GTEST_ASSERT_EQ(b.max_count(), 4u);
    GTEST_ASSERT_EQ(b[0].id(), 10);
    GTEST_ASSERT_EQ(*b[0].destructor_counter(), 0);
    GTEST_ASSERT_EQ(b[1].id(), 20);
    GTEST_ASSERT_EQ(*b[1].destructor_counter(), 0);
    GTEST_ASSERT_EQ(b.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(b.allocator().free_count(), 0u);


    swap(a, b);

    GTEST_ASSERT_NE(a.data(), nullptr);
    GTEST_ASSERT_EQ(a.count(), 2u);
    GTEST_ASSERT_EQ(a.max_count(), 4u);
    GTEST_ASSERT_EQ(a[0].id(), 10);
    GTEST_ASSERT_EQ(*a[0].destructor_counter(), 0);
    GTEST_ASSERT_EQ(a[1].id(), 20);
    GTEST_ASSERT_EQ(*a[1].destructor_counter(), 0);
    GTEST_ASSERT_EQ(a.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(a.allocator().free_count(), 0u);

    GTEST_ASSERT_NE(b.data(), nullptr);
    GTEST_ASSERT_EQ(b.count(), 3u);
    GTEST_ASSERT_EQ(b.max_count(), 5u);
    GTEST_ASSERT_EQ(b[0].id(), 100);
    GTEST_ASSERT_EQ(*b[0].destructor_counter(), 0);
    GTEST_ASSERT_EQ(b[1].id(), 200);
    GTEST_ASSERT_EQ(*b[1].destructor_counter(), 0);
    GTEST_ASSERT_EQ(b[2].id(), 300);
    GTEST_ASSERT_EQ(*b[2].destructor_counter(), 0);
    GTEST_ASSERT_EQ(b.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(b.allocator().free_count(), 0u);
}

GTEST_TEST(array, swap_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type a;
    a.reserve(4);
    a.emplace_back(10);
    a.emplace_back(20);

    GTEST_ASSERT_NE(a.data(), nullptr);
    GTEST_ASSERT_EQ(a.count(), 2u);
    GTEST_ASSERT_EQ(a.max_count(), 4u);
    GTEST_ASSERT_EQ(a[0], 10u);
    GTEST_ASSERT_EQ(a[1], 20u);
    GTEST_ASSERT_EQ(a.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(a.allocator().free_count(), 0u);

    array_type b;
    b.reserve(5);
    b.emplace_back(100);
    b.emplace_back(200);
    b.emplace_back(300);

    GTEST_ASSERT_NE(b.data(), nullptr);
    GTEST_ASSERT_EQ(b.count(), 3u);
    GTEST_ASSERT_EQ(b.max_count(), 5u);
    GTEST_ASSERT_EQ(b[0], 100u);
    GTEST_ASSERT_EQ(b[1], 200u);
    GTEST_ASSERT_EQ(b[2], 300u);
    GTEST_ASSERT_EQ(b.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(b.allocator().free_count(), 0u);

    a.swap(b);

    GTEST_ASSERT_NE(a.data(), nullptr);
    GTEST_ASSERT_EQ(a.count(), 3u);
    GTEST_ASSERT_EQ(a.max_count(), 5u);
    GTEST_ASSERT_EQ(a[0], 100u);
    GTEST_ASSERT_EQ(a[1], 200u);
    GTEST_ASSERT_EQ(a[2], 300u);
    GTEST_ASSERT_EQ(a.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(a.allocator().free_count(), 0u);

    GTEST_ASSERT_NE(b.data(), nullptr);
    GTEST_ASSERT_EQ(b.count(), 2u);
    GTEST_ASSERT_EQ(b.max_count(), 4u);
    GTEST_ASSERT_EQ(b[0], 10u);
    GTEST_ASSERT_EQ(b[1], 20u);
    GTEST_ASSERT_EQ(b.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(b.allocator().free_count(), 0u);


    swap(a, b);

    GTEST_ASSERT_NE(a.data(), nullptr);
    GTEST_ASSERT_EQ(a.count(), 2u);
    GTEST_ASSERT_EQ(a.max_count(), 4u);
    GTEST_ASSERT_EQ(a[0], 10u);
    GTEST_ASSERT_EQ(a[1], 20u);
    GTEST_ASSERT_EQ(a.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(a.allocator().free_count(), 0u);

    GTEST_ASSERT_NE(b.data(), nullptr);
    GTEST_ASSERT_EQ(b.count(), 3u);
    GTEST_ASSERT_EQ(b.max_count(), 5u);
    GTEST_ASSERT_EQ(b[0], 100u);
    GTEST_ASSERT_EQ(b[1], 200u);
    GTEST_ASSERT_EQ(b[2], 300u);
    GTEST_ASSERT_EQ(b.allocator().allocation_count(), 1u);
    GTEST_ASSERT_EQ(b.allocator().free_count(), 0u);
}