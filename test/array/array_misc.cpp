#include <core/containers/array.h>
#include "../misc/array_allocators.h"

GTEST_TEST(array, less_or_equal_size_as_std_vector)
{
    GTEST_ASSERT_LE(sizeof(hud::array<i32>), sizeof(std::vector<i32>));
}

GTEST_TEST(array, count_return_count_of_element)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // Count return 0 if array is empty
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);
    }

    // Count return number of element if not empty
    {
        array_type array({1, 2});
        hud_assert_ne(array.data(), nullptr);
        hud_assert_eq(array.count(), 2u);
        hud_assert_eq(array.max_count(), 2u);
        hud_assert_eq(array.allocator().allocation_count(), 1u);
        hud_assert_eq(array.allocator().free_count(), 0u);
    }
}

GTEST_TEST(array, byte_count_return_count_of_bytes_of_all_elements)
{
    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // Count return 0 if array is empty
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.byte_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);
    }

    // Count return number of element if not empty
    {
        array_type array({1, 2});
        hud_assert_ne(array.data(), nullptr);
        hud_assert_eq(array.count(), 2u);
        hud_assert_eq(array.byte_count(), 2u * sizeof(type));
        hud_assert_eq(array.allocator().allocation_count(), 1u);
        hud_assert_eq(array.allocator().free_count(), 0u);
    }
}

GTEST_TEST(array, max_count_return_allocated_count_of_element)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // Max_count return 0 if array is empty
    {
        array_type array;

        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), array.max_count());
        hud_assert_eq(array.max_count(), 0u);
    }

    // Max_count return maximum number of element allocated
    {
        array_type array;
        array.reserve(4);
        array.emplace_back(10);
        array.emplace_back(20);

        hud_assert_ne(array.data(), nullptr);
        hud_assert_ne(array.count(), array.max_count());
        hud_assert_eq(array.max_count(), 4u);
    }
}

GTEST_TEST(array, max_byte_count_return_count_of_bytes_of_all_elements)
{
    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // Max_count return 0 if array is empty
    {
        array_type array;

        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.byte_count(), array.max_byte_count());
        hud_assert_eq(array.max_byte_count(), 0u);
    }

    // Max_count return maximum number of element allocated
    {
        array_type array;
        array.reserve(4);
        array.emplace_back(10);
        array.emplace_back(20);

        hud_assert_ne(array.data(), nullptr);
        hud_assert_ne(array.byte_count(), array.max_byte_count());
        hud_assert_eq(array.max_byte_count(), 4u * sizeof(type));
    }
}

GTEST_TEST(array, allocator_return_the_allocator)
{

    using type = usize;
    using allocator_type = hud_test::array_allocator<alignof(type)>;
    using array_type = hud::array<type, allocator_type>;

    // Check that the default id is not the one we use to test if the allocator is really the one given to the constructor
    const u32 id = 42;
    hud_assert_ne(allocator_type().id(), id);

    allocator_type allocator(42);
    array_type array({10, 20}, allocator);
    hud_assert_eq(array.allocator().id(), allocator.id());
}

GTEST_TEST(array, data_return_nullptr_if_array_is_empty)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
}

GTEST_TEST(array, data_return_pointer_to_the_first_element_if_array_is_not_empty)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array({1, 2});
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(*array.data(), 1u);
}

GTEST_TEST(array, slack_return_zero_if_array_is_empty)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.slack(), 0u);
}

GTEST_TEST(array, slack_return_zero_if_count_equal_max_count_of_non_empty_array)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array({1, 2});
    hud_assert_false(array.is_empty());
    hud_assert_eq(array.count(), array.max_count());
    hud_assert_eq(array.slack(), 0u);
}

GTEST_TEST(array, slack_return_extra_element_allocated_memory_of_non_empty_array)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array({1, 2});
    array.reserve(4);
    hud_assert_false(array.is_empty());
    hud_assert_ne(array.count(), array.max_count());
    hud_assert_eq(array.slack(), 2u);
}

