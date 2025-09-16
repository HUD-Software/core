#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(vector, remove_at_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // remove_at(const index index)
    {
        // Initilize the vector
        i32 destructor_called[5];
        array_type vector;
        vector.reserve(5);
        hud::memory::set_memory_zero_safe(destructor_called);
        vector.emplace_back(0, &destructor_called[0]);
        vector.emplace_back(1, &destructor_called[1]);
        vector.emplace_back(2, &destructor_called[2]);
        vector.emplace_back(3, &destructor_called[3]);
        vector.emplace_back(4, &destructor_called[4]);

        const type *const buffer = vector.data();
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 5u);
        hud_assert_eq(vector.max_count(), 5u);
        hud_assert_eq(vector.data(), buffer);
        hud_assert_eq(vector[0].id(), 0);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 1);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_assign_count(), 0u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector[2].id(), 2);
        hud_assert_eq(vector[2].copy_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_assign_count(), 0u);
        hud_assert_eq(vector[2].copy_assign_count(), 0u);
        hud_assert_eq(vector[3].id(), 3);
        hud_assert_eq(vector[3].copy_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_assign_count(), 0u);
        hud_assert_eq(vector[3].copy_assign_count(), 0u);
        hud_assert_eq(vector[4].id(), 4);
        hud_assert_eq(vector[4].copy_constructor_count(), 0u);
        hud_assert_eq(vector[4].move_constructor_count(), 0u);
        hud_assert_eq(vector[4].move_assign_count(), 0u);
        hud_assert_eq(vector[4].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Should remove the first without shrinking
        vector.remove_at(0);
        hud_assert_eq(vector.data(), buffer);
        hud_assert_eq(vector.count(), 4u);
        hud_assert_eq(vector.max_count(), 5u);
        hud_assert_eq(vector[0].id(), 1);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 1u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 2);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_assign_count(), 1u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector[2].id(), 3);
        hud_assert_eq(vector[2].copy_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_assign_count(), 1u);
        hud_assert_eq(vector[3].copy_assign_count(), 0u);
        hud_assert_eq(vector[3].id(), 4);
        hud_assert_eq(vector[3].copy_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_assign_count(), 1u);
        hud_assert_eq(vector[3].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);
        hud_assert_true(destructor_called[0]);
        hud_assert_false(destructor_called[1]);
        hud_assert_false(destructor_called[2]);
        hud_assert_false(destructor_called[3]);
        hud_assert_false(destructor_called[4]);
        hud::memory::set_memory_zero_safe(destructor_called);

        // Should remove at the middle without shrinking
        vector.remove_at(1);
        hud_assert_eq(vector.data(), buffer);  // no reallocate
        hud_assert_eq(vector.count(), 3u);     // remove elements
        hud_assert_eq(vector.max_count(), 5u); // no shrink
        hud_assert_eq(vector[0].id(), 1);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 1u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 3);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 1u);
        hud_assert_eq(vector[1].move_assign_count(), 1u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector[2].id(), 4);
        hud_assert_eq(vector[2].copy_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_assign_count(), 2u);
        hud_assert_eq(vector[2].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);
        hud_assert_false(destructor_called[0]);
        hud_assert_false(destructor_called[1]);
        hud_assert_true(destructor_called[2]);
        hud_assert_false(destructor_called[3]);
        hud_assert_true(destructor_called[4]);
        hud::memory::set_memory_zero_safe(destructor_called);

        // Should remove at the end without shrinking
        vector.remove_at(vector.count() - 1);
        hud_assert_eq(vector.data(), buffer);  // no reallocate
        hud_assert_eq(vector.count(), 2u);     // remove elements
        hud_assert_eq(vector.max_count(), 5u); // no shrink
        hud_assert_eq(vector[0].id(), 1);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 1u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 3);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 1u);
        hud_assert_eq(vector[1].move_assign_count(), 1u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);
        hud_assert_false(destructor_called[0]);
        hud_assert_false(destructor_called[1]);
        hud_assert_false(destructor_called[2]);
        hud_assert_false(destructor_called[3]);
        hud_assert_true(destructor_called[4]);
    }

    // remove_at(const index index, const usize count_to_remove)
    {
        // Initilize the vector
        i32 destructor_called[8];
        array_type vector;
        vector.reserve(8);

        hud::memory::set_memory_zero_safe(destructor_called);
        vector.emplace_back(0, &destructor_called[0]);
        vector.emplace_back(1, &destructor_called[1]);
        vector.emplace_back(2, &destructor_called[2]);
        vector.emplace_back(3, &destructor_called[3]);
        vector.emplace_back(4, &destructor_called[4]);
        vector.emplace_back(5, &destructor_called[5]);
        vector.emplace_back(6, &destructor_called[6]);
        vector.emplace_back(7, &destructor_called[7]);

        const type *const buffer = vector.data();
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 8u);
        hud_assert_eq(vector.max_count(), 8u);
        hud_assert_eq(vector.data(), buffer);
        hud_assert_eq(vector[0].id(), 0);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 1);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_assign_count(), 0u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector[2].id(), 2);
        hud_assert_eq(vector[2].copy_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_assign_count(), 0u);
        hud_assert_eq(vector[2].copy_assign_count(), 0u);
        hud_assert_eq(vector[3].id(), 3);
        hud_assert_eq(vector[3].copy_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_assign_count(), 0u);
        hud_assert_eq(vector[3].copy_assign_count(), 0u);
        hud_assert_eq(vector[4].id(), 4);
        hud_assert_eq(vector[4].copy_constructor_count(), 0u);
        hud_assert_eq(vector[4].move_constructor_count(), 0u);
        hud_assert_eq(vector[4].move_assign_count(), 0u);
        hud_assert_eq(vector[4].copy_assign_count(), 0u);
        hud_assert_eq(vector[5].id(), 5);
        hud_assert_eq(vector[5].copy_constructor_count(), 0u);
        hud_assert_eq(vector[5].move_constructor_count(), 0u);
        hud_assert_eq(vector[5].move_assign_count(), 0u);
        hud_assert_eq(vector[5].copy_assign_count(), 0u);
        hud_assert_eq(vector[6].id(), 6);
        hud_assert_eq(vector[6].copy_constructor_count(), 0u);
        hud_assert_eq(vector[6].move_constructor_count(), 0u);
        hud_assert_eq(vector[6].move_assign_count(), 0u);
        hud_assert_eq(vector[6].copy_assign_count(), 0u);
        hud_assert_eq(vector[7].id(), 7);
        hud_assert_eq(vector[7].copy_constructor_count(), 0u);
        hud_assert_eq(vector[7].move_constructor_count(), 0u);
        hud_assert_eq(vector[7].move_assign_count(), 0u);
        hud_assert_eq(vector[7].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Should remove the first without shrinking
        vector.remove_at(0, 2);
        hud_assert_eq(vector.data(), buffer);
        hud_assert_eq(vector.count(), 6u);
        hud_assert_eq(vector.max_count(), 8u);
        hud_assert_eq(vector[0].id(), 2);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 1u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 3);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 1u);
        hud_assert_eq(vector[1].move_assign_count(), 0u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector[2].id(), 4);
        hud_assert_eq(vector[2].copy_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_assign_count(), 1u);
        hud_assert_eq(vector[2].copy_assign_count(), 0u);
        hud_assert_eq(vector[3].id(), 5);
        hud_assert_eq(vector[3].copy_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_assign_count(), 1u);
        hud_assert_eq(vector[3].copy_assign_count(), 0u);
        hud_assert_eq(vector[4].id(), 6);
        hud_assert_eq(vector[4].copy_constructor_count(), 0u);
        hud_assert_eq(vector[4].move_constructor_count(), 0u);
        hud_assert_eq(vector[4].move_assign_count(), 1u);
        hud_assert_eq(vector[4].copy_assign_count(), 0u);
        hud_assert_eq(vector[5].id(), 7);
        hud_assert_eq(vector[5].copy_constructor_count(), 0u);
        hud_assert_eq(vector[5].move_constructor_count(), 0u);
        hud_assert_eq(vector[5].move_assign_count(), 1u);
        hud_assert_eq(vector[5].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);
        hud_assert_true(destructor_called[0]);
        hud_assert_true(destructor_called[1]);
        hud_assert_false(destructor_called[2]);
        hud_assert_false(destructor_called[3]);
        hud_assert_false(destructor_called[4]);
        hud_assert_false(destructor_called[5]);
        hud_assert_false(destructor_called[6]);
        hud_assert_false(destructor_called[7]);
        hud::memory::set_memory_zero_safe(destructor_called);

        // Should remove at the middle without shrinking
        vector.remove_at(1, 2);
        hud_assert_eq(vector.data(), buffer);  // no reallocate
        hud_assert_eq(vector.count(), 4u);     // remove elements
        hud_assert_eq(vector.max_count(), 8u); // no shrink
        hud_assert_eq(vector[0].id(), 2);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 1u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 5);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 1u);
        hud_assert_eq(vector[1].move_assign_count(), 1u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector[2].id(), 6);
        hud_assert_eq(vector[2].copy_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_constructor_count(), 1u);
        hud_assert_eq(vector[2].move_assign_count(), 1u);
        hud_assert_eq(vector[2].copy_assign_count(), 0u);
        hud_assert_eq(vector[3].id(), 7);
        hud_assert_eq(vector[3].copy_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_assign_count(), 2u);
        hud_assert_eq(vector[3].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);
        hud_assert_false(destructor_called[0]);
        hud_assert_false(destructor_called[1]);
        hud_assert_false(destructor_called[2]);
        hud_assert_true(destructor_called[3]);
        hud_assert_true(destructor_called[4]);
        hud_assert_false(destructor_called[5]);
        hud_assert_true(destructor_called[6]);
        hud_assert_false(destructor_called[7]);
        hud::memory::set_memory_zero_safe(destructor_called);

        // Should remove at the end without shrinking
        vector.remove_at(vector.count() - 2, 2);
        hud_assert_eq(vector.data(), buffer);  // no reallocate
        hud_assert_eq(vector.count(), 2u);     // remove elements
        hud_assert_eq(vector.max_count(), 8u); // no shrink
        hud_assert_eq(vector[0].id(), 2);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 1u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 5);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 1u);
        hud_assert_eq(vector[1].move_assign_count(), 1u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);
        hud_assert_false(destructor_called[0]);
        hud_assert_false(destructor_called[1]);
        hud_assert_false(destructor_called[2]);
        hud_assert_false(destructor_called[3]);
        hud_assert_false(destructor_called[4]);
        hud_assert_false(destructor_called[5]);
        hud_assert_true(destructor_called[6]);
        hud_assert_true(destructor_called[7]);
        hud::memory::set_memory_zero_safe(destructor_called);
    }
}

