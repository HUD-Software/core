#include <core/containers/array.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(array, emplace_at_to_ref_can_default_construct_non_trivially_default_constructible_type)
{

    using type = hud_test::default_constructible_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_trivially_default_constructible_v<type>);
    static_assert(hud::is_default_constructible_v<type>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(array.count());

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(&array[0], &first_element);
            hud_assert_eq(array[0].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[0].default_constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(array.count());

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[0].default_constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 1u);
            hud_assert_eq(&array[1], &second_element);
            hud_assert_eq(array[1].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[1].default_constructor_count(), 1u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].move_constructor_count(), 0u);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(&array[0], &first_element);
            hud_assert_eq(array[0].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[0].default_constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(1);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(&array[0], &first_element);
            hud_assert_eq(array[0].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[0].default_constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);
            hud_assert_eq(&array[1], &second_element);
            hud_assert_eq(array[1].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[1].default_constructor_count(), 1u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].move_constructor_count(), 0u);
        }
    }

    // Emplace at the middle of an array
    {
        // Reallocation
        {
            // The array with 4 elements with ID [1,2,3,4]
            array_type array;
            array.reserve(4);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(3);
            array.emplace_back(4);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 4u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &element = array.emplace_at_to_ref(array.count() / 2u);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].default_constructor_count(), 0u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 1u);
            hud_assert_eq(array[1].id(), 2);
            hud_assert_eq(array[1].default_constructor_count(), 0u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].move_constructor_count(), 1u);
            hud_assert_eq(&array[2], &element);
            hud_assert_eq(array[2].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[2].default_constructor_count(), 1u);
            hud_assert_eq(array[2].copy_constructor_count(), 0u);
            hud_assert_eq(array[2].move_constructor_count(), 0u);
            hud_assert_eq(array[3].id(), 3);
            hud_assert_eq(array[3].default_constructor_count(), 0u);
            hud_assert_eq(array[3].copy_constructor_count(), 0u);
            hud_assert_eq(array[3].move_constructor_count(), 1u);
            hud_assert_eq(array[4].id(), 4);
            hud_assert_eq(array[4].default_constructor_count(), 0u);
            hud_assert_eq(array[4].copy_constructor_count(), 0u);
            hud_assert_eq(array[4].move_constructor_count(), 1u);
        }

        // No reallocation
        {
            // The array with 4 elements with ID [1,2,3,4]
            array_type array;
            array.reserve(5);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(3);
            array.emplace_back(4);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace element in the middle
            const type &element = array.emplace_at_to_ref(array.count() / 2u);

            // Ensure we correctly emplaced the element.
            // Array should be [1,2,type::DEFAULT_ID_VALUE,3,4]
            // Only 4 and 5 are copied
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].default_constructor_count(), 0u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);
            hud_assert_eq(array[1].id(), 2);
            hud_assert_eq(array[1].default_constructor_count(), 0u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].move_constructor_count(), 0u);
            hud_assert_eq(&array[2], &element);
            hud_assert_eq(array[2].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[2].default_constructor_count(), 1u);
            hud_assert_eq(array[2].copy_constructor_count(), 0u);
            hud_assert_eq(array[2].move_constructor_count(), 0u);
            hud_assert_eq(array[3].id(), 3);
            hud_assert_eq(array[3].default_constructor_count(), 0u);
            hud_assert_eq(array[3].copy_constructor_count(), 0u);
            hud_assert_eq(array[3].move_constructor_count(), 1u);
            hud_assert_eq(array[4].id(), 4);
            hud_assert_eq(array[4].default_constructor_count(), 0u);
            hud_assert_eq(array[4].copy_constructor_count(), 0u);
            hud_assert_eq(array[4].move_constructor_count(), 1u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(&array[0], &first_element);
            hud_assert_eq(array[0].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[0].default_constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[0].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[0].default_constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);
            hud_assert_eq(array[1].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[1].default_constructor_count(), 1u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].move_constructor_count(), 1u);
        }

        // No reallocation
        {
            // The empty array
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(&array[0], &first_element);
            hud_assert_eq(array[0].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[0].default_constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[0].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[0].default_constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);
            hud_assert_eq(array[1].id(), type::DEFAULT_ID_VALUE);
            hud_assert_eq(array[1].default_constructor_count(), 1u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].move_constructor_count(), 1u);
            hud_assert_ne(&array[1], &first_element);
            hud_assert_eq(&first_element, &second_element);
        }
    }
}

