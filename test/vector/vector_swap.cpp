#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(vector, swap_empty_array_do_nothing)
{
    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type a, b;
    hud_assert_eq(a.data(), nullptr);
    hud_assert_eq(a.count(), 0u);
    hud_assert_eq(a.max_count(), 0u);
    hud_assert_eq(a.allocator().allocation_count(), 0u);
    hud_assert_eq(a.allocator().free_count(), 0u);
    hud_assert_eq(b.data(), nullptr);
    hud_assert_eq(b.count(), 0u);
    hud_assert_eq(b.max_count(), 0u);
    hud_assert_eq(b.allocator().allocation_count(), 0u);
    hud_assert_eq(b.allocator().free_count(), 0u);

    a.swap(b);

    hud_assert_eq(a.data(), nullptr);
    hud_assert_eq(a.count(), 0u);
    hud_assert_eq(a.max_count(), 0u);
    hud_assert_eq(a.allocator().allocation_count(), 0u);
    hud_assert_eq(a.allocator().free_count(), 0u);
    hud_assert_eq(b.data(), nullptr);
    hud_assert_eq(b.count(), 0u);
    hud_assert_eq(b.max_count(), 0u);
    hud_assert_eq(b.allocator().allocation_count(), 0u);
    hud_assert_eq(b.allocator().free_count(), 0u);

    swap(a, b);

    hud_assert_eq(a.data(), nullptr);
    hud_assert_eq(a.count(), 0u);
    hud_assert_eq(a.max_count(), 0u);
    hud_assert_eq(a.allocator().allocation_count(), 0u);
    hud_assert_eq(a.allocator().free_count(), 0u);
    hud_assert_eq(b.data(), nullptr);
    hud_assert_eq(b.count(), 0u);
    hud_assert_eq(b.max_count(), 0u);
    hud_assert_eq(b.allocator().allocation_count(), 0u);
    hud_assert_eq(b.allocator().free_count(), 0u);
}

GTEST_TEST(vector, swap_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    i32 a_destructor_called[2] = {false, false};
    array_type a;
    a.reserve(4);
    a.emplace_back(10, &a_destructor_called[0]);
    a.emplace_back(20, &a_destructor_called[1]);

    hud_assert_ne(a.data(), nullptr);
    hud_assert_eq(a.count(), 2u);
    hud_assert_eq(a.max_count(), 4u);
    hud_assert_eq(a[0].id(), 10);
    hud_assert_eq(*a[0].destructor_counter(), 0);
    hud_assert_eq(a[1].id(), 20);
    hud_assert_eq(*a[1].destructor_counter(), 0);
    hud_assert_eq(a.allocator().allocation_count(), 1u);
    hud_assert_eq(a.allocator().free_count(), 0u);

    i32 b_destructor_called[3] = {false, false, false};
    array_type b;
    b.reserve(5);
    b.emplace_back(100, &b_destructor_called[0]);
    b.emplace_back(200, &b_destructor_called[1]);
    b.emplace_back(300, &b_destructor_called[1]);

    hud_assert_ne(b.data(), nullptr);
    hud_assert_eq(b.count(), 3u);
    hud_assert_eq(b.max_count(), 5u);
    hud_assert_eq(b[0].id(), 100);
    hud_assert_eq(*b[0].destructor_counter(), 0);
    hud_assert_eq(b[1].id(), 200);
    hud_assert_eq(*b[1].destructor_counter(), 0);
    hud_assert_eq(b[2].id(), 300);
    hud_assert_eq(*b[2].destructor_counter(), 0);
    hud_assert_eq(b.allocator().allocation_count(), 1u);
    hud_assert_eq(b.allocator().free_count(), 0u);

    a.swap(b);

    hud_assert_ne(a.data(), nullptr);
    hud_assert_eq(a.count(), 3u);
    hud_assert_eq(a.max_count(), 5u);
    hud_assert_eq(a[0].id(), 100);
    hud_assert_eq(*a[0].destructor_counter(), 0);
    hud_assert_eq(a[1].id(), 200);
    hud_assert_eq(*a[1].destructor_counter(), 0);
    hud_assert_eq(a[2].id(), 300);
    hud_assert_eq(*a[2].destructor_counter(), 0);
    hud_assert_eq(a.allocator().allocation_count(), 1u);
    hud_assert_eq(a.allocator().free_count(), 0u);

    hud_assert_ne(b.data(), nullptr);
    hud_assert_eq(b.count(), 2u);
    hud_assert_eq(b.max_count(), 4u);
    hud_assert_eq(b[0].id(), 10);
    hud_assert_eq(*b[0].destructor_counter(), 0);
    hud_assert_eq(b[1].id(), 20);
    hud_assert_eq(*b[1].destructor_counter(), 0);
    hud_assert_eq(b.allocator().allocation_count(), 1u);
    hud_assert_eq(b.allocator().free_count(), 0u);

    hud::swap(a, b);

    hud_assert_ne(a.data(), nullptr);
    hud_assert_eq(a.count(), 2u);
    hud_assert_eq(a.max_count(), 4u);
    hud_assert_eq(a[0].id(), 10);
    hud_assert_eq(*a[0].destructor_counter(), 0);
    hud_assert_eq(a[1].id(), 20);
    hud_assert_eq(*a[1].destructor_counter(), 0);
    hud_assert_eq(a.allocator().allocation_count(), 1u);
    hud_assert_eq(a.allocator().free_count(), 0u);

    hud_assert_ne(b.data(), nullptr);
    hud_assert_eq(b.count(), 3u);
    hud_assert_eq(b.max_count(), 5u);
    hud_assert_eq(b[0].id(), 100);
    hud_assert_eq(*b[0].destructor_counter(), 0);
    hud_assert_eq(b[1].id(), 200);
    hud_assert_eq(*b[1].destructor_counter(), 0);
    hud_assert_eq(b[2].id(), 300);
    hud_assert_eq(*b[2].destructor_counter(), 0);
    hud_assert_eq(b.allocator().allocation_count(), 1u);
    hud_assert_eq(b.allocator().free_count(), 0u);
}

