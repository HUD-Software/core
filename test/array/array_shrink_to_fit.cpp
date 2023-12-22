#include <core/containers/array.h>
#include "../misc/array_allocators.h"

GTEST_TEST(array, shrink_to_fit_decrease_allocation)
{
    hud::array<hud_test::non_bitwise_type, hud_test::array_allocator<alignof(hud_test::non_bitwise_type)>> array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    // Emplace 5 elements and reserve for 10
    array.reserve(10);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 10u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    // Shrink to fit
    array.shrink_to_fit();
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 1u);
}

GTEST_TEST(array, shrink_to_fit_free_the_array_if_count_is_zero)
{
    i32 destructor_called[10];
    hud::array<hud_test::non_bitwise_type, hud_test::array_allocator<alignof(hud_test::non_bitwise_type)>> array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    // Emplace 5 elements and reserve for 10
    array.reserve(10);
    hud::memory::set_zero(destructor_called, 10 * sizeof(bool));

    for (usize index = 0; index < 5; index++)
    {
        array.emplace_back(static_cast<i32>(index), &destructor_called[index]);
    }
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 5u);
    hud_assert_eq(array.max_count(), 10u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);
    for (usize index = 0; index < 5; index++)
    {
        hud_assert_eq(array[index].constructor_count(), 1u);
        hud_assert_eq(array[index].copy_constructor_count(), 0u);
        hud_assert_eq(array[index].move_constructor_count(), 0u);
        hud_assert_eq(array[index].copy_assign_count(), 0u);
        hud_assert_eq(array[index].move_assign_count(), 0u);
        hud_assert_false(destructor_called[index]);
    }

    // Shrink to fit
    array.shrink_to_fit();
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 5u);
    hud_assert_eq(array.max_count(), 5u);
    hud_assert_eq(array.allocator().allocation_count(), 2u);
    hud_assert_eq(array.allocator().free_count(), 1u);
    for (usize index = 0; index < 5; index++)
    {
        hud_assert_eq(array[index].constructor_count(), 1u);
        hud_assert_eq(array[index].copy_constructor_count(), 0u);
        hud_assert_eq(array[index].move_constructor_count(), 1u);
        hud_assert_eq(array[index].copy_assign_count(), 0u);
        hud_assert_eq(array[index].move_assign_count(), 0u);
        hud_assert_true(destructor_called[index]);
    }
}