GTEST_TEST(array, emplace_at_to_ref_can_default_construct_trivially_default_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_trivially_default_constructible_v<type>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(array.count());

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(&array[0], &first_element);
            hud_assert_eq(array[0], 0u);

            // Change the value to differanciate it from default value
            array[0] = 1u;

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(array.count());

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(array[1], 0u);
            hud_assert_eq(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 0u);
            hud_assert_eq(&array[0], &first_element);

            // Change the value to differanciate it from default value
            array[0] = 1u;

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(1);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(&array[0], &first_element);
            hud_assert_eq(array[1], 0u);
            hud_assert_eq(&array[1], &second_element);
        }
    }

    // Emplace at the middle of an array
    {
        // Reallocation
        {
            // The array with 4 elements with ID [1,2,3,4]
            array_type array;
            array.reserve(4);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(3);
            array.emplace_back(4);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 4u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(array[1], 2u);
            hud_assert_eq(array[2], 0u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3], 3u);
            hud_assert_eq(array[4], 4u);
        }

        // No reallocation
        {
            // The array with 4 elements with ID [1,2,3,4]
            array_type array;
            array.reserve(5);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(3);
            array.emplace_back(4);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace element in the middle
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u);

            // Ensure we correctly emplaced the element.
            // Array should be [1,2,0,3,4]
            // Only 4 and 5 are copied
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(array[1], 2u);
            hud_assert_eq(array[2], 0u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3], 3u);
            hud_assert_eq(array[4], 4u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 0u);
            hud_assert_eq(&array[0], &first_element);

            // Change the value to differanciate it from default value
            array[0] = 1u;

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0], 0u);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[1], 1u);
        }

        // No reallocation
        {
            // The empty array
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 0u);
            hud_assert_eq(&array[0], &first_element);

            // Change the value to differanciate it from default value
            array[0] = 1u;

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 0u);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[1], 1u);
            hud_assert_ne(&array[1], &first_element);
            hud_assert_eq(&first_element, &second_element);
        }
    }
}

