#include <core/containers/array.h>
#include "allocators.h"

GTEST_TEST(array, remove_at_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // remove_at(const index index)
    {
        // Initilize the array
        array_type array;
        array.reserve(5);
        i32 destructor_called[5];
        hud::memory::set_zero(destructor_called);
        array.emplace_back(0, &destructor_called[0]);
        array.emplace_back(1, &destructor_called[1]);
        array.emplace_back(2, &destructor_called[2]);
        array.emplace_back(3, &destructor_called[3]);
        array.emplace_back(4, &destructor_called[4]);

        const type *const buffer = array.data();
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 5u);
        GTEST_ASSERT_EQ(array.max_count(), 5u);
        GTEST_ASSERT_EQ(array.data(), buffer);
        GTEST_ASSERT_EQ(array[0].id(), 0);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[1].id(), 1);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[2].id(), 2);
        GTEST_ASSERT_EQ(array[2].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[3].id(), 3);
        GTEST_ASSERT_EQ(array[3].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[4].id(), 4);
        GTEST_ASSERT_EQ(array[4].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[4].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[4].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

        // Should remove the first without shrinking
        array.remove_at(0);
        GTEST_ASSERT_EQ(array.data(), buffer);
        GTEST_ASSERT_EQ(array.count(), 4u);
        GTEST_ASSERT_EQ(array.max_count(), 5u);
        GTEST_ASSERT_EQ(array[0].id(), 1);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array[1].id(), 2);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array[2].id(), 3);
        GTEST_ASSERT_EQ(array[2].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array[3].id(), 4);
        GTEST_ASSERT_EQ(array[3].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
        GTEST_ASSERT_TRUE(destructor_called[0]);
        GTEST_ASSERT_FALSE(destructor_called[1]);
        GTEST_ASSERT_FALSE(destructor_called[2]);
        GTEST_ASSERT_FALSE(destructor_called[3]);
        GTEST_ASSERT_FALSE(destructor_called[4]);
        hud::memory::set_zero(destructor_called);

        // Should remove at the middle without shrinking
        array.remove_at(1);
        GTEST_ASSERT_EQ(array.data(), buffer);  // no reallocate
        GTEST_ASSERT_EQ(array.count(), 3u);     // remove elements
        GTEST_ASSERT_EQ(array.max_count(), 5u); // no shrink
        GTEST_ASSERT_EQ(array[0].id(), 1);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array[1].id(), 3);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 2u);
        GTEST_ASSERT_EQ(array[2].id(), 4);
        GTEST_ASSERT_EQ(array[2].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_assign_count(), 2u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
        GTEST_ASSERT_FALSE(destructor_called[0]);
        GTEST_ASSERT_FALSE(destructor_called[1]);
        GTEST_ASSERT_TRUE(destructor_called[2]);
        GTEST_ASSERT_FALSE(destructor_called[3]);
        GTEST_ASSERT_TRUE(destructor_called[4]);
        hud::memory::set_zero(destructor_called);

        // Should remove at the end without shrinking
        array.remove_at(array.count() - 1);
        GTEST_ASSERT_EQ(array.data(), buffer);  // no reallocate
        GTEST_ASSERT_EQ(array.count(), 2u);     // remove elements
        GTEST_ASSERT_EQ(array.max_count(), 5u); // no shrink
        GTEST_ASSERT_EQ(array[0].id(), 1);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array[1].id(), 3);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 2u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
        GTEST_ASSERT_FALSE(destructor_called[0]);
        GTEST_ASSERT_FALSE(destructor_called[1]);
        GTEST_ASSERT_FALSE(destructor_called[2]);
        GTEST_ASSERT_FALSE(destructor_called[3]);
        GTEST_ASSERT_TRUE(destructor_called[4]);
    }

    // remove_at(const index index, const usize count_to_remove)
    {
        // Initilize the array
        array_type array;
        array.reserve(8);
        i32 destructor_called[8];
        hud::memory::set_zero(destructor_called);
        array.emplace_back(0, &destructor_called[0]);
        array.emplace_back(1, &destructor_called[1]);
        array.emplace_back(2, &destructor_called[2]);
        array.emplace_back(3, &destructor_called[3]);
        array.emplace_back(4, &destructor_called[4]);
        array.emplace_back(5, &destructor_called[5]);
        array.emplace_back(6, &destructor_called[6]);
        array.emplace_back(7, &destructor_called[7]);

        const type *const buffer = array.data();
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 8u);
        GTEST_ASSERT_EQ(array.max_count(), 8u);
        GTEST_ASSERT_EQ(array.data(), buffer);
        GTEST_ASSERT_EQ(array[0].id(), 0);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[1].id(), 1);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[2].id(), 2);
        GTEST_ASSERT_EQ(array[2].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[3].id(), 3);
        GTEST_ASSERT_EQ(array[3].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[4].id(), 4);
        GTEST_ASSERT_EQ(array[4].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[4].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[4].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[5].id(), 5);
        GTEST_ASSERT_EQ(array[5].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[5].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[5].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[6].id(), 6);
        GTEST_ASSERT_EQ(array[6].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[6].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[6].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[7].id(), 7);
        GTEST_ASSERT_EQ(array[7].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[7].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[7].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

        // Should remove the first without shrinking
        array.remove_at(0, 2);
        GTEST_ASSERT_EQ(array.data(), buffer);
        GTEST_ASSERT_EQ(array.count(), 6u);
        GTEST_ASSERT_EQ(array.max_count(), 8u);
        GTEST_ASSERT_EQ(array[0].id(), 2);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array[1].id(), 3);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array[2].id(), 4);
        GTEST_ASSERT_EQ(array[2].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array[3].id(), 5);
        GTEST_ASSERT_EQ(array[3].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array[4].id(), 6);
        GTEST_ASSERT_EQ(array[4].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[4].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[4].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array[5].id(), 7);
        GTEST_ASSERT_EQ(array[5].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[5].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[5].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
        GTEST_ASSERT_TRUE(destructor_called[0]);
        GTEST_ASSERT_TRUE(destructor_called[1]);
        GTEST_ASSERT_FALSE(destructor_called[2]);
        GTEST_ASSERT_FALSE(destructor_called[3]);
        GTEST_ASSERT_FALSE(destructor_called[4]);
        GTEST_ASSERT_FALSE(destructor_called[5]);
        GTEST_ASSERT_FALSE(destructor_called[6]);
        GTEST_ASSERT_FALSE(destructor_called[7]);
        hud::memory::set_zero(destructor_called);

        // Should remove at the middle without shrinking
        array.remove_at(1, 2);
        GTEST_ASSERT_EQ(array.data(), buffer);  // no reallocate
        GTEST_ASSERT_EQ(array.count(), 4u);     // remove elements
        GTEST_ASSERT_EQ(array.max_count(), 8u); // no shrink
        GTEST_ASSERT_EQ(array[0].id(), 2);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array[1].id(), 5);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 2u);
        GTEST_ASSERT_EQ(array[2].id(), 6);
        GTEST_ASSERT_EQ(array[2].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_assign_count(), 2u);
        GTEST_ASSERT_EQ(array[3].id(), 7);
        GTEST_ASSERT_EQ(array[3].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_assign_count(), 2u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
        GTEST_ASSERT_FALSE(destructor_called[0]);
        GTEST_ASSERT_FALSE(destructor_called[1]);
        GTEST_ASSERT_FALSE(destructor_called[2]);
        GTEST_ASSERT_TRUE(destructor_called[3]);
        GTEST_ASSERT_TRUE(destructor_called[4]);
        GTEST_ASSERT_FALSE(destructor_called[5]);
        GTEST_ASSERT_TRUE(destructor_called[6]);
        GTEST_ASSERT_FALSE(destructor_called[7]);
        hud::memory::set_zero(destructor_called);

        // Should remove at the end without shrinking
        array.remove_at(array.count() - 2, 2);
        GTEST_ASSERT_EQ(array.data(), buffer);  // no reallocate
        GTEST_ASSERT_EQ(array.count(), 2u);     // remove elements
        GTEST_ASSERT_EQ(array.max_count(), 8u); // no shrink
        GTEST_ASSERT_EQ(array[0].id(), 2);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 1u);
        GTEST_ASSERT_EQ(array[1].id(), 5);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 2u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
        GTEST_ASSERT_FALSE(destructor_called[0]);
        GTEST_ASSERT_FALSE(destructor_called[1]);
        GTEST_ASSERT_FALSE(destructor_called[2]);
        GTEST_ASSERT_FALSE(destructor_called[3]);
        GTEST_ASSERT_FALSE(destructor_called[4]);
        GTEST_ASSERT_FALSE(destructor_called[5]);
        GTEST_ASSERT_TRUE(destructor_called[6]);
        GTEST_ASSERT_TRUE(destructor_called[7]);
        hud::memory::set_zero(destructor_called);
    }
}