GTEST_TEST(vector, remove_at_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // remove_at(const index index)
    {
        // Initilize the vector
        array_type vector;
        vector.reserve(5);
        vector.emplace_back(0);
        vector.emplace_back(1);
        vector.emplace_back(2);
        vector.emplace_back(3);
        vector.emplace_back(4);

        const type *const buffer = vector.data();
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 5u);
        hud_assert_eq(vector.max_count(), 5u);
        hud_assert_eq(vector.data(), buffer);
        hud_assert_eq(vector[0], 0u);
        hud_assert_eq(vector[1], usize(1));
        hud_assert_eq(vector[2], usize(2));
        hud_assert_eq(vector[3], usize(3));
        hud_assert_eq(vector[4], usize(4));
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Should remove the first without shrinking
        vector.remove_at(0);
        hud_assert_eq(vector.data(), buffer);
        hud_assert_eq(vector.count(), 4u);
        hud_assert_eq(vector.max_count(), 5u);
        hud_assert_eq(vector[0], usize(1));
        hud_assert_eq(vector[1], usize(2));
        hud_assert_eq(vector[2], usize(3));
        hud_assert_eq(vector[3], usize(4));
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Should remove at the middle without shrinking
        vector.remove_at(1);
        hud_assert_eq(vector.data(), buffer);  // no reallocate
        hud_assert_eq(vector.count(), 3u);     // remove elements
        hud_assert_eq(vector.max_count(), 5u); // no shrink
        hud_assert_eq(vector[0], usize(1));
        hud_assert_eq(vector[1], usize(3));
        hud_assert_eq(vector[2], usize(4));
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Should remove at the end without shrinking
        vector.remove_at(vector.count() - 1);
        hud_assert_eq(vector.data(), buffer);  // no reallocate
        hud_assert_eq(vector.count(), 2u);     // remove elements
        hud_assert_eq(vector.max_count(), 5u); // no shrink
        hud_assert_eq(vector[0], usize(1));
        hud_assert_eq(vector[1], usize(3));
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);
    }

    // remove_at(const index index, const usize count_to_remove)
    {
        // Initilize the vector
        array_type vector;
        vector.reserve(8);
        vector.emplace_back(0);
        vector.emplace_back(1);
        vector.emplace_back(2);
        vector.emplace_back(3);
        vector.emplace_back(4);
        vector.emplace_back(5);
        vector.emplace_back(6);
        vector.emplace_back(7);

        const type *const buffer = vector.data();
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 8u);
        hud_assert_eq(vector.max_count(), 8u);
        hud_assert_eq(vector.data(), buffer);
        hud_assert_eq(vector[0], 0u);
        hud_assert_eq(vector[1], usize(1));
        hud_assert_eq(vector[2], usize(2));
        hud_assert_eq(vector[3], usize(3));
        hud_assert_eq(vector[4], usize(4));
        hud_assert_eq(vector[5], usize(5));
        hud_assert_eq(vector[6], usize(6));
        hud_assert_eq(vector[7], usize(7));
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Should remove the first without shrinking
        vector.remove_at(0, 2);
        hud_assert_eq(vector.data(), buffer);
        hud_assert_eq(vector.count(), 6u);
        hud_assert_eq(vector.max_count(), 8u);
        hud_assert_eq(vector[0], usize(2));
        hud_assert_eq(vector[1], usize(3));
        hud_assert_eq(vector[2], usize(4));
        hud_assert_eq(vector[3], usize(5));
        hud_assert_eq(vector[4], usize(6));
        hud_assert_eq(vector[5], usize(7));
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Should remove at the middle without shrinking
        vector.remove_at(1, 2);
        hud_assert_eq(vector.data(), buffer);  // no reallocate
        hud_assert_eq(vector.count(), 4u);     // remove elements
        hud_assert_eq(vector.max_count(), 8u); // no shrink
        hud_assert_eq(vector[0], usize(2));
        hud_assert_eq(vector[1], usize(5));
        hud_assert_eq(vector[2], usize(6));
        hud_assert_eq(vector[3], usize(7));
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Should remove at the end without shrinking
        vector.remove_at(vector.count() - 2, 2);
        hud_assert_eq(vector.data(), buffer);  // no reallocate
        hud_assert_eq(vector.count(), 2u);     // remove elements
        hud_assert_eq(vector.max_count(), 8u); // no shrink
        hud_assert_eq(vector[0], usize(2));
        hud_assert_eq(vector[1], usize(5));
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);
    }
}