GTEST_TEST(array, emplace_at_to_ref_can_construct_non_trivially_constructible_type)
{

    using type = hud_test::NonDefaultConstructibleType;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_trivially_constructible_v<type, usize>);
    static_assert(hud::is_constructible_v<type, usize>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(array.count(), 1);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);
            hud_assert_eq(&array[0], &first_element);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(array.count(), 2);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 1u);
            hud_assert_eq(array[1].id(), 2);
            hud_assert_eq(array[1].constructor_count(), 1u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].move_constructor_count(), 0u);
            hud_assert_eq(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, 1);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);
            hud_assert_eq(&array[0], &first_element);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(1, 2);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);
            hud_assert_eq(&array[0], &first_element);
            hud_assert_eq(array[1].id(), 2);
            hud_assert_eq(array[1].constructor_count(), 1u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].move_constructor_count(), 0u);
            hud_assert_eq(&array[1], &second_element);
        }
    }

    // Emplace at the middle of an array
    {
        // Reallocation
        {
            // The array with 4 elements with ID [1,2,4,5]
            array_type array;
            array.reserve(4);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(4);
            array.emplace_back(5);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 4u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u, 3);

            // Ensure we correctly emplaced the element in the middle.
            // Array should be [1,2,3,4,5]
            // All elements are should be moved except the inserted in the middle
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 1u);
            hud_assert_eq(array[1].id(), 2);
            hud_assert_eq(array[1].constructor_count(), 1u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].move_constructor_count(), 1u);
            hud_assert_eq(array[2].id(), 3);
            hud_assert_eq(array[2].constructor_count(), 1u);
            hud_assert_eq(array[2].copy_constructor_count(), 0u);
            hud_assert_eq(array[2].move_constructor_count(), 0u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3].id(), 4);
            hud_assert_eq(array[3].constructor_count(), 1u);
            hud_assert_eq(array[3].copy_constructor_count(), 0u);
            hud_assert_eq(array[3].move_constructor_count(), 1u);
            hud_assert_eq(array[4].id(), 5);
            hud_assert_eq(array[4].constructor_count(), 1u);
            hud_assert_eq(array[4].copy_constructor_count(), 0u);
            hud_assert_eq(array[4].move_constructor_count(), 1u);
        }

        // No reallocation
        {
            // The array with 4 elements with ID [1,2,4,5]
            array_type array;
            array.reserve(5);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(4);
            array.emplace_back(5);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace element in the middle
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u, 3);

            // Ensure we correctly emplaced the element.
            // Array should be [1,2,3,4,5]
            // Only 4 and 5 are copied
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);
            hud_assert_eq(array[1].id(), 2);
            hud_assert_eq(array[1].constructor_count(), 1u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].move_constructor_count(), 0u);
            hud_assert_eq(array[2].id(), 3);
            hud_assert_eq(array[2].constructor_count(), 1u);
            hud_assert_eq(array[2].copy_constructor_count(), 0u);
            hud_assert_eq(array[2].move_constructor_count(), 0u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3].id(), 4);
            hud_assert_eq(array[3].constructor_count(), 1u);
            hud_assert_eq(array[3].copy_constructor_count(), 0u);
            hud_assert_eq(array[3].move_constructor_count(), 1u);
            hud_assert_eq(array[4].id(), 5);
            hud_assert_eq(array[4].constructor_count(), 1u);
            hud_assert_eq(array[4].copy_constructor_count(), 0u);
            hud_assert_eq(array[4].move_constructor_count(), 1u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, 1);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);
            hud_assert_eq(&array[0], &first_element);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0, 2);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0].id(), 2);
            hud_assert_eq(array[0].constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[1].id(), 1);
            hud_assert_eq(array[1].constructor_count(), 1u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].move_constructor_count(), 1u);
        }

        // No reallocation
        {
            // The empty array
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, 1);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);
            hud_assert_eq(&array[0], &first_element);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0, 2);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), 2);
            hud_assert_eq(array[0].constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[0].move_constructor_count(), 0u);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[1].id(), 1);
            hud_assert_eq(array[1].constructor_count(), 1u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].move_constructor_count(), 1u);
            hud_assert_ne(&array[1], &first_element);
            hud_assert_eq(&first_element, &second_element);
        }
    }
}

GTEST_TEST(array, emplace_at_to_ref_can_construct_trivially_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_trivially_constructible_v<type, usize>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(array.count(), 1u);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(&array[0], &first_element);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(array.count(), 2u);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(array[1], 2u);
            hud_assert_eq(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, 1u);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 1u);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(1, 2u);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(&array[0], &first_element);
            hud_assert_eq(array[1], 2u);
            hud_assert_eq(&array[1], &second_element);
        }
    }

    // Emplace at the middle of an array
    {
        // Reallocation
        {
            // The array with 4 elements with ID [1,2,4,5]
            array_type array;
            array.reserve(4);
            array.emplace_back(1u);
            array.emplace_back(2u);
            array.emplace_back(4u);
            array.emplace_back(5u);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 4u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u, 3u);

            // Ensure we correctly emplaced the element in the middle.
            // Array should be [1,2,3,4,5]
            // All elements are should be moved except the inserted in the middle
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(array[1], 2u);
            hud_assert_eq(array[2], 3u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3], 4u);
            hud_assert_eq(array[4], 5u);
        }

        // No reallocation
        {
            // The array with 4 elements with ID [1,2,4,5]
            array_type array;
            array.reserve(5);
            array.emplace_back(1u);
            array.emplace_back(2u);
            array.emplace_back(4u);
            array.emplace_back(5u);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace element in the middle
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u, 3u);

            // Ensure we correctly emplaced the element.
            // Array should be [1,2,3,4,5]
            // Only 4 and 5 are copied
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(array[1], 2u);
            hud_assert_eq(array[2], 3u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3], 4u);
            hud_assert_eq(array[4], 5u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, 1u);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(&array[0], &first_element);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0, 2u);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0], 2u);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[1], 1u);
        }

        // No reallocation
        {
            // The empty array
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, 1u);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(&array[0], &first_element);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0, 2u);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 2u);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[1], 1u);
            hud_assert_ne(&array[1], &first_element);
            hud_assert_eq(&first_element, &second_element);
        }
    }
}