GTEST_TEST(array, remove_at_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // remove_at(const index index)
    {
        // Initilize the array
        array_type array;
        array.reserve(5);
        array.emplace_back(0);
        array.emplace_back(1);
        array.emplace_back(2);
        array.emplace_back(3);
        array.emplace_back(4);

        const type *const buffer = array.data();
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 5u);
        GTEST_ASSERT_EQ(array.max_count(), 5u);
        GTEST_ASSERT_EQ(array.data(), buffer);
        GTEST_ASSERT_EQ(array[0], 0u);
        GTEST_ASSERT_EQ(array[1], usize(1));
        GTEST_ASSERT_EQ(array[2], usize(2));
        GTEST_ASSERT_EQ(array[3], usize(3));
        GTEST_ASSERT_EQ(array[4], usize(4));
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

        // Should remove the first without shrinking
        array.remove_at(0);
        GTEST_ASSERT_EQ(array.data(), buffer);
        GTEST_ASSERT_EQ(array.count(), 4u);
        GTEST_ASSERT_EQ(array.max_count(), 5u);
        GTEST_ASSERT_EQ(array[0], usize(1));
        GTEST_ASSERT_EQ(array[1], usize(2));
        GTEST_ASSERT_EQ(array[2], usize(3));
        GTEST_ASSERT_EQ(array[3], usize(4));
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

        // Should remove at the middle without shrinking
        array.remove_at(1);
        GTEST_ASSERT_EQ(array.data(), buffer);  // no reallocate
        GTEST_ASSERT_EQ(array.count(), 3u);     // remove elements
        GTEST_ASSERT_EQ(array.max_count(), 5u); // no shrink
        GTEST_ASSERT_EQ(array[0], usize(1));
        GTEST_ASSERT_EQ(array[1], usize(3));
        GTEST_ASSERT_EQ(array[2], usize(4));
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

        // Should remove at the end without shrinking
        array.remove_at(array.count() - 1);
        GTEST_ASSERT_EQ(array.data(), buffer);  // no reallocate
        GTEST_ASSERT_EQ(array.count(), 2u);     // remove elements
        GTEST_ASSERT_EQ(array.max_count(), 5u); // no shrink
        GTEST_ASSERT_EQ(array[0], usize(1));
        GTEST_ASSERT_EQ(array[1], usize(3));
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
    }

    // remove_at(const index index, const usize count_to_remove)
    {
        // Initilize the array
        array_type array;
        array.reserve(8);
        array.emplace_back(0);
        array.emplace_back(1);
        array.emplace_back(2);
        array.emplace_back(3);
        array.emplace_back(4);
        array.emplace_back(5);
        array.emplace_back(6);
        array.emplace_back(7);

        const type *const buffer = array.data();
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 8u);
        GTEST_ASSERT_EQ(array.max_count(), 8u);
        GTEST_ASSERT_EQ(array.data(), buffer);
        GTEST_ASSERT_EQ(array[0], 0u);
        GTEST_ASSERT_EQ(array[1], usize(1));
        GTEST_ASSERT_EQ(array[2], usize(2));
        GTEST_ASSERT_EQ(array[3], usize(3));
        GTEST_ASSERT_EQ(array[4], usize(4));
        GTEST_ASSERT_EQ(array[5], usize(5));
        GTEST_ASSERT_EQ(array[6], usize(6));
        GTEST_ASSERT_EQ(array[7], usize(7));
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

        // Should remove the first without shrinking
        array.remove_at(0, 2);
        GTEST_ASSERT_EQ(array.data(), buffer);
        GTEST_ASSERT_EQ(array.count(), 6u);
        GTEST_ASSERT_EQ(array.max_count(), 8u);
        GTEST_ASSERT_EQ(array[0], usize(2));
        GTEST_ASSERT_EQ(array[1], usize(3));
        GTEST_ASSERT_EQ(array[2], usize(4));
        GTEST_ASSERT_EQ(array[3], usize(5));
        GTEST_ASSERT_EQ(array[4], usize(6));
        GTEST_ASSERT_EQ(array[5], usize(7));
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

        // Should remove at the middle without shrinking
        array.remove_at(1, 2);
        GTEST_ASSERT_EQ(array.data(), buffer);  // no reallocate
        GTEST_ASSERT_EQ(array.count(), 4u);     // remove elements
        GTEST_ASSERT_EQ(array.max_count(), 8u); // no shrink
        GTEST_ASSERT_EQ(array[0], usize(2));
        GTEST_ASSERT_EQ(array[1], usize(5));
        GTEST_ASSERT_EQ(array[2], usize(6));
        GTEST_ASSERT_EQ(array[3], usize(7));
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

        // Should remove at the end without shrinking
        array.remove_at(array.count() - 2, 2);
        GTEST_ASSERT_EQ(array.data(), buffer);  // no reallocate
        GTEST_ASSERT_EQ(array.count(), 2u);     // remove elements
        GTEST_ASSERT_EQ(array.max_count(), 8u); // no shrink
        GTEST_ASSERT_EQ(array[0], usize(2));
        GTEST_ASSERT_EQ(array[1], usize(5));
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);
    }
}