GTEST_TEST(array, is_valid_index_retur_false_for_empty_array)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    hud_assert_eq(array.data(), nullptr);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 0u);
    hud_assert_eq(array.allocator().allocation_count(), 0u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    hud_assert_false(array.is_valid_index(0));
    hud_assert_false(array.is_valid_index(1));
}

GTEST_TEST(array, is_valid_index_check_if_an_index_can_be_used_to_acces_an_element)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array({1, 2}, 2);
    hud_assert_ne(array.data(), nullptr);
    hud_assert_eq(array.count(), 2u);
    hud_assert_eq(array.max_count(), 4u);
    hud_assert_eq(array.allocator().allocation_count(), 1u);
    hud_assert_eq(array.allocator().free_count(), 0u);

    hud_assert_true(array.is_valid_index(0));
    hud_assert_true(array.is_valid_index(1));
    hud_assert_false(array.is_valid_index(2));
    hud_assert_false(array.is_valid_index(3));
    hud_assert_false(array.is_valid_index(4));
}

GTEST_TEST(array, is_empty_return_if_array_contains_elements_or_not)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    hud_assert_true(array.is_empty());

    array.reserve(4);
    hud_assert_eq(array.count(), 0u);
    hud_assert_eq(array.max_count(), 4u);
    hud_assert_true(array.is_empty());

    array.emplace_back(10);
    hud_assert_eq(array.count(), 1u);
    hud_assert_eq(array.max_count(), 4u);
    hud_assert_false(array.is_empty());
}

GTEST_TEST(array, operator_indexed_accessor_return_the_element_at_the_given_index)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // Non const array
    {
        array_type array({10, 20}, 4);
        hud_assert_eq(array[0], 10u);
        hud_assert_eq(array[1], 20u);
    }

    // const array
    {
        const array_type array({10, 20}, 4);
        hud_assert_eq(array[0], 10u);
        hud_assert_eq(array[1], 20u);
    }
}

GTEST_TEST(array, sub_slice_return_a_slice_on_elements_in_the_array)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // Non const array
    {
        array_type array({10, 20, 30, 40, 50}, 4);
        hud::slice<type> slice = array.sub_slice(0, 5);
        hud_assert_eq(slice.count(), array.count());
        hud_assert_eq(slice.data(), array.data());
    }

    // const array
    {
        const array_type array({10, 20, 30, 40, 50}, 4);
        const hud::slice<type> slice = array.sub_slice(0, 5);
        hud_assert_eq(slice.count(), array.count());
        hud_assert_eq(slice.data(), array.data());
    }
}

GTEST_TEST(array, first_return_the_first_element_in_the_array)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // Non const array
    {
        array_type array({10, 20, 30}, 4);
        hud_assert_eq(array.first(), 10u);
    }

    // const array
    {
        const array_type array({10, 20, 30}, 4);
        hud_assert_eq(array.first(), 10u);
    }
}

GTEST_TEST(array, first_return_the_first_nth_element_in_the_array)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // Non const array
    {
        array_type array({10, 20, 30}, 4);
        hud_assert_eq(array.first(1), 20u);
    }

    // const array
    {
        const array_type array({10, 20, 30}, 4);
        hud_assert_eq(array.first(1), 20u);
    }
}

GTEST_TEST(array, last_return_the_last_element_in_the_array)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // Non const array
    {
        array_type array({10, 20, 30}, 4);
        hud_assert_eq(array.last(), 30u);
    }

    // const array
    {
        const array_type array({10, 20, 30}, 4);
        hud_assert_eq(array.last(), 30u);
    }
}

GTEST_TEST(array, last_return_the_last_nth_element_in_the_array)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    // Non const array
    {
        array_type array({10, 20, 30}, 4);
        hud_assert_eq(array.last(1), 20u);
    }

    // const array
    {
        const array_type array({10, 20, 30}, 4);
        hud_assert_eq(array.last(1), 20u);
    }
}