GTEST_TEST(array, emplace_at_to_ref_can_copy_construct_non_bitwise_copy_constructible_type)
{

    using type = hud_test::NonBitwiseCopyConstructibleType;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_bitwise_copy_constructible_v<type, type>);
    static_assert(hud::is_copy_constructible_v<type, type>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);
            hud_assert_eq(first_element_to_copy.copy_constructor_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(array.count(), first_element_to_copy);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), first_element_to_copy.id());
            hud_assert_eq(array[0].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 1u);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);
            hud_assert_eq(second_element_to_copy.copy_constructor_count(), 0u);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(array.count(), second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0].id(), first_element_to_copy.id());
            hud_assert_eq(array[0].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 2u);
            hud_assert_eq(array[1].id(), second_element_to_copy.id());
            hud_assert_eq(array[1].copy_constructor_count(), second_element_to_copy.copy_constructor_count() + 1u);
            hud_assert_eq(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);
            hud_assert_eq(first_element_to_copy.copy_constructor_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), first_element_to_copy.id());
            hud_assert_eq(array[0].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 1u);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);
            hud_assert_eq(second_element_to_copy.copy_constructor_count(), 0u);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(1, second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), first_element_to_copy.id());
            hud_assert_eq(array[0].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 1u);
            hud_assert_eq(&array[0], &first_element);
            hud_assert_eq(array[1].id(), second_element_to_copy.id());
            hud_assert_eq(array[1].copy_constructor_count(), second_element_to_copy.copy_constructor_count() + 1u);
            hud_assert_eq(&array[1], &second_element);
        }
    }

    // Emplace at the middle of an array
    {
        // Reallocation
        {
            // The array with 4 elements with ID [1,2,4,5]
            array_type array;
            array.reserve(4);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(4);
            array.emplace_back(5);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 4u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The element to emplace by copy construct with ID = 1
            const type first_element_to_copy(3);
            hud_assert_eq(first_element_to_copy.copy_constructor_count(), 0u);

            // Emplace first element
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].copy_constructor_count(), 1u);
            hud_assert_eq(array[1].id(), 2);
            hud_assert_eq(array[1].copy_constructor_count(), 1u);
            hud_assert_eq(array[2].id(), 3);
            hud_assert_eq(array[2].copy_constructor_count(), 1u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3].id(), 4);
            hud_assert_eq(array[3].copy_constructor_count(), 1u);
            hud_assert_eq(array[4].id(), 5);
            hud_assert_eq(array[4].copy_constructor_count(), 1u);
        }

        // No reallocation
        {
            // The array with 4 elements with ID [1,2,4,5]
            array_type array;
            array.reserve(5);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(4);
            array.emplace_back(5);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The element to emplace by copy construct with ID = 1
            const type first_element_to_copy(3);
            hud_assert_eq(first_element_to_copy.copy_constructor_count(), 0u);

            // Emplace element in the middle
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u, first_element_to_copy);

            // Ensure we correctly emplaced the element. Array should be [1,2,3,4,5]
            // Only 4 and 5 are copied
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].id(), 2);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[2].id(), 3);
            hud_assert_eq(array[2].copy_constructor_count(), 1u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3].id(), 4);
            hud_assert_eq(array[3].copy_constructor_count(), 1u);
            hud_assert_eq(array[4].id(), 5);
            hud_assert_eq(array[4].copy_constructor_count(), 1u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);
            hud_assert_eq(first_element_to_copy.copy_constructor_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), first_element_to_copy.id());
            hud_assert_eq(array[0].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 1u);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);
            hud_assert_eq(second_element_to_copy.copy_constructor_count(), 0u);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0, second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0].id(), second_element_to_copy.id());
            hud_assert_eq(array[0].copy_constructor_count(), second_element_to_copy.copy_constructor_count() + 1u);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[1].id(), first_element_to_copy.id());
            hud_assert_eq(array[1].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 2u);
        }

        // No reallocation
        {
            // The empty array
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);
            hud_assert_eq(first_element_to_copy.copy_constructor_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), first_element_to_copy.id());
            hud_assert_eq(array[0].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 1u);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);
            hud_assert_eq(second_element_to_copy.copy_constructor_count(), 0u);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0, second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), second_element_to_copy.id());
            hud_assert_eq(array[0].copy_constructor_count(), second_element_to_copy.copy_constructor_count() + 1u);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[1].id(), first_element_to_copy.id());
            hud_assert_eq(array[1].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 2u);
            hud_assert_ne(&array[1], &first_element);
            hud_assert_eq(&first_element, &second_element);
        }
    }
}