GTEST_TEST(array, remove_at_shrink_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // remove_at(const index index)
    {
        // Initilize the array
        array_type array;
        array.reserve(5);
        i32 destructor_called[5];
        hud::memory::set_zero(destructor_called);
        array.emplace_back(0, &destructor_called[0]);
        array.emplace_back(1, &destructor_called[1]);
        array.emplace_back(2, &destructor_called[2]);
        array.emplace_back(3, &destructor_called[3]);
        array.emplace_back(4, &destructor_called[4]);

        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 5u);
        GTEST_ASSERT_EQ(array.max_count(), 5u);
        GTEST_ASSERT_EQ(array[0].id(), 0);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[1].id(), 1);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[2].id(), 2);
        GTEST_ASSERT_EQ(array[2].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[3].id(), 3);
        GTEST_ASSERT_EQ(array[3].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[4].id(), 4);
        GTEST_ASSERT_EQ(array[4].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[4].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[4].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

        // Should remove the first then shrink
        array.remove_at_shrink(0);
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 4u);
        GTEST_ASSERT_EQ(array.max_count(), 4u);
        GTEST_ASSERT_EQ(array[0].id(), 1);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 1u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[1].id(), 2);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 1u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[2].id(), 3);
        GTEST_ASSERT_EQ(array[2].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_constructor_count(), 1u);
        GTEST_ASSERT_EQ(array[2].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[3].id(), 4);
        GTEST_ASSERT_EQ(array[3].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_constructor_count(), 1u);
        GTEST_ASSERT_EQ(array[3].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 2u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 1u);

        GTEST_ASSERT_TRUE(destructor_called[0]);
        GTEST_ASSERT_TRUE(destructor_called[1]);
        GTEST_ASSERT_TRUE(destructor_called[2]);
        GTEST_ASSERT_TRUE(destructor_called[3]);
        GTEST_ASSERT_TRUE(destructor_called[4]);
        hud::memory::set_zero(destructor_called);

        // Should remove at the middle then shrink
        array.remove_at_shrink(1);
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 3u);
        GTEST_ASSERT_EQ(array.max_count(), 3u);
        GTEST_ASSERT_EQ(array[0].id(), 1);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 2u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[1].id(), 3);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 2u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[2].id(), 4);
        GTEST_ASSERT_EQ(array[2].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_constructor_count(), 2u);
        GTEST_ASSERT_EQ(array[2].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 3u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 2u);

        GTEST_ASSERT_FALSE(destructor_called[0]);
        GTEST_ASSERT_TRUE(destructor_called[1]);
        GTEST_ASSERT_TRUE(destructor_called[2]);
        GTEST_ASSERT_TRUE(destructor_called[3]);
        GTEST_ASSERT_TRUE(destructor_called[4]);
        hud::memory::set_zero(destructor_called);

        // Should remove at the end then shrink
        array.remove_at_shrink(array.count() - 1);
        GTEST_ASSERT_EQ(array.count(), 2u);
        GTEST_ASSERT_EQ(array.max_count(), 2u);
        GTEST_ASSERT_EQ(array[0].id(), 1);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 3u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[1].id(), 3);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 3u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 4u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 3u);

        GTEST_ASSERT_FALSE(destructor_called[0]);
        GTEST_ASSERT_TRUE(destructor_called[1]);
        GTEST_ASSERT_FALSE(destructor_called[2]);
        GTEST_ASSERT_TRUE(destructor_called[3]);
        GTEST_ASSERT_TRUE(destructor_called[4]);
        hud::memory::set_zero(destructor_called);
    }

    // remove_at(const index index, const usize count_to_remove)
    {
        // Initilize the array
        array_type array;
        array.reserve(8);
        i32 destructor_called[8];
        hud::memory::set_zero(destructor_called);
        array.emplace_back(0, &destructor_called[0]);
        array.emplace_back(1, &destructor_called[1]);
        array.emplace_back(2, &destructor_called[2]);
        array.emplace_back(3, &destructor_called[3]);
        array.emplace_back(4, &destructor_called[4]);
        array.emplace_back(5, &destructor_called[5]);
        array.emplace_back(6, &destructor_called[6]);
        array.emplace_back(7, &destructor_called[7]);

        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 8u);
        GTEST_ASSERT_EQ(array.max_count(), 8u);
        GTEST_ASSERT_EQ(array[0].id(), 0);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[1].id(), 1);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[2].id(), 2);
        GTEST_ASSERT_EQ(array[2].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[3].id(), 3);
        GTEST_ASSERT_EQ(array[3].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[4].id(), 4);
        GTEST_ASSERT_EQ(array[4].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[4].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[4].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[5].id(), 5);
        GTEST_ASSERT_EQ(array[5].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[5].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[5].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[6].id(), 6);
        GTEST_ASSERT_EQ(array[6].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[6].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[6].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[7].id(), 7);
        GTEST_ASSERT_EQ(array[7].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[7].move_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[7].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

        // Should remove the first then shrink
        array.remove_at_shrink(0, 2);
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 6u);
        GTEST_ASSERT_EQ(array.max_count(), 6u);
        GTEST_ASSERT_EQ(array[0].id(), 2);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 1u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[1].id(), 3);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 1u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[2].id(), 4);
        GTEST_ASSERT_EQ(array[2].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_constructor_count(), 1u);
        GTEST_ASSERT_EQ(array[2].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[3].id(), 5);
        GTEST_ASSERT_EQ(array[3].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_constructor_count(), 1u);
        GTEST_ASSERT_EQ(array[3].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[4].id(), 6);
        GTEST_ASSERT_EQ(array[4].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[4].move_constructor_count(), 1u);
        GTEST_ASSERT_EQ(array[4].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[5].id(), 7);
        GTEST_ASSERT_EQ(array[5].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[5].move_constructor_count(), 1u);
        GTEST_ASSERT_EQ(array[5].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 2u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 1u);
        GTEST_ASSERT_TRUE(destructor_called[0]);
        GTEST_ASSERT_TRUE(destructor_called[1]);
        GTEST_ASSERT_TRUE(destructor_called[2]);
        GTEST_ASSERT_TRUE(destructor_called[3]);
        GTEST_ASSERT_TRUE(destructor_called[4]);
        GTEST_ASSERT_TRUE(destructor_called[5]);
        GTEST_ASSERT_TRUE(destructor_called[6]);
        GTEST_ASSERT_TRUE(destructor_called[7]);
        hud::memory::set_zero(destructor_called);

        // Should remove at the middle then shrink
        array.remove_at_shrink(1, 2);
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 4u);
        GTEST_ASSERT_EQ(array.max_count(), 4u);
        GTEST_ASSERT_EQ(array[0].id(), 2);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 2u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[1].id(), 5);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 2u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[2].id(), 6);
        GTEST_ASSERT_EQ(array[2].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[2].move_constructor_count(), 2u);
        GTEST_ASSERT_EQ(array[2].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[3].id(), 7);
        GTEST_ASSERT_EQ(array[3].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[3].move_constructor_count(), 2u);
        GTEST_ASSERT_EQ(array[3].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 3u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 2u);
        GTEST_ASSERT_FALSE(destructor_called[0]);
        GTEST_ASSERT_FALSE(destructor_called[1]);
        GTEST_ASSERT_TRUE(destructor_called[2]);
        GTEST_ASSERT_TRUE(destructor_called[3]);
        GTEST_ASSERT_TRUE(destructor_called[4]);
        GTEST_ASSERT_TRUE(destructor_called[5]);
        GTEST_ASSERT_TRUE(destructor_called[6]);
        GTEST_ASSERT_TRUE(destructor_called[7]);
        hud::memory::set_zero(destructor_called);

        // Should remove at the end then shrink
        array.remove_at_shrink(array.count() - 2, 2);
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 2u);
        GTEST_ASSERT_EQ(array.max_count(), 2u);
        GTEST_ASSERT_EQ(array[0].id(), 2);
        GTEST_ASSERT_EQ(array[0].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[0].move_constructor_count(), 3u);
        GTEST_ASSERT_EQ(array[0].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array[1].id(), 5);
        GTEST_ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        GTEST_ASSERT_EQ(array[1].move_constructor_count(), 3u);
        GTEST_ASSERT_EQ(array[1].move_assign_count(), 0u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 4u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 3u);
        GTEST_ASSERT_FALSE(destructor_called[0]);
        GTEST_ASSERT_FALSE(destructor_called[1]);
        GTEST_ASSERT_TRUE(destructor_called[2]);
        GTEST_ASSERT_FALSE(destructor_called[3]);
        GTEST_ASSERT_FALSE(destructor_called[4]);
        GTEST_ASSERT_TRUE(destructor_called[5]);
        GTEST_ASSERT_TRUE(destructor_called[6]);
        GTEST_ASSERT_TRUE(destructor_called[7]);
    }
}

