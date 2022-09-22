#include <core/containers/array.h>
#include "allocators.h"

TEST(array, shrink_to_fit_decrease_allocation)
{


    hud::array<hud_test::non_bitwise_type, hud_test::array_allocator<alignof(hud_test::non_bitwise_type)>> array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Emplace 5 elements and reserve for 10
    array.reserve(10);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 10u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Shrink to fit
    array.shrink_to_fit();
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 1u);
}

TEST(array, shrink_to_fit_free_the_array_if_count_is_zero)
{


    hud::array<hud_test::non_bitwise_type, hud_test::array_allocator<alignof(hud_test::non_bitwise_type)>> array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Emplace 5 elements and reserve for 10
    array.reserve(10);
    i32 destructor_called[10];
    hud::memory::set_zero(destructor_called, 10 * sizeof(bool));

    for (usize index = 0; index < 5; index++) {
        array.emplace_back(static_cast<i32>(index), &destructor_called[index]);
    }
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 5u);
    ASSERT_EQ(array.max_count(), 10u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
    for (usize index = 0; index < 5; index++) {
        ASSERT_EQ(array[index].constructor_count(), 1u);
        ASSERT_EQ(array[index].copy_constructor_count(), 0u);
        ASSERT_EQ(array[index].move_constructor_count(), 0u);
        ASSERT_EQ(array[index].copy_assign_count(), 0u);
        ASSERT_EQ(array[index].move_assign_count(), 0u);
        ASSERT_FALSE(destructor_called[index]);
    }

    // Shrink to fit
    array.shrink_to_fit();
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 5u);
    ASSERT_EQ(array.max_count(), 5u);
    ASSERT_EQ(array.allocator().allocation_count(), 2u);
    ASSERT_EQ(array.allocator().free_count(), 1u);
    for (usize index = 0; index < 5; index++) {
        ASSERT_EQ(array[index].constructor_count(), 1u);
        ASSERT_EQ(array[index].copy_constructor_count(), 0u);
        ASSERT_EQ(array[index].move_constructor_count(), 1u);
        ASSERT_EQ(array[index].copy_assign_count(), 0u);
        ASSERT_EQ(array[index].move_assign_count(), 0u);
        ASSERT_TRUE(destructor_called[index]);
    }
}