GTEST_TEST(array, emplace_at_to_ref_can_copy_construct_bitwise_copy_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_bitwise_copy_constructible_v<type, type>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(array.count(), first_element_to_copy);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], first_element_to_copy);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(array.count(), second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0], first_element_to_copy);
            hud_assert_eq(array[1], second_element_to_copy);
            hud_assert_eq(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], first_element_to_copy);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(1, second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], first_element_to_copy);
            hud_assert_eq(&array[0], &first_element);
            hud_assert_eq(array[1], second_element_to_copy);
            hud_assert_eq(&array[1], &second_element);
        }
    }

    // Emplace at the middle of an array
    {
        // Reallocation
        {
            // The array with 4 elements with ID [1,2,4,5]
            array_type array;
            array.reserve(4);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(4);
            array.emplace_back(5);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 4u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The element to emplace by copy construct with ID = 1
            const type first_element_to_copy(3);

            // Emplace first element
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(array[1], 2u);
            hud_assert_eq(array[2], 3u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3], 4u);
            hud_assert_eq(array[4], 5u);
        }

        // No reallocation
        {
            // The array with 4 elements with ID [1,2,4,5]
            array_type array;
            array.reserve(5);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(4);
            array.emplace_back(5);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The element to emplace by copy construct with ID = 1
            const type first_element_to_copy(3);

            // Emplace element in the middle
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u, first_element_to_copy);

            // Ensure we correctly emplaced the element. Array should be [1,2,3,4,5]
            // Only 4 and 5 are copied
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(array[1], 2u);
            hud_assert_eq(array[2], 3u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3], 4u);
            hud_assert_eq(array[4], 5u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], first_element_to_copy);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0, second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0], second_element_to_copy);
            hud_assert_eq(array[1], first_element_to_copy);
            hud_assert_eq(&array[0], &second_element);
        }

        // No reallocation
        {
            // The empty array
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], first_element_to_copy);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0, second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], second_element_to_copy);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[1], first_element_to_copy);
            hud_assert_ne(&array[1], &first_element);
            hud_assert_eq(&first_element, &second_element);
        }
    }
}