GTEST_TEST(array, remove_at_shrink_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // remove_at(const index index)
    {
        // Initilize the array
        array_type array;
        array.reserve(5);
        array.emplace_back(0);
        array.emplace_back(1);
        array.emplace_back(2);
        array.emplace_back(3);
        array.emplace_back(4);

        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 5u);
        GTEST_ASSERT_EQ(array.max_count(), 5u);
        GTEST_ASSERT_EQ(array[0], 0u);
        GTEST_ASSERT_EQ(array[1], 1u);
        GTEST_ASSERT_EQ(array[2], 2u);
        GTEST_ASSERT_EQ(array[3], 3u);
        GTEST_ASSERT_EQ(array[4], 4u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

        // Should remove the first then shrink
        array.remove_at_shrink(0);
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 4u);
        GTEST_ASSERT_EQ(array.max_count(), 4u);
        GTEST_ASSERT_EQ(array[0], 1u);
        GTEST_ASSERT_EQ(array[1], 2u);
        GTEST_ASSERT_EQ(array[2], 3u);
        GTEST_ASSERT_EQ(array[3], 4u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 2u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 1u);

        // Should remove at the middle then shrink
        array.remove_at_shrink(1);
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 3u);
        GTEST_ASSERT_EQ(array.max_count(), 3u);
        GTEST_ASSERT_EQ(array[0], 1u);
        GTEST_ASSERT_EQ(array[1], 3u);
        GTEST_ASSERT_EQ(array[2], 4u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 3u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 2u);

        // Should remove at the end then shrink
        array.remove_at_shrink(array.count() - 1);
        GTEST_ASSERT_EQ(array.count(), 2u);
        GTEST_ASSERT_EQ(array.max_count(), 2u);
        GTEST_ASSERT_EQ(array[0], 1u);
        GTEST_ASSERT_EQ(array[1], 3u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 4u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 3u);
    }

    // remove_at(const index index, const usize count_to_remove)
    {
        // Initilize the array
        array_type array;
        array.reserve(8);
        array.emplace_back(0);
        array.emplace_back(1);
        array.emplace_back(2);
        array.emplace_back(3);
        array.emplace_back(4);
        array.emplace_back(5);
        array.emplace_back(6);
        array.emplace_back(7);

        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 8u);
        GTEST_ASSERT_EQ(array.max_count(), 8u);
        GTEST_ASSERT_EQ(array[0], 0u);
        GTEST_ASSERT_EQ(array[1], 1u);
        GTEST_ASSERT_EQ(array[2], 2u);
        GTEST_ASSERT_EQ(array[3], 3u);
        GTEST_ASSERT_EQ(array[4], 4u);
        GTEST_ASSERT_EQ(array[5], 5u);
        GTEST_ASSERT_EQ(array[6], 6u);
        GTEST_ASSERT_EQ(array[7], 7u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 1u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 0u);

        // Should remove the first then shrink
        array.remove_at_shrink(0, 2);
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 6u);
        GTEST_ASSERT_EQ(array.max_count(), 6u);
        GTEST_ASSERT_EQ(array[0], 2u);
        GTEST_ASSERT_EQ(array[1], 3u);
        GTEST_ASSERT_EQ(array[2], 4u);
        GTEST_ASSERT_EQ(array[3], 5u);
        GTEST_ASSERT_EQ(array[4], 6u);
        GTEST_ASSERT_EQ(array[5], 7u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 2u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 1u);

        // Should remove at the middle then shrink
        array.remove_at_shrink(1, 2);
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 4u);
        GTEST_ASSERT_EQ(array.max_count(), 4u);
        GTEST_ASSERT_EQ(array[0], 2u);
        GTEST_ASSERT_EQ(array[1], 5u);
        GTEST_ASSERT_EQ(array[2], 6u);
        GTEST_ASSERT_EQ(array[3], 7u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 3u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 2u);

        // Should remove at the end then shrink
        array.remove_at_shrink(array.count() - 2, 2);
        GTEST_ASSERT_NE(array.data(), nullptr);
        GTEST_ASSERT_EQ(array.count(), 2u);
        GTEST_ASSERT_EQ(array.max_count(), 2u);
        GTEST_ASSERT_EQ(array[0], 2u);
        GTEST_ASSERT_EQ(array[1], 5u);
        GTEST_ASSERT_EQ(array.allocator().allocation_count(), 4u);
        GTEST_ASSERT_EQ(array.allocator().free_count(), 3u);
    }
}