GTEST_TEST(vector, remove_at_shrink_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // remove_at(const index index)
    {
        // Initilize the vector
        i32 destructor_called[5];
        array_type vector;
        vector.reserve(5);
        hud::memory::set_memory_zero_safe(destructor_called);
        vector.emplace_back(0, &destructor_called[0]);
        vector.emplace_back(1, &destructor_called[1]);
        vector.emplace_back(2, &destructor_called[2]);
        vector.emplace_back(3, &destructor_called[3]);
        vector.emplace_back(4, &destructor_called[4]);

        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 5u);
        hud_assert_eq(vector.max_count(), 5u);
        hud_assert_eq(vector[0].id(), 0);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 1);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_assign_count(), 0u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector[2].id(), 2);
        hud_assert_eq(vector[2].copy_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_assign_count(), 0u);
        hud_assert_eq(vector[2].copy_assign_count(), 0u);
        hud_assert_eq(vector[3].id(), 3);
        hud_assert_eq(vector[3].copy_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_assign_count(), 0u);
        hud_assert_eq(vector[3].copy_assign_count(), 0u);
        hud_assert_eq(vector[4].id(), 4);
        hud_assert_eq(vector[4].copy_constructor_count(), 0u);
        hud_assert_eq(vector[4].move_constructor_count(), 0u);
        hud_assert_eq(vector[4].move_assign_count(), 0u);
        hud_assert_eq(vector[4].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Should remove the first then shrink
        vector.remove_at_shrink(0);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 4u);
        hud_assert_eq(vector.max_count(), 4u);
        hud_assert_eq(vector[0].id(), 1);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 1u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 2);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 1u);
        hud_assert_eq(vector[1].move_assign_count(), 0u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector[2].id(), 3);
        hud_assert_eq(vector[2].copy_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_constructor_count(), 1u);
        hud_assert_eq(vector[2].move_assign_count(), 0u);
        hud_assert_eq(vector[2].copy_assign_count(), 0u);
        hud_assert_eq(vector[3].id(), 4);
        hud_assert_eq(vector[3].copy_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_constructor_count(), 1u);
        hud_assert_eq(vector[3].move_assign_count(), 0u);
        hud_assert_eq(vector[3].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 2u);
        hud_assert_eq(vector.allocator().free_count(), 1u);

        hud_assert_true(destructor_called[0]);
        hud_assert_true(destructor_called[1]);
        hud_assert_true(destructor_called[2]);
        hud_assert_true(destructor_called[3]);
        hud_assert_true(destructor_called[4]);
        hud::memory::set_memory_zero_safe(destructor_called);

        // Should remove at the middle then shrink
        vector.remove_at_shrink(1);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 3u);
        hud_assert_eq(vector.max_count(), 3u);
        hud_assert_eq(vector[0].id(), 1);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 2u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 3);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 2u);
        hud_assert_eq(vector[1].move_assign_count(), 0u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector[2].id(), 4);
        hud_assert_eq(vector[2].copy_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_constructor_count(), 2u);
        hud_assert_eq(vector[2].move_assign_count(), 0u);
        hud_assert_eq(vector[2].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 3u);
        hud_assert_eq(vector.allocator().free_count(), 2u);

        hud_assert_false(destructor_called[0]);
        hud_assert_true(destructor_called[1]);
        hud_assert_true(destructor_called[2]);
        hud_assert_true(destructor_called[3]);
        hud_assert_true(destructor_called[4]);
        hud::memory::set_memory_zero_safe(destructor_called);

        // Should remove at the end then shrink
        vector.remove_at_shrink(vector.count() - 1);
        hud_assert_eq(vector.count(), 2u);
        hud_assert_eq(vector.max_count(), 2u);
        hud_assert_eq(vector[0].id(), 1);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 3u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 3);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 3u);
        hud_assert_eq(vector[1].move_assign_count(), 0u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 4u);
        hud_assert_eq(vector.allocator().free_count(), 3u);

        hud_assert_false(destructor_called[0]);
        hud_assert_true(destructor_called[1]);
        hud_assert_false(destructor_called[2]);
        hud_assert_true(destructor_called[3]);
        hud_assert_true(destructor_called[4]);
        hud::memory::set_memory_zero_safe(destructor_called);
    }

    // remove_at(const index index, const usize count_to_remove)
    {
        // Initilize the vector
        i32 destructor_called[8];
        array_type vector;
        vector.reserve(8);
        hud::memory::set_memory_zero_safe(destructor_called);
        vector.emplace_back(0, &destructor_called[0]);
        vector.emplace_back(1, &destructor_called[1]);
        vector.emplace_back(2, &destructor_called[2]);
        vector.emplace_back(3, &destructor_called[3]);
        vector.emplace_back(4, &destructor_called[4]);
        vector.emplace_back(5, &destructor_called[5]);
        vector.emplace_back(6, &destructor_called[6]);
        vector.emplace_back(7, &destructor_called[7]);

        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 8u);
        hud_assert_eq(vector.max_count(), 8u);
        hud_assert_eq(vector[0].id(), 0);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 1);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_assign_count(), 0u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector[2].id(), 2);
        hud_assert_eq(vector[2].copy_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_assign_count(), 0u);
        hud_assert_eq(vector[2].copy_assign_count(), 0u);
        hud_assert_eq(vector[3].id(), 3);
        hud_assert_eq(vector[3].copy_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_assign_count(), 0u);
        hud_assert_eq(vector[3].copy_assign_count(), 0u);
        hud_assert_eq(vector[4].id(), 4);
        hud_assert_eq(vector[4].copy_constructor_count(), 0u);
        hud_assert_eq(vector[4].move_constructor_count(), 0u);
        hud_assert_eq(vector[4].move_assign_count(), 0u);
        hud_assert_eq(vector[4].copy_assign_count(), 0u);
        hud_assert_eq(vector[5].id(), 5);
        hud_assert_eq(vector[5].copy_constructor_count(), 0u);
        hud_assert_eq(vector[5].move_constructor_count(), 0u);
        hud_assert_eq(vector[5].move_assign_count(), 0u);
        hud_assert_eq(vector[5].copy_assign_count(), 0u);
        hud_assert_eq(vector[6].id(), 6);
        hud_assert_eq(vector[6].copy_constructor_count(), 0u);
        hud_assert_eq(vector[6].move_constructor_count(), 0u);
        hud_assert_eq(vector[6].move_assign_count(), 0u);
        hud_assert_eq(vector[6].copy_assign_count(), 0u);
        hud_assert_eq(vector[7].id(), 7);
        hud_assert_eq(vector[7].copy_constructor_count(), 0u);
        hud_assert_eq(vector[7].move_constructor_count(), 0u);
        hud_assert_eq(vector[7].move_assign_count(), 0u);
        hud_assert_eq(vector[7].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Should remove the first then shrink
        vector.remove_at_shrink(0, 2);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 6u);
        hud_assert_eq(vector.max_count(), 6u);
        hud_assert_eq(vector[0].id(), 2);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 1u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 3);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 1u);
        hud_assert_eq(vector[1].move_assign_count(), 0u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector[2].id(), 4);
        hud_assert_eq(vector[2].copy_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_constructor_count(), 1u);
        hud_assert_eq(vector[2].move_assign_count(), 0u);
        hud_assert_eq(vector[2].copy_assign_count(), 0u);
        hud_assert_eq(vector[3].id(), 5);
        hud_assert_eq(vector[3].copy_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_constructor_count(), 1u);
        hud_assert_eq(vector[3].move_assign_count(), 0u);
        hud_assert_eq(vector[3].copy_assign_count(), 0u);
        hud_assert_eq(vector[4].id(), 6);
        hud_assert_eq(vector[4].copy_constructor_count(), 0u);
        hud_assert_eq(vector[4].move_constructor_count(), 1u);
        hud_assert_eq(vector[4].move_assign_count(), 0u);
        hud_assert_eq(vector[4].copy_assign_count(), 0u);
        hud_assert_eq(vector[5].id(), 7);
        hud_assert_eq(vector[5].copy_constructor_count(), 0u);
        hud_assert_eq(vector[5].move_constructor_count(), 1u);
        hud_assert_eq(vector[5].move_assign_count(), 0u);
        hud_assert_eq(vector[5].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 2u);
        hud_assert_eq(vector.allocator().free_count(), 1u);
        hud_assert_true(destructor_called[0]);
        hud_assert_true(destructor_called[1]);
        hud_assert_true(destructor_called[2]);
        hud_assert_true(destructor_called[3]);
        hud_assert_true(destructor_called[4]);
        hud_assert_true(destructor_called[5]);
        hud_assert_true(destructor_called[6]);
        hud_assert_true(destructor_called[7]);
        hud::memory::set_memory_zero_safe(destructor_called);

        // Should remove at the middle then shrink
        vector.remove_at_shrink(1, 2);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 4u);
        hud_assert_eq(vector.max_count(), 4u);
        hud_assert_eq(vector[0].id(), 2);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 2u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 5);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 2u);
        hud_assert_eq(vector[1].move_assign_count(), 0u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector[2].id(), 6);
        hud_assert_eq(vector[2].copy_constructor_count(), 0u);
        hud_assert_eq(vector[2].move_constructor_count(), 2u);
        hud_assert_eq(vector[2].move_assign_count(), 0u);
        hud_assert_eq(vector[2].copy_assign_count(), 0u);
        hud_assert_eq(vector[3].id(), 7);
        hud_assert_eq(vector[3].copy_constructor_count(), 0u);
        hud_assert_eq(vector[3].move_constructor_count(), 2u);
        hud_assert_eq(vector[3].move_assign_count(), 0u);
        hud_assert_eq(vector[3].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 3u);
        hud_assert_eq(vector.allocator().free_count(), 2u);
        hud_assert_false(destructor_called[0]);
        hud_assert_false(destructor_called[1]);
        hud_assert_true(destructor_called[2]);
        hud_assert_true(destructor_called[3]);
        hud_assert_true(destructor_called[4]);
        hud_assert_true(destructor_called[5]);
        hud_assert_true(destructor_called[6]);
        hud_assert_true(destructor_called[7]);
        hud::memory::set_memory_zero_safe(destructor_called);

        // Should remove at the end then shrink
        vector.remove_at_shrink(vector.count() - 2, 2);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 2u);
        hud_assert_eq(vector.max_count(), 2u);
        hud_assert_eq(vector[0].id(), 2);
        hud_assert_eq(vector[0].copy_constructor_count(), 0u);
        hud_assert_eq(vector[0].move_constructor_count(), 3u);
        hud_assert_eq(vector[0].move_assign_count(), 0u);
        hud_assert_eq(vector[0].copy_assign_count(), 0u);
        hud_assert_eq(vector[1].id(), 5);
        hud_assert_eq(vector[1].copy_constructor_count(), 0u);
        hud_assert_eq(vector[1].move_constructor_count(), 3u);
        hud_assert_eq(vector[1].move_assign_count(), 0u);
        hud_assert_eq(vector[1].copy_assign_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 4u);
        hud_assert_eq(vector.allocator().free_count(), 3u);
        hud_assert_false(destructor_called[0]);
        hud_assert_false(destructor_called[1]);
        hud_assert_true(destructor_called[2]);
        hud_assert_false(destructor_called[3]);
        hud_assert_false(destructor_called[4]);
        hud_assert_true(destructor_called[5]);
        hud_assert_true(destructor_called[6]);
        hud_assert_true(destructor_called[7]);
    }
}