GTEST_TEST(array, emplace_at_to_ref_can_move_construct_non_bitwise_move_constructible_type)
{

    using type = hud_test::NonBitwiseMoveConstructibleType;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_bitwise_move_constructible_v<type>);
    static_assert(hud::is_move_constructible_v<type>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);
            hud_assert_eq(first_element_to_move.copy_constructor_count(), 0u);
            hud_assert_eq(first_element_to_move.move_constructor_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(array.count(), hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), first_element_to_move.id());
            hud_assert_eq(array[0].move_constructor_count(), first_element_to_move.move_constructor_count() + 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);
            hud_assert_eq(second_element_to_move.copy_constructor_count(), 0u);
            hud_assert_eq(second_element_to_move.move_constructor_count(), 0u);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(array.count(), hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0].id(), first_element_to_move.id());
            hud_assert_eq(array[0].move_constructor_count(), first_element_to_move.move_constructor_count() + 2u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].id(), second_element_to_move.id());
            hud_assert_eq(array[1].move_constructor_count(), second_element_to_move.move_constructor_count() + 1u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);
            hud_assert_eq(first_element_to_move.copy_constructor_count(), 0u);
            hud_assert_eq(first_element_to_move.move_constructor_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), first_element_to_move.id());
            hud_assert_eq(array[0].move_constructor_count(), first_element_to_move.move_constructor_count() + 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);
            hud_assert_eq(second_element_to_move.copy_constructor_count(), 0u);
            hud_assert_eq(second_element_to_move.move_constructor_count(), 0u);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(1, hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), first_element_to_move.id());
            hud_assert_eq(array[0].move_constructor_count(), first_element_to_move.move_constructor_count() + 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(&array[0], &first_element);
            hud_assert_eq(array[1].id(), second_element_to_move.id());
            hud_assert_eq(array[1].move_constructor_count(), second_element_to_move.move_constructor_count() + 1u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(&array[1], &second_element);
        }
    }

    // Emplace at the middle of an array
    {
        // Reallocation
        {
            // The array with 4 elements with ID [1,2,4,5]
            array_type array;
            array.reserve(4);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(4);
            array.emplace_back(5);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 4u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The element to emplace by move construct with ID = 1
            type first_element_to_move(3);
            hud_assert_eq(first_element_to_move.copy_constructor_count(), 0u);
            hud_assert_eq(first_element_to_move.move_constructor_count(), 0u);

            // Emplace first element
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].move_constructor_count(), 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].id(), 2);
            hud_assert_eq(array[1].move_constructor_count(), 1u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[2].id(), 3);
            hud_assert_eq(array[2].move_constructor_count(), 1u);
            hud_assert_eq(array[2].copy_constructor_count(), 0u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3].id(), 4);
            hud_assert_eq(array[3].move_constructor_count(), 1u);
            hud_assert_eq(array[3].copy_constructor_count(), 0u);
            hud_assert_eq(array[4].id(), 5);
            hud_assert_eq(array[4].move_constructor_count(), 1u);
            hud_assert_eq(array[4].copy_constructor_count(), 0u);
        }

        // No reallocation
        {
            // The array with 4 elements with ID [1,2,4,5]
            array_type array;
            array.reserve(5);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(4);
            array.emplace_back(5);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The element to emplace by move construct with ID = 1
            type first_element_to_move(3);
            hud_assert_eq(first_element_to_move.copy_constructor_count(), 0u);
            hud_assert_eq(first_element_to_move.move_constructor_count(), 0u);

            // Emplace element in the middle
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the element. Array should be [1,2,3,4,5]
            // Only 4 and 5 are copied
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), 1);
            hud_assert_eq(array[0].move_constructor_count(), 0u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(array[1].id(), 2);
            hud_assert_eq(array[1].move_constructor_count(), 0u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_eq(array[2].id(), 3);
            hud_assert_eq(array[2].move_constructor_count(), 1u);
            hud_assert_eq(array[2].copy_constructor_count(), 0u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3].id(), 4);
            hud_assert_eq(array[3].move_constructor_count(), 1u);
            hud_assert_eq(array[3].copy_constructor_count(), 0u);
            hud_assert_eq(array[4].id(), 5);
            hud_assert_eq(array[4].move_constructor_count(), 1u);
            hud_assert_eq(array[4].copy_constructor_count(), 0u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);
            hud_assert_eq(first_element_to_move.copy_constructor_count(), 0u);
            hud_assert_eq(first_element_to_move.move_constructor_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), first_element_to_move.id());
            hud_assert_eq(array[0].move_constructor_count(), first_element_to_move.move_constructor_count() + 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);
            hud_assert_eq(second_element_to_move.move_constructor_count(), 0u);
            hud_assert_eq(second_element_to_move.copy_constructor_count(), 0u);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0, hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0].id(), second_element_to_move.id());
            hud_assert_eq(array[0].move_constructor_count(), second_element_to_move.move_constructor_count() + 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[1].id(), first_element_to_move.id());
            hud_assert_eq(array[1].move_constructor_count(), first_element_to_move.move_constructor_count() + 2u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
        }

        // No reallocation
        {
            // The empty array
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);
            hud_assert_eq(first_element_to_move.copy_constructor_count(), 0u);
            hud_assert_eq(first_element_to_move.move_constructor_count(), 0u);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), first_element_to_move.id());
            hud_assert_eq(array[0].move_constructor_count(), first_element_to_move.move_constructor_count() + 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);
            hud_assert_eq(second_element_to_move.move_constructor_count(), 0u);
            hud_assert_eq(second_element_to_move.copy_constructor_count(), 0u);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0, hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0].id(), second_element_to_move.id());
            hud_assert_eq(array[0].move_constructor_count(), second_element_to_move.move_constructor_count() + 1u);
            hud_assert_eq(array[0].copy_constructor_count(), 0u);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[1].id(), first_element_to_move.id());
            hud_assert_eq(array[1].move_constructor_count(), first_element_to_move.move_constructor_count() + 2u);
            hud_assert_eq(array[1].copy_constructor_count(), 0u);
            hud_assert_ne(&array[1], &first_element);
            hud_assert_eq(&first_element, &second_element);
        }
    }
}

