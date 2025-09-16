#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(vector, shrink_to_fit_decrease_allocation)
{
    hud::vector<hud_test::non_bitwise_type, hud_test::allocator_watcher<alignof(hud_test::non_bitwise_type)>> vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Emplace 5 elements and reserve for 10
    vector.reserve(10);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 10u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Shrink to fit
    vector.shrink_to_fit();
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 1u);
}

GTEST_TEST(vector, shrink_to_fit_free_the_array_if_count_is_zero)
{
    i32 destructor_called[10];
    hud::vector<hud_test::non_bitwise_type, hud_test::allocator_watcher<alignof(hud_test::non_bitwise_type)>> vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Emplace 5 elements and reserve for 10
    vector.reserve(10);
    hud::memory::set_memory_zero_safe(destructor_called, 10 * sizeof(bool));

    for (usize index = 0; index < 5; index++) {
        vector.emplace_back(static_cast<i32>(index), &destructor_called[index]);
    }
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 5u);
    hud_assert_eq(vector.max_count(), 10u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
    for (usize index = 0; index < 5; index++) {
        hud_assert_eq(vector[index].constructor_count(), 1u);
        hud_assert_eq(vector[index].copy_constructor_count(), 0u);
        hud_assert_eq(vector[index].move_constructor_count(), 0u);
        hud_assert_eq(vector[index].copy_assign_count(), 0u);
        hud_assert_eq(vector[index].move_assign_count(), 0u);
        hud_assert_false(destructor_called[index]);
    }

    // Shrink to fit
    vector.shrink_to_fit();
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 5u);
    hud_assert_eq(vector.max_count(), 5u);
    hud_assert_eq(vector.allocator().allocation_count(), 2u);
    hud_assert_eq(vector.allocator().free_count(), 1u);
    for (usize index = 0; index < 5; index++) {
        hud_assert_eq(vector[index].constructor_count(), 1u);
        hud_assert_eq(vector[index].copy_constructor_count(), 0u);
        hud_assert_eq(vector[index].move_constructor_count(), 1u);
        hud_assert_eq(vector[index].copy_assign_count(), 0u);
        hud_assert_eq(vector[index].move_assign_count(), 0u);
        hud_assert_true(destructor_called[index]);
    }
}