GTEST_TEST(vector, remove_at_shrink_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // remove_at(const index index)
    {
        // Initilize the vector
        array_type vector;
        vector.reserve(5);
        vector.emplace_back(0);
        vector.emplace_back(1);
        vector.emplace_back(2);
        vector.emplace_back(3);
        vector.emplace_back(4);

        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 5u);
        hud_assert_eq(vector.max_count(), 5u);
        hud_assert_eq(vector[0], 0u);
        hud_assert_eq(vector[1], 1u);
        hud_assert_eq(vector[2], 2u);
        hud_assert_eq(vector[3], 3u);
        hud_assert_eq(vector[4], 4u);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Should remove the first then shrink
        vector.remove_at_shrink(0);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 4u);
        hud_assert_eq(vector.max_count(), 4u);
        hud_assert_eq(vector[0], 1u);
        hud_assert_eq(vector[1], 2u);
        hud_assert_eq(vector[2], 3u);
        hud_assert_eq(vector[3], 4u);
        hud_assert_eq(vector.allocator().allocation_count(), 2u);
        hud_assert_eq(vector.allocator().free_count(), 1u);

        // Should remove at the middle then shrink
        vector.remove_at_shrink(1);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 3u);
        hud_assert_eq(vector.max_count(), 3u);
        hud_assert_eq(vector[0], 1u);
        hud_assert_eq(vector[1], 3u);
        hud_assert_eq(vector[2], 4u);
        hud_assert_eq(vector.allocator().allocation_count(), 3u);
        hud_assert_eq(vector.allocator().free_count(), 2u);

        // Should remove at the end then shrink
        vector.remove_at_shrink(vector.count() - 1);
        hud_assert_eq(vector.count(), 2u);
        hud_assert_eq(vector.max_count(), 2u);
        hud_assert_eq(vector[0], 1u);
        hud_assert_eq(vector[1], 3u);
        hud_assert_eq(vector.allocator().allocation_count(), 4u);
        hud_assert_eq(vector.allocator().free_count(), 3u);

        // Remove the rest
        vector.remove_at_shrink(0);
        hud_assert_eq(vector.count(), 1u);
        hud_assert_eq(vector.max_count(), 1u);
        hud_assert_eq(vector[0], 3u);
        hud_assert_eq(vector.allocator().allocation_count(), 5u);
        hud_assert_eq(vector.allocator().free_count(), 4u);
        vector.remove_at_shrink(0);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 5u);
        hud_assert_eq(vector.allocator().free_count(), 5u);

        // Remove when empty
        vector.remove_at_shrink(0);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 5u);
        hud_assert_eq(vector.allocator().free_count(), 5u);
        vector.remove_at_shrink(1);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 5u);
        hud_assert_eq(vector.allocator().free_count(), 5u);
    }

    // remove_at(const index index, const usize count_to_remove)
    {
        // Initilize the vector
        array_type vector;
        vector.reserve(8);
        vector.emplace_back(0);
        vector.emplace_back(1);
        vector.emplace_back(2);
        vector.emplace_back(3);
        vector.emplace_back(4);
        vector.emplace_back(5);
        vector.emplace_back(6);
        vector.emplace_back(7);

        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 8u);
        hud_assert_eq(vector.max_count(), 8u);
        hud_assert_eq(vector[0], 0u);
        hud_assert_eq(vector[1], 1u);
        hud_assert_eq(vector[2], 2u);
        hud_assert_eq(vector[3], 3u);
        hud_assert_eq(vector[4], 4u);
        hud_assert_eq(vector[5], 5u);
        hud_assert_eq(vector[6], 6u);
        hud_assert_eq(vector[7], 7u);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Should remove the first then shrink
        vector.remove_at_shrink(0, 2);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 6u);
        hud_assert_eq(vector.max_count(), 6u);
        hud_assert_eq(vector[0], 2u);
        hud_assert_eq(vector[1], 3u);
        hud_assert_eq(vector[2], 4u);
        hud_assert_eq(vector[3], 5u);
        hud_assert_eq(vector[4], 6u);
        hud_assert_eq(vector[5], 7u);
        hud_assert_eq(vector.allocator().allocation_count(), 2u);
        hud_assert_eq(vector.allocator().free_count(), 1u);

        // Should remove at the middle then shrink
        vector.remove_at_shrink(1, 2);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 4u);
        hud_assert_eq(vector.max_count(), 4u);
        hud_assert_eq(vector[0], 2u);
        hud_assert_eq(vector[1], 5u);
        hud_assert_eq(vector[2], 6u);
        hud_assert_eq(vector[3], 7u);
        hud_assert_eq(vector.allocator().allocation_count(), 3u);
        hud_assert_eq(vector.allocator().free_count(), 2u);

        // Should remove at the end then shrink
        vector.remove_at_shrink(vector.count() - 2, 2);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 2u);
        hud_assert_eq(vector.max_count(), 2u);
        hud_assert_eq(vector[0], 2u);
        hud_assert_eq(vector[1], 5u);
        hud_assert_eq(vector.allocator().allocation_count(), 4u);
        hud_assert_eq(vector.allocator().free_count(), 3u);

        // Remove the rest
        vector.remove_at_shrink(0, 2);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 4u);
        hud_assert_eq(vector.allocator().free_count(), 4u);

        // Remove when empty
        vector.remove_at_shrink(0, 0);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 4u);
        hud_assert_eq(vector.allocator().free_count(), 4u);
        vector.remove_at_shrink(0, 2);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 4u);
        hud_assert_eq(vector.allocator().free_count(), 4u);
    }
}