GTEST_TEST(vector, swap_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type a;
    a.reserve(4);
    a.emplace_back(10);
    a.emplace_back(20);

    hud_assert_ne(a.data(), nullptr);
    hud_assert_eq(a.count(), 2u);
    hud_assert_eq(a.max_count(), 4u);
    hud_assert_eq(a[0], 10u);
    hud_assert_eq(a[1], 20u);
    hud_assert_eq(a.allocator().allocation_count(), 1u);
    hud_assert_eq(a.allocator().free_count(), 0u);

    array_type b;
    b.reserve(5);
    b.emplace_back(100);
    b.emplace_back(200);
    b.emplace_back(300);

    hud_assert_ne(b.data(), nullptr);
    hud_assert_eq(b.count(), 3u);
    hud_assert_eq(b.max_count(), 5u);
    hud_assert_eq(b[0], 100u);
    hud_assert_eq(b[1], 200u);
    hud_assert_eq(b[2], 300u);
    hud_assert_eq(b.allocator().allocation_count(), 1u);
    hud_assert_eq(b.allocator().free_count(), 0u);

    a.swap(b);

    hud_assert_ne(a.data(), nullptr);
    hud_assert_eq(a.count(), 3u);
    hud_assert_eq(a.max_count(), 5u);
    hud_assert_eq(a[0], 100u);
    hud_assert_eq(a[1], 200u);
    hud_assert_eq(a[2], 300u);
    hud_assert_eq(a.allocator().allocation_count(), 1u);
    hud_assert_eq(a.allocator().free_count(), 0u);

    hud_assert_ne(b.data(), nullptr);
    hud_assert_eq(b.count(), 2u);
    hud_assert_eq(b.max_count(), 4u);
    hud_assert_eq(b[0], 10u);
    hud_assert_eq(b[1], 20u);
    hud_assert_eq(b.allocator().allocation_count(), 1u);
    hud_assert_eq(b.allocator().free_count(), 0u);

    hud::swap(a, b);

    hud_assert_ne(a.data(), nullptr);
    hud_assert_eq(a.count(), 2u);
    hud_assert_eq(a.max_count(), 4u);
    hud_assert_eq(a[0], 10u);
    hud_assert_eq(a[1], 20u);
    hud_assert_eq(a.allocator().allocation_count(), 1u);
    hud_assert_eq(a.allocator().free_count(), 0u);

    hud_assert_ne(b.data(), nullptr);
    hud_assert_eq(b.count(), 3u);
    hud_assert_eq(b.max_count(), 5u);
    hud_assert_eq(b[0], 100u);
    hud_assert_eq(b[1], 200u);
    hud_assert_eq(b[2], 300u);
    hud_assert_eq(b.allocator().allocation_count(), 1u);
    hud_assert_eq(b.allocator().free_count(), 0u);
}