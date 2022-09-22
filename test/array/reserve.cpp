#include <core/containers/array.h>
#include "allocators.h"

TEST(array, reserve_empty_to_zero_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.reserve(0);
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
}

TEST(array, reserve_empty_to_zero_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.reserve(0);
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
}

TEST(array, reserve_empty_to_size_allocate_only_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.reserve(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
}

TEST(array, reserve_empty_to_size_allocate_only_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.reserve(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
}

TEST(array, reserve_non_empty_to_more_size_allocate_only_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.reserve(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them, except moving them
    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    ASSERT_EQ(array[0].id(), 10);
    ASSERT_EQ(array[0].destructor_counter(), nullptr);
    ASSERT_EQ(array[0].constructor_count(), 1u);
    ASSERT_EQ(array[0].copy_constructor_count(), 0u);
    ASSERT_EQ(array[0].move_constructor_count(), 0u);
    ASSERT_EQ(array[0].copy_assign_count(), 0u);
    ASSERT_EQ(array[0].move_assign_count(), 0u);

    ASSERT_EQ(array[1].id(), 20);
    ASSERT_EQ(array[1].destructor_counter(), nullptr);
    ASSERT_EQ(array[1].constructor_count(), 1u);
    ASSERT_EQ(array[1].copy_constructor_count(), 0u);
    ASSERT_EQ(array[1].move_constructor_count(), 0u);
    ASSERT_EQ(array[1].copy_assign_count(), 0u);
    ASSERT_EQ(array[1].move_assign_count(), 0u);

    array.reserve(4);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 4u);
    ASSERT_EQ(array.allocator().allocation_count(), 2u);
    ASSERT_EQ(array.allocator().free_count(), 1u);

    ASSERT_EQ(array[0].id(), 10);
    ASSERT_EQ(array[0].destructor_counter(), nullptr);
    ASSERT_EQ(array[0].constructor_count(), 1u);
    ASSERT_EQ(array[0].copy_constructor_count(), 0u);
    ASSERT_EQ(array[0].move_constructor_count(), 1u);
    ASSERT_EQ(array[0].copy_assign_count(), 0u);
    ASSERT_EQ(array[0].move_assign_count(), 0u);

    ASSERT_EQ(array[1].id(), 20);
    ASSERT_EQ(array[1].destructor_counter(), nullptr);
    ASSERT_EQ(array[1].constructor_count(), 1u);
    ASSERT_EQ(array[1].copy_constructor_count(), 0u);
    ASSERT_EQ(array[1].move_constructor_count(), 1u);
    ASSERT_EQ(array[1].copy_assign_count(), 0u);
    ASSERT_EQ(array[1].move_assign_count(), 0u);
}

TEST(array, reserve_non_empty_to_more_size_allocate_only_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.reserve(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them, except moving them
    array.emplace_back(10);
    array.emplace_back(20);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
    ASSERT_EQ(array[0], type(10));
    ASSERT_EQ(array[1], type(20));


    array.reserve(4);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 4u);
    ASSERT_EQ(array.allocator().allocation_count(), 2u);
    ASSERT_EQ(array.allocator().free_count(), 1u);
    ASSERT_EQ(array[0], type(10));
    ASSERT_EQ(array[1], type(20));
}

TEST(array, reserve_less_than_max_count_but_more_than_count_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.reserve(4);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 4u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them
    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 4u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    ASSERT_EQ(array[0].id(), 10);
    ASSERT_EQ(array[0].destructor_counter(), nullptr);
    ASSERT_EQ(array[0].constructor_count(), 1u);
    ASSERT_EQ(array[0].copy_constructor_count(), 0u);
    ASSERT_EQ(array[0].move_constructor_count(), 0u);
    ASSERT_EQ(array[0].copy_assign_count(), 0u);
    ASSERT_EQ(array[0].move_assign_count(), 0u);

    ASSERT_EQ(array[1].id(), 20);
    ASSERT_EQ(array[1].destructor_counter(), nullptr);
    ASSERT_EQ(array[1].constructor_count(), 1u);
    ASSERT_EQ(array[1].copy_constructor_count(), 0u);
    ASSERT_EQ(array[1].move_constructor_count(), 0u);
    ASSERT_EQ(array[1].copy_assign_count(), 0u);
    ASSERT_EQ(array[1].move_assign_count(), 0u);

    // Reserve less but more than 
    array.reserve(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 4u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    ASSERT_EQ(array[0].id(), 10);
    ASSERT_EQ(array[0].destructor_counter(), nullptr);
    ASSERT_EQ(array[0].constructor_count(), 1u);
    ASSERT_EQ(array[0].copy_constructor_count(), 0u);
    ASSERT_EQ(array[0].move_constructor_count(), 0u);
    ASSERT_EQ(array[0].copy_assign_count(), 0u);
    ASSERT_EQ(array[0].move_assign_count(), 0u);

    ASSERT_EQ(array[1].id(), 20);
    ASSERT_EQ(array[1].destructor_counter(), nullptr);
    ASSERT_EQ(array[1].constructor_count(), 1u);
    ASSERT_EQ(array[1].copy_constructor_count(), 0u);
    ASSERT_EQ(array[1].move_constructor_count(), 0u);
    ASSERT_EQ(array[1].copy_assign_count(), 0u);
    ASSERT_EQ(array[1].move_assign_count(), 0u);
}

TEST(array, reserve_less_than_max_count_but_more_than_count_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.reserve(4);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 4u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them
    array.emplace_back(10);
    array.emplace_back(20);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 4u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
    ASSERT_EQ(array[0], type(10));
    ASSERT_EQ(array[1], type(20));

    // Reserve less but more than 
    array.reserve(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 4u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
    ASSERT_EQ(array[0], type(10));
    ASSERT_EQ(array[1], type(20));
}

TEST(array, reserve_less_than_count_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.reserve(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them, except moving them
    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    ASSERT_EQ(array[0].id(), 10);
    ASSERT_EQ(array[0].destructor_counter(), nullptr);
    ASSERT_EQ(array[0].constructor_count(), 1u);
    ASSERT_EQ(array[0].copy_constructor_count(), 0u);
    ASSERT_EQ(array[0].move_constructor_count(), 0u);
    ASSERT_EQ(array[0].copy_assign_count(), 0u);
    ASSERT_EQ(array[0].move_assign_count(), 0u);

    ASSERT_EQ(array[1].id(), 20);
    ASSERT_EQ(array[1].destructor_counter(), nullptr);
    ASSERT_EQ(array[1].constructor_count(), 1u);
    ASSERT_EQ(array[1].copy_constructor_count(), 0u);
    ASSERT_EQ(array[1].move_constructor_count(), 0u);
    ASSERT_EQ(array[1].copy_assign_count(), 0u);
    ASSERT_EQ(array[1].move_assign_count(), 0u);

    array.reserve(1);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    ASSERT_EQ(array[0].id(), 10);
    ASSERT_EQ(array[0].destructor_counter(), nullptr);
    ASSERT_EQ(array[0].constructor_count(), 1u);
    ASSERT_EQ(array[0].copy_constructor_count(), 0u);
    ASSERT_EQ(array[0].move_constructor_count(), 0u);
    ASSERT_EQ(array[0].copy_assign_count(), 0u);
    ASSERT_EQ(array[0].move_assign_count(), 0u);

    ASSERT_EQ(array[1].id(), 20);
    ASSERT_EQ(array[1].destructor_counter(), nullptr);
    ASSERT_EQ(array[1].constructor_count(), 1u);
    ASSERT_EQ(array[1].copy_constructor_count(), 0u);
    ASSERT_EQ(array[1].move_constructor_count(), 0u);
    ASSERT_EQ(array[1].copy_assign_count(), 0u);
    ASSERT_EQ(array[1].move_assign_count(), 0u);
}

TEST(array, reserve_less_than_count_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.reserve(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Emplace 2 elements to test we don't touch them, except moving them
    array.emplace_back(10);
    array.emplace_back(20);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
    ASSERT_EQ(array[0], type(10));
    ASSERT_EQ(array[1], type(20));

    array.reserve(1);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
    ASSERT_EQ(array[0], type(10));
    ASSERT_EQ(array[1], type(20));
}