GTEST_TEST(array, emplace_at_to_ref_can_move_construct_bitwise_move_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_bitwise_move_constructible_v<type>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(array.count(), hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], first_element_to_move);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(array.count(), hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0], first_element_to_move);
            hud_assert_eq(array[1], second_element_to_move);
            hud_assert_eq(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], first_element_to_move);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(1, hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], first_element_to_move);
            hud_assert_eq(&array[0], &first_element);
            hud_assert_eq(array[1], second_element_to_move);
            hud_assert_eq(&array[1], &second_element);
        }
    }

    // Emplace at the middle of an array
    {
        // Reallocation
        {
            // The array with 4 elements with ID [1,2,4,5]
            array_type array;
            array.reserve(4);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(4);
            array.emplace_back(5);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 4u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The element to emplace by move construct with ID = 1
            type first_element_to_move(3);

            // Emplace first element
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(array[1], 2u);
            hud_assert_eq(array[2], 3u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3], 4u);
            hud_assert_eq(array[4], 5u);
        }

        // No reallocation
        {
            // The array with 4 elements with ID [1,2,4,5]
            array_type array;
            array.reserve(5);
            array.emplace_back(1);
            array.emplace_back(2);
            array.emplace_back(4);
            array.emplace_back(5);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 4u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The element to emplace by move construct with ID = 1
            type first_element_to_move(3);

            // Emplace element in the middle
            const type &mid_element = array.emplace_at_to_ref(array.count() / 2u, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the element. Array should be [1,2,3,4,5]
            // Only 4 and 5 are copied
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 5u);
            hud_assert_eq(array.max_count(), 5u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], 1u);
            hud_assert_eq(array[1], 2u);
            hud_assert_eq(array[2], 3u);
            hud_assert_eq(&array[2], &mid_element);
            hud_assert_eq(array[3], 4u);
            hud_assert_eq(array[4], 5u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array
            array_type array;
            hud_assert_eq(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 0u);
            hud_assert_eq(array.allocator().allocation_count(), 0u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 1u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], first_element_to_move);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0, hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 2u);
            hud_assert_eq(array.allocator().free_count(), 1u);
            hud_assert_eq(array[0], second_element_to_move);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[1], first_element_to_move);
        }

        // No reallocation
        {
            // The empty array
            array_type array;
            array.reserve(2);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 0u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);

            // Emplace first element
            const type &first_element = array.emplace_at_to_ref(0, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 1u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], first_element_to_move);
            hud_assert_eq(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);

            // Emplace second element
            const type &second_element = array.emplace_at_to_ref(0, hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), 2u);
            hud_assert_eq(array.max_count(), 2u);
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
            hud_assert_eq(array[0], second_element_to_move);
            hud_assert_eq(&array[0], &second_element);
            hud_assert_eq(array[1], first_element_to_move);
            hud_assert_ne(&array[1], &first_element);
            hud_assert_eq(&first_element, &second_element);
        }
    }
}
