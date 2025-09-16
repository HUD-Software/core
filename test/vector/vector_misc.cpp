#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(vector, less_or_equal_size_as_std_vector)
{
    GTEST_ASSERT_LE(sizeof(hud::vector<i32>), sizeof(std::vector<i32>));
}

GTEST_TEST(vector, count_return_count_of_element)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // Count return 0 if vector is empty
    {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);
    }

    // Count return number of element if not empty
    {
        array_type vector({1, 2});
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 2u);
        hud_assert_eq(vector.max_count(), 2u);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);
    }
}

GTEST_TEST(vector, byte_count_return_count_of_bytes_of_all_elements)
{
    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // Count return 0 if vector is empty
    {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.byte_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);
    }

    // Count return number of element if not empty
    {
        array_type vector({1, 2});
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 2u);
        hud_assert_eq(vector.byte_count(), 2u * sizeof(type));
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);
    }
}

GTEST_TEST(vector, max_count_return_allocated_count_of_element)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // Max_count return 0 if vector is empty
    {
        array_type vector;

        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), vector.max_count());
        hud_assert_eq(vector.max_count(), 0u);
    }

    // Max_count return maximum number of element allocated
    {
        array_type vector;
        vector.reserve(4);
        vector.emplace_back(10);
        vector.emplace_back(20);

        hud_assert_ne(vector.data(), nullptr);
        hud_assert_ne(vector.count(), vector.max_count());
        hud_assert_eq(vector.max_count(), 4u);
    }
}

GTEST_TEST(vector, max_byte_count_return_count_of_bytes_of_all_elements)
{
    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // Max_count return 0 if vector is empty
    {
        array_type vector;

        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.byte_count(), vector.max_byte_count());
        hud_assert_eq(vector.max_byte_count(), 0u);
    }

    // Max_count return maximum number of element allocated
    {
        array_type vector;
        vector.reserve(4);
        vector.emplace_back(10);
        vector.emplace_back(20);

        hud_assert_ne(vector.data(), nullptr);
        hud_assert_ne(vector.byte_count(), vector.max_byte_count());
        hud_assert_eq(vector.max_byte_count(), 4u * sizeof(type));
    }
}

GTEST_TEST(vector, allocator_return_the_allocator)
{

    using type = usize;
    using allocator_type = hud_test::allocator_watcher<alignof(type)>;
    using array_type = hud::vector<type, allocator_type>;

    // Check that the default id is not the one we use to test if the allocator is really the one given to the constructor
    const u32 id = 42;
    hud_assert_ne(allocator_type().id(), id);

    allocator_type allocator(42);
    array_type vector({10, 20}, allocator);
    hud_assert_eq(vector.allocator().id(), allocator.id());
}

GTEST_TEST(vector, data_return_nullptr_if_array_is_empty)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
}

GTEST_TEST(vector, data_return_pointer_to_the_first_element_if_array_is_not_empty)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector({1, 2});
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(*vector.data(), 1u);
}

GTEST_TEST(vector, slack_return_zero_if_array_is_empty)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.slack(), 0u);
}

GTEST_TEST(vector, slack_return_zero_if_count_equal_max_count_of_non_empty_array)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector({1, 2});
    hud_assert_false(vector.is_empty());
    hud_assert_eq(vector.count(), vector.max_count());
    hud_assert_eq(vector.slack(), 0u);
}

GTEST_TEST(vector, slack_return_extra_element_allocated_memory_of_non_empty_array)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector({1, 2});
    vector.reserve(4);
    hud_assert_false(vector.is_empty());
    hud_assert_ne(vector.count(), vector.max_count());
    hud_assert_eq(vector.slack(), 2u);
}

GTEST_TEST(vector, is_valid_index_retur_false_for_empty_array)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    hud_assert_false(vector.is_valid_index(0));
    hud_assert_false(vector.is_valid_index(1));
}

GTEST_TEST(vector, is_valid_index_check_if_an_index_can_be_used_to_acces_an_element)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector({1, 2}, 2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    hud_assert_true(vector.is_valid_index(0));
    hud_assert_true(vector.is_valid_index(1));
    hud_assert_false(vector.is_valid_index(2));
    hud_assert_false(vector.is_valid_index(3));
    hud_assert_false(vector.is_valid_index(4));
}

GTEST_TEST(vector, is_empty_return_if_array_contains_elements_or_not)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_true(vector.is_empty());

    vector.reserve(4);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_true(vector.is_empty());

    vector.emplace_back(10);
    hud_assert_eq(vector.count(), 1u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_false(vector.is_empty());
}

GTEST_TEST(vector, operator_indexed_accessor_return_the_element_at_the_given_index)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // Non const vector
    {
        array_type vector({10, 20}, 4);
        hud_assert_eq(vector[0], 10u);
        hud_assert_eq(vector[1], 20u);
    }

    // const vector
    {
        const array_type vector({10, 20}, 4);
        hud_assert_eq(vector[0], 10u);
        hud_assert_eq(vector[1], 20u);
    }
}

GTEST_TEST(vector, sub_slice_return_a_slice_on_elements_in_the_array)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // Non const vector
    {
        array_type vector({10, 20, 30, 40, 50}, 4);
        hud::slice<type> slice = vector.sub_slice(0, 5);
        hud_assert_eq(slice.count(), vector.count());
        hud_assert_eq(slice.data(), vector.data());
    }

    // const vector
    {
        const array_type vector({10, 20, 30, 40, 50}, 4);
        const hud::slice<type> slice = vector.sub_slice(0, 5);
        hud_assert_eq(slice.count(), vector.count());
        hud_assert_eq(slice.data(), vector.data());
    }
}

GTEST_TEST(vector, first_return_the_first_element_in_the_array)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // Non const vector
    {
        array_type vector({10, 20, 30}, 4);
        hud_assert_eq(vector.first(), 10u);
    }

    // const vector
    {
        const array_type vector({10, 20, 30}, 4);
        hud_assert_eq(vector.first(), 10u);
    }
}

GTEST_TEST(vector, first_return_the_first_nth_element_in_the_array)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // Non const vector
    {
        array_type vector({10, 20, 30}, 4);
        hud_assert_eq(vector.first(1), 20u);
    }

    // const vector
    {
        const array_type vector({10, 20, 30}, 4);
        hud_assert_eq(vector.first(1), 20u);
    }
}

GTEST_TEST(vector, last_return_the_last_element_in_the_array)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // Non const vector
    {
        array_type vector({10, 20, 30}, 4);
        hud_assert_eq(vector.last(), 30u);
    }

    // const vector
    {
        const array_type vector({10, 20, 30}, 4);
        hud_assert_eq(vector.last(), 30u);
    }
}

GTEST_TEST(vector, last_return_the_last_nth_element_in_the_array)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    // Non const vector
    {
        array_type vector({10, 20, 30}, 4);
        hud_assert_eq(vector.last(1), 20u);
    }

    // const vector
    {
        const array_type vector({10, 20, 30}, 4);
        hud_assert_eq(vector.last(1), 20u);
    }
}