#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(vector, reserve_empty_to_zero_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.reserve(0);
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
}

GTEST_TEST(vector, reserve_empty_to_zero_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.reserve(0);
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
}

GTEST_TEST(vector, reserve_empty_to_size_allocate_only_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.reserve(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
}

GTEST_TEST(vector, reserve_empty_to_size_allocate_only_with_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.reserve(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
}

GTEST_TEST(vector, reserve_non_empty_to_more_size_allocate_only_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.reserve(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them, except moving them
    vector.emplace_back(10, nullptr);
    vector.emplace_back(20, nullptr);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    hud_assert_eq(vector[0].id(), 10);
    hud_assert_eq(vector[0].destructor_counter(), nullptr);
    hud_assert_eq(vector[0].constructor_count(), 1u);
    hud_assert_eq(vector[0].copy_constructor_count(), 0u);
    hud_assert_eq(vector[0].move_constructor_count(), 0u);
    hud_assert_eq(vector[0].copy_assign_count(), 0u);
    hud_assert_eq(vector[0].move_assign_count(), 0u);

    hud_assert_eq(vector[1].id(), 20);
    hud_assert_eq(vector[1].destructor_counter(), nullptr);
    hud_assert_eq(vector[1].constructor_count(), 1u);
    hud_assert_eq(vector[1].copy_constructor_count(), 0u);
    hud_assert_eq(vector[1].move_constructor_count(), 0u);
    hud_assert_eq(vector[1].copy_assign_count(), 0u);
    hud_assert_eq(vector[1].move_assign_count(), 0u);

    vector.reserve(4);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_eq(vector.allocator().allocation_count(), 2u);
    hud_assert_eq(vector.allocator().free_count(), 1u);

    hud_assert_eq(vector[0].id(), 10);
    hud_assert_eq(vector[0].destructor_counter(), nullptr);
    hud_assert_eq(vector[0].constructor_count(), 1u);
    hud_assert_eq(vector[0].copy_constructor_count(), 0u);
    hud_assert_eq(vector[0].move_constructor_count(), 1u);
    hud_assert_eq(vector[0].copy_assign_count(), 0u);
    hud_assert_eq(vector[0].move_assign_count(), 0u);

    hud_assert_eq(vector[1].id(), 20);
    hud_assert_eq(vector[1].destructor_counter(), nullptr);
    hud_assert_eq(vector[1].constructor_count(), 1u);
    hud_assert_eq(vector[1].copy_constructor_count(), 0u);
    hud_assert_eq(vector[1].move_constructor_count(), 1u);
    hud_assert_eq(vector[1].copy_assign_count(), 0u);
    hud_assert_eq(vector[1].move_assign_count(), 0u);
}

GTEST_TEST(vector, reserve_non_empty_to_more_size_allocate_only_with_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.reserve(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them, except moving them
    vector.emplace_back(10);
    vector.emplace_back(20);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
    hud_assert_eq(vector[0], type(10));
    hud_assert_eq(vector[1], type(20));

    vector.reserve(4);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_eq(vector.allocator().allocation_count(), 2u);
    hud_assert_eq(vector.allocator().free_count(), 1u);
    hud_assert_eq(vector[0], type(10));
    hud_assert_eq(vector[1], type(20));
}

GTEST_TEST(vector, reserve_less_than_max_count_but_more_than_count_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.reserve(4);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them
    vector.emplace_back(10, nullptr);
    vector.emplace_back(20, nullptr);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    hud_assert_eq(vector[0].id(), 10);
    hud_assert_eq(vector[0].destructor_counter(), nullptr);
    hud_assert_eq(vector[0].constructor_count(), 1u);
    hud_assert_eq(vector[0].copy_constructor_count(), 0u);
    hud_assert_eq(vector[0].move_constructor_count(), 0u);
    hud_assert_eq(vector[0].copy_assign_count(), 0u);
    hud_assert_eq(vector[0].move_assign_count(), 0u);

    hud_assert_eq(vector[1].id(), 20);
    hud_assert_eq(vector[1].destructor_counter(), nullptr);
    hud_assert_eq(vector[1].constructor_count(), 1u);
    hud_assert_eq(vector[1].copy_constructor_count(), 0u);
    hud_assert_eq(vector[1].move_constructor_count(), 0u);
    hud_assert_eq(vector[1].copy_assign_count(), 0u);
    hud_assert_eq(vector[1].move_assign_count(), 0u);

    // Reserve less but more than
    vector.reserve(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    hud_assert_eq(vector[0].id(), 10);
    hud_assert_eq(vector[0].destructor_counter(), nullptr);
    hud_assert_eq(vector[0].constructor_count(), 1u);
    hud_assert_eq(vector[0].copy_constructor_count(), 0u);
    hud_assert_eq(vector[0].move_constructor_count(), 0u);
    hud_assert_eq(vector[0].copy_assign_count(), 0u);
    hud_assert_eq(vector[0].move_assign_count(), 0u);

    hud_assert_eq(vector[1].id(), 20);
    hud_assert_eq(vector[1].destructor_counter(), nullptr);
    hud_assert_eq(vector[1].constructor_count(), 1u);
    hud_assert_eq(vector[1].copy_constructor_count(), 0u);
    hud_assert_eq(vector[1].move_constructor_count(), 0u);
    hud_assert_eq(vector[1].copy_assign_count(), 0u);
    hud_assert_eq(vector[1].move_assign_count(), 0u);
}

GTEST_TEST(vector, reserve_less_than_max_count_but_more_than_count_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.reserve(4);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them
    vector.emplace_back(10);
    vector.emplace_back(20);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
    hud_assert_eq(vector[0], type(10));
    hud_assert_eq(vector[1], type(20));

    // Reserve less but more than
    vector.reserve(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
    hud_assert_eq(vector[0], type(10));
    hud_assert_eq(vector[1], type(20));
}

GTEST_TEST(vector, reserve_less_than_count_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.reserve(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them, except moving them
    vector.emplace_back(10, nullptr);
    vector.emplace_back(20, nullptr);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    hud_assert_eq(vector[0].id(), 10);
    hud_assert_eq(vector[0].destructor_counter(), nullptr);
    hud_assert_eq(vector[0].constructor_count(), 1u);
    hud_assert_eq(vector[0].copy_constructor_count(), 0u);
    hud_assert_eq(vector[0].move_constructor_count(), 0u);
    hud_assert_eq(vector[0].copy_assign_count(), 0u);
    hud_assert_eq(vector[0].move_assign_count(), 0u);

    hud_assert_eq(vector[1].id(), 20);
    hud_assert_eq(vector[1].destructor_counter(), nullptr);
    hud_assert_eq(vector[1].constructor_count(), 1u);
    hud_assert_eq(vector[1].copy_constructor_count(), 0u);
    hud_assert_eq(vector[1].move_constructor_count(), 0u);
    hud_assert_eq(vector[1].copy_assign_count(), 0u);
    hud_assert_eq(vector[1].move_assign_count(), 0u);

    vector.reserve(1);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    hud_assert_eq(vector[0].id(), 10);
    hud_assert_eq(vector[0].destructor_counter(), nullptr);
    hud_assert_eq(vector[0].constructor_count(), 1u);
    hud_assert_eq(vector[0].copy_constructor_count(), 0u);
    hud_assert_eq(vector[0].move_constructor_count(), 0u);
    hud_assert_eq(vector[0].copy_assign_count(), 0u);
    hud_assert_eq(vector[0].move_assign_count(), 0u);

    hud_assert_eq(vector[1].id(), 20);
    hud_assert_eq(vector[1].destructor_counter(), nullptr);
    hud_assert_eq(vector[1].constructor_count(), 1u);
    hud_assert_eq(vector[1].copy_constructor_count(), 0u);
    hud_assert_eq(vector[1].move_constructor_count(), 0u);
    hud_assert_eq(vector[1].copy_assign_count(), 0u);
    hud_assert_eq(vector[1].move_assign_count(), 0u);
}

GTEST_TEST(vector, reserve_less_than_count_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.reserve(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them, except moving them
    vector.emplace_back(10);
    vector.emplace_back(20);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
    hud_assert_eq(vector[0], type(10));
    hud_assert_eq(vector[1], type(20));

    vector.reserve(1);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
    hud_assert_eq(vector[0], type(10));
    hud_assert_eq(vector[1], type(20));
}