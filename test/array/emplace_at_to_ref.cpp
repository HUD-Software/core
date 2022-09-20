#include <core/containers/array.h>
#include "allocators.h"

TEST(Array, emplace_at_to_ref_can_default_construct_non_trivially_default_constructible_type)
{

    using type = hud::test::default_constructible_type;
    using array_type = hud::array<type, hud::test::array_allocator<alignof(type)>>;
    static_assert(!hud::is_trivially_default_constructible_v<type>);
    static_assert(hud::is_default_constructible_v<type>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(array.count());

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);
            ASSERT_EQ(array[0].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[0].default_constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(array.count());

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[0].default_constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 1u);
            ASSERT_EQ(&array[1], &second_element);
            ASSERT_EQ(array[1].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[1].default_constructor_count(), 1u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].move_constructor_count(), 0u);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);
            ASSERT_EQ(array[0].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[0].default_constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(1);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);
            ASSERT_EQ(array[0].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[0].default_constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);
            ASSERT_EQ(&array[1], &second_element);
            ASSERT_EQ(array[1].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[1].default_constructor_count(), 1u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].move_constructor_count(), 0u);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 4u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& element = array.emplace_at_to_ref(array.count() / 2u);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].default_constructor_count(), 0u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 1u);
            ASSERT_EQ(array[1].id(), 2);
            ASSERT_EQ(array[1].default_constructor_count(), 0u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].move_constructor_count(), 1u);
            ASSERT_EQ(&array[2], &element);
            ASSERT_EQ(array[2].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[2].default_constructor_count(), 1u);
            ASSERT_EQ(array[2].copy_constructor_count(), 0u);
            ASSERT_EQ(array[2].move_constructor_count(), 0u);
            ASSERT_EQ(array[3].id(), 3);
            ASSERT_EQ(array[3].default_constructor_count(), 0u);
            ASSERT_EQ(array[3].copy_constructor_count(), 0u);
            ASSERT_EQ(array[3].move_constructor_count(), 1u);
            ASSERT_EQ(array[4].id(), 4);
            ASSERT_EQ(array[4].default_constructor_count(), 0u);
            ASSERT_EQ(array[4].copy_constructor_count(), 0u);
            ASSERT_EQ(array[4].move_constructor_count(), 1u);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace element in the middle
            const type& element = array.emplace_at_to_ref(array.count() / 2u);

            // Ensure we correctly emplaced the element. 
            // Array should be [1,2,type::DEFAULT_ID_VALUE,3,4]
            // Only 4 and 5 are copied 
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].default_constructor_count(), 0u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);
            ASSERT_EQ(array[1].id(), 2);
            ASSERT_EQ(array[1].default_constructor_count(), 0u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].move_constructor_count(), 0u);
            ASSERT_EQ(&array[2], &element);
            ASSERT_EQ(array[2].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[2].default_constructor_count(), 1u);
            ASSERT_EQ(array[2].copy_constructor_count(), 0u);
            ASSERT_EQ(array[2].move_constructor_count(), 0u);
            ASSERT_EQ(array[3].id(), 3);
            ASSERT_EQ(array[3].default_constructor_count(), 0u);
            ASSERT_EQ(array[3].copy_constructor_count(), 0u);
            ASSERT_EQ(array[3].move_constructor_count(), 1u);
            ASSERT_EQ(array[4].id(), 4);
            ASSERT_EQ(array[4].default_constructor_count(), 0u);
            ASSERT_EQ(array[4].copy_constructor_count(), 0u);
            ASSERT_EQ(array[4].move_constructor_count(), 1u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);
            ASSERT_EQ(array[0].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[0].default_constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[0].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[0].default_constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);
            ASSERT_EQ(array[1].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[1].default_constructor_count(), 1u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].move_constructor_count(), 1u);
        }

        // No reallocation
        {
            // The empty array 
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);
            ASSERT_EQ(array[0].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[0].default_constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[0].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[0].default_constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);
            ASSERT_EQ(array[1].id(), type::DEFAULT_ID_VALUE);
            ASSERT_EQ(array[1].default_constructor_count(), 1u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].move_constructor_count(), 1u);
            ASSERT_NE(&array[1], &first_element);
            ASSERT_EQ(&first_element, &second_element);
        }
    }
}

TEST(Array, emplace_at_to_ref_can_default_construct_trivially_default_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud::test::array_allocator<alignof(type)>>;
    static_assert(hud::is_trivially_default_constructible_v<type>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(array.count());

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);
            ASSERT_EQ(array[0], 0u);

            // Change the value to differanciate it from default value
            array[0] = 1u;

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(array.count());

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(array[1], 0u);
            ASSERT_EQ(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 0u);
            ASSERT_EQ(&array[0], &first_element);

            // Change the value to differanciate it from default value
            array[0] = 1u;

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(1);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(&array[0], &first_element);
            ASSERT_EQ(array[1], 0u);
            ASSERT_EQ(&array[1], &second_element);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 4u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(array[1], 2u);
            ASSERT_EQ(array[2], 0u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3], 3u);
            ASSERT_EQ(array[4], 4u);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace element in the middle
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u);

            // Ensure we correctly emplaced the element. 
            // Array should be [1,2,0,3,4]
            // Only 4 and 5 are copied 
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(array[1], 2u);
            ASSERT_EQ(array[2], 0u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3], 3u);
            ASSERT_EQ(array[4], 4u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 0u);
            ASSERT_EQ(&array[0], &first_element);

            // Change the value to differanciate it from default value
            array[0] = 1u;

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0], 0u);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[1], 1u);
        }

        // No reallocation
        {
            // The empty array 
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 0u);
            ASSERT_EQ(&array[0], &first_element);

            // Change the value to differanciate it from default value
            array[0] = 1u;

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 0u);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[1], 1u);
            ASSERT_NE(&array[1], &first_element);
            ASSERT_EQ(&first_element, &second_element);
        }
    }
}

TEST(Array, emplace_at_to_ref_can_construct_non_trivially_constructible_type)
{

    using type = hud::test::NonDefaultConstructibleType;
    using array_type = hud::array<type, hud::test::array_allocator<alignof(type)>>;
    static_assert(!hud::is_trivially_constructible_v<type, usize>);
    static_assert(hud::is_constructible_v<type, usize>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(array.count(), 1);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(array.count(), 2);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 1u);
            ASSERT_EQ(array[1].id(), 2);
            ASSERT_EQ(array[1].constructor_count(), 1u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].move_constructor_count(), 0u);
            ASSERT_EQ(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, 1);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(1, 2);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);
            ASSERT_EQ(array[1].id(), 2);
            ASSERT_EQ(array[1].constructor_count(), 1u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].move_constructor_count(), 0u);
            ASSERT_EQ(&array[1], &second_element);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 4u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u, 3);

            // Ensure we correctly emplaced the element in the middle.
            // Array should be [1,2,3,4,5]
            // All elements are should be moved except the inserted in the middle
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 1u);
            ASSERT_EQ(array[1].id(), 2);
            ASSERT_EQ(array[1].constructor_count(), 1u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].move_constructor_count(), 1u);
            ASSERT_EQ(array[2].id(), 3);
            ASSERT_EQ(array[2].constructor_count(), 1u);
            ASSERT_EQ(array[2].copy_constructor_count(), 0u);
            ASSERT_EQ(array[2].move_constructor_count(), 0u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3].id(), 4);
            ASSERT_EQ(array[3].constructor_count(), 1u);
            ASSERT_EQ(array[3].copy_constructor_count(), 0u);
            ASSERT_EQ(array[3].move_constructor_count(), 1u);
            ASSERT_EQ(array[4].id(), 5);
            ASSERT_EQ(array[4].constructor_count(), 1u);
            ASSERT_EQ(array[4].copy_constructor_count(), 0u);
            ASSERT_EQ(array[4].move_constructor_count(), 1u);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace element in the middle
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u, 3);

            // Ensure we correctly emplaced the element. 
            // Array should be [1,2,3,4,5]
            // Only 4 and 5 are copied 
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);
            ASSERT_EQ(array[1].id(), 2);
            ASSERT_EQ(array[1].constructor_count(), 1u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].move_constructor_count(), 0u);
            ASSERT_EQ(array[2].id(), 3);
            ASSERT_EQ(array[2].constructor_count(), 1u);
            ASSERT_EQ(array[2].copy_constructor_count(), 0u);
            ASSERT_EQ(array[2].move_constructor_count(), 0u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3].id(), 4);
            ASSERT_EQ(array[3].constructor_count(), 1u);
            ASSERT_EQ(array[3].copy_constructor_count(), 0u);
            ASSERT_EQ(array[3].move_constructor_count(), 1u);
            ASSERT_EQ(array[4].id(), 5);
            ASSERT_EQ(array[4].constructor_count(), 1u);
            ASSERT_EQ(array[4].copy_constructor_count(), 0u);
            ASSERT_EQ(array[4].move_constructor_count(), 1u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, 1);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0, 2);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0].id(), 2);
            ASSERT_EQ(array[0].constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[1].id(), 1);
            ASSERT_EQ(array[1].constructor_count(), 1u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].move_constructor_count(), 1u);
        }

        // No reallocation
        {
            // The empty array 
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, 1);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0, 2);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), 2);
            ASSERT_EQ(array[0].constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[1].id(), 1);
            ASSERT_EQ(array[1].constructor_count(), 1u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].move_constructor_count(), 1u);
            ASSERT_NE(&array[1], &first_element);
            ASSERT_EQ(&first_element, &second_element);
        }
    }
}

TEST(Array, emplace_at_to_ref_can_construct_trivially_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud::test::array_allocator<alignof(type)>>;
    static_assert(hud::is_trivially_constructible_v<type, usize>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(array.count(), 1u);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(&array[0], &first_element);


            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(array.count(), 2u);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(array[1], 2u);
            ASSERT_EQ(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, 1u);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 1u);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(1, 2u);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(&array[0], &first_element);
            ASSERT_EQ(array[1], 2u);
            ASSERT_EQ(&array[1], &second_element);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 4u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u, 3u);

            // Ensure we correctly emplaced the element in the middle.
            // Array should be [1,2,3,4,5]
            // All elements are should be moved except the inserted in the middle
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(array[1], 2u);
            ASSERT_EQ(array[2], 3u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3], 4u);
            ASSERT_EQ(array[4], 5u);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace element in the middle
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u, 3u);

            // Ensure we correctly emplaced the element. 
            // Array should be [1,2,3,4,5]
            // Only 4 and 5 are copied 
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(array[1], 2u);
            ASSERT_EQ(array[2], 3u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3], 4u);
            ASSERT_EQ(array[4], 5u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, 1u);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(&array[0], &first_element);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0, 2u);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0], 2u);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[1], 1u);
        }

        // No reallocation
        {
            // The empty array 
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, 1u);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(&array[0], &first_element);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0, 2u);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 2u);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[1], 1u);
            ASSERT_NE(&array[1], &first_element);
            ASSERT_EQ(&first_element, &second_element);
        }
    }
}

TEST(Array, emplace_at_to_ref_can_copy_construct_non_bitwise_copy_constructible_type)
{

    using type = hud::test::NonBitwiseCopyConstructibleType;
    using array_type = hud::array<type, hud::test::array_allocator<alignof(type)>>;
    static_assert(!hud::is_bitwise_copy_constructible_v<type, type>);
    static_assert(hud::is_copy_constructible_v<type, type>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);
            ASSERT_EQ(first_element_to_copy.copy_constructor_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(array.count(), first_element_to_copy);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), first_element_to_copy.id());
            ASSERT_EQ(array[0].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 1u);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);
            ASSERT_EQ(second_element_to_copy.copy_constructor_count(), 0u);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(array.count(), second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0].id(), first_element_to_copy.id());
            ASSERT_EQ(array[0].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 2u);
            ASSERT_EQ(array[1].id(), second_element_to_copy.id());
            ASSERT_EQ(array[1].copy_constructor_count(), second_element_to_copy.copy_constructor_count() + 1u);
            ASSERT_EQ(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);
            ASSERT_EQ(first_element_to_copy.copy_constructor_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), first_element_to_copy.id());
            ASSERT_EQ(array[0].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 1u);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);
            ASSERT_EQ(second_element_to_copy.copy_constructor_count(), 0u);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(1, second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), first_element_to_copy.id());
            ASSERT_EQ(array[0].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 1u);
            ASSERT_EQ(&array[0], &first_element);
            ASSERT_EQ(array[1].id(), second_element_to_copy.id());
            ASSERT_EQ(array[1].copy_constructor_count(), second_element_to_copy.copy_constructor_count() + 1u);
            ASSERT_EQ(&array[1], &second_element);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 4u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The element to emplace by copy construct with ID = 1
            const type first_element_to_copy(3);
            ASSERT_EQ(first_element_to_copy.copy_constructor_count(), 0u);

            // Emplace first element
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].copy_constructor_count(), 1u);
            ASSERT_EQ(array[1].id(), 2);
            ASSERT_EQ(array[1].copy_constructor_count(), 1u);
            ASSERT_EQ(array[2].id(), 3);
            ASSERT_EQ(array[2].copy_constructor_count(), 1u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3].id(), 4);
            ASSERT_EQ(array[3].copy_constructor_count(), 1u);
            ASSERT_EQ(array[4].id(), 5);
            ASSERT_EQ(array[4].copy_constructor_count(), 1u);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The element to emplace by copy construct with ID = 1
            const type first_element_to_copy(3);
            ASSERT_EQ(first_element_to_copy.copy_constructor_count(), 0u);

            // Emplace element in the middle
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u, first_element_to_copy);

            // Ensure we correctly emplaced the element. Array should be [1,2,3,4,5]
            // Only 4 and 5 are copied 
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].id(), 2);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[2].id(), 3);
            ASSERT_EQ(array[2].copy_constructor_count(), 1u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3].id(), 4);
            ASSERT_EQ(array[3].copy_constructor_count(), 1u);
            ASSERT_EQ(array[4].id(), 5);
            ASSERT_EQ(array[4].copy_constructor_count(), 1u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);
            ASSERT_EQ(first_element_to_copy.copy_constructor_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), first_element_to_copy.id());
            ASSERT_EQ(array[0].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 1u);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);
            ASSERT_EQ(second_element_to_copy.copy_constructor_count(), 0u);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0, second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0].id(), second_element_to_copy.id());
            ASSERT_EQ(array[0].copy_constructor_count(), second_element_to_copy.copy_constructor_count() + 1u);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[1].id(), first_element_to_copy.id());
            ASSERT_EQ(array[1].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 2u);
        }

        // No reallocation
        {
            // The empty array 
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);
            ASSERT_EQ(first_element_to_copy.copy_constructor_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), first_element_to_copy.id());
            ASSERT_EQ(array[0].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 1u);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);
            ASSERT_EQ(second_element_to_copy.copy_constructor_count(), 0u);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0, second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), second_element_to_copy.id());
            ASSERT_EQ(array[0].copy_constructor_count(), second_element_to_copy.copy_constructor_count() + 1u);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[1].id(), first_element_to_copy.id());
            ASSERT_EQ(array[1].copy_constructor_count(), first_element_to_copy.copy_constructor_count() + 2u);
            ASSERT_NE(&array[1], &first_element);
            ASSERT_EQ(&first_element, &second_element);
        }
    }
}

TEST(Array, emplace_at_to_ref_can_copy_construct_bitwise_copy_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud::test::array_allocator<alignof(type)>>;
    static_assert(hud::is_bitwise_copy_constructible_v<type, type>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(array.count(), first_element_to_copy);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], first_element_to_copy);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(array.count(), second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0], first_element_to_copy);
            ASSERT_EQ(array[1], second_element_to_copy);
            ASSERT_EQ(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], first_element_to_copy);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(1, second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], first_element_to_copy);
            ASSERT_EQ(&array[0], &first_element);
            ASSERT_EQ(array[1], second_element_to_copy);
            ASSERT_EQ(&array[1], &second_element);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 4u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The element to emplace by copy construct with ID = 1
            const type first_element_to_copy(3);

            // Emplace first element
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(array[1], 2u);
            ASSERT_EQ(array[2], 3u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3], 4u);
            ASSERT_EQ(array[4], 5u);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The element to emplace by copy construct with ID = 1
            const type first_element_to_copy(3);

            // Emplace element in the middle
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u, first_element_to_copy);

            // Ensure we correctly emplaced the element. Array should be [1,2,3,4,5]
            // Only 4 and 5 are copied 
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(array[1], 2u);
            ASSERT_EQ(array[2], 3u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3], 4u);
            ASSERT_EQ(array[4], 5u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], first_element_to_copy);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0, second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0], second_element_to_copy);
            ASSERT_EQ(array[1], first_element_to_copy);
            ASSERT_EQ(&array[0], &second_element);
        }

        // No reallocation
        {
            // The empty array 
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by copy construct
            const type first_element_to_copy(0);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, first_element_to_copy);

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], first_element_to_copy);

            // The second element to emplace by copy construct
            const type second_element_to_copy(1);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0, second_element_to_copy);

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], second_element_to_copy);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[1], first_element_to_copy);
            ASSERT_NE(&array[1], &first_element);
            ASSERT_EQ(&first_element, &second_element);
        }
    }
}

TEST(Array, emplace_at_to_ref_can_move_construct_non_bitwise_move_constructible_type)
{

    using type = hud::test::NonBitwiseMoveConstructibleType;
    using array_type = hud::array<type, hud::test::array_allocator<alignof(type)>>;
    static_assert(!hud::is_bitwise_move_constructible_v<type>);
    static_assert(hud::is_move_constructible_v<type>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);
            ASSERT_EQ(first_element_to_move.copy_constructor_count(), 0u);
            ASSERT_EQ(first_element_to_move.move_constructor_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(array.count(), hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), first_element_to_move.id());
            ASSERT_EQ(array[0].move_constructor_count(), first_element_to_move.move_constructor_count() + 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);
            ASSERT_EQ(second_element_to_move.copy_constructor_count(), 0u);
            ASSERT_EQ(second_element_to_move.move_constructor_count(), 0u);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(array.count(), hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0].id(), first_element_to_move.id());
            ASSERT_EQ(array[0].move_constructor_count(), first_element_to_move.move_constructor_count() + 2u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].id(), second_element_to_move.id());
            ASSERT_EQ(array[1].move_constructor_count(), second_element_to_move.move_constructor_count() + 1u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);
            ASSERT_EQ(first_element_to_move.copy_constructor_count(), 0u);
            ASSERT_EQ(first_element_to_move.move_constructor_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), first_element_to_move.id());
            ASSERT_EQ(array[0].move_constructor_count(), first_element_to_move.move_constructor_count() + 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);
            ASSERT_EQ(second_element_to_move.copy_constructor_count(), 0u);
            ASSERT_EQ(second_element_to_move.move_constructor_count(), 0u);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(1, hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), first_element_to_move.id());
            ASSERT_EQ(array[0].move_constructor_count(), first_element_to_move.move_constructor_count() + 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);
            ASSERT_EQ(array[1].id(), second_element_to_move.id());
            ASSERT_EQ(array[1].move_constructor_count(), second_element_to_move.move_constructor_count() + 1u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(&array[1], &second_element);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 4u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The element to emplace by move construct with ID = 1
            type first_element_to_move(3);
            ASSERT_EQ(first_element_to_move.copy_constructor_count(), 0u);
            ASSERT_EQ(first_element_to_move.move_constructor_count(), 0u);

            // Emplace first element
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].move_constructor_count(), 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].id(), 2);
            ASSERT_EQ(array[1].move_constructor_count(), 1u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[2].id(), 3);
            ASSERT_EQ(array[2].move_constructor_count(), 1u);
            ASSERT_EQ(array[2].copy_constructor_count(), 0u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3].id(), 4);
            ASSERT_EQ(array[3].move_constructor_count(), 1u);
            ASSERT_EQ(array[3].copy_constructor_count(), 0u);
            ASSERT_EQ(array[4].id(), 5);
            ASSERT_EQ(array[4].move_constructor_count(), 1u);
            ASSERT_EQ(array[4].copy_constructor_count(), 0u);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The element to emplace by move construct with ID = 1
            type first_element_to_move(3);
            ASSERT_EQ(first_element_to_move.copy_constructor_count(), 0u);
            ASSERT_EQ(first_element_to_move.move_constructor_count(), 0u);

            // Emplace element in the middle
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the element. Array should be [1,2,3,4,5]
            // Only 4 and 5 are copied 
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), 1);
            ASSERT_EQ(array[0].move_constructor_count(), 0u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(array[1].id(), 2);
            ASSERT_EQ(array[1].move_constructor_count(), 0u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_EQ(array[2].id(), 3);
            ASSERT_EQ(array[2].move_constructor_count(), 1u);
            ASSERT_EQ(array[2].copy_constructor_count(), 0u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3].id(), 4);
            ASSERT_EQ(array[3].move_constructor_count(), 1u);
            ASSERT_EQ(array[3].copy_constructor_count(), 0u);
            ASSERT_EQ(array[4].id(), 5);
            ASSERT_EQ(array[4].move_constructor_count(), 1u);
            ASSERT_EQ(array[4].copy_constructor_count(), 0u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);
            ASSERT_EQ(first_element_to_move.copy_constructor_count(), 0u);
            ASSERT_EQ(first_element_to_move.move_constructor_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), first_element_to_move.id());
            ASSERT_EQ(array[0].move_constructor_count(), first_element_to_move.move_constructor_count() + 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);
            ASSERT_EQ(second_element_to_move.move_constructor_count(), 0u);
            ASSERT_EQ(second_element_to_move.copy_constructor_count(), 0u);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0, hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0].id(), second_element_to_move.id());
            ASSERT_EQ(array[0].move_constructor_count(), second_element_to_move.move_constructor_count() + 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[1].id(), first_element_to_move.id());
            ASSERT_EQ(array[1].move_constructor_count(), first_element_to_move.move_constructor_count() + 2u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
        }

        // No reallocation
        {
            // The empty array 
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);
            ASSERT_EQ(first_element_to_move.copy_constructor_count(), 0u);
            ASSERT_EQ(first_element_to_move.move_constructor_count(), 0u);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), first_element_to_move.id());
            ASSERT_EQ(array[0].move_constructor_count(), first_element_to_move.move_constructor_count() + 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);
            ASSERT_EQ(second_element_to_move.move_constructor_count(), 0u);
            ASSERT_EQ(second_element_to_move.copy_constructor_count(), 0u);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0, hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0].id(), second_element_to_move.id());
            ASSERT_EQ(array[0].move_constructor_count(), second_element_to_move.move_constructor_count() + 1u);
            ASSERT_EQ(array[0].copy_constructor_count(), 0u);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[1].id(), first_element_to_move.id());
            ASSERT_EQ(array[1].move_constructor_count(), first_element_to_move.move_constructor_count() + 2u);
            ASSERT_EQ(array[1].copy_constructor_count(), 0u);
            ASSERT_NE(&array[1], &first_element);
            ASSERT_EQ(&first_element, &second_element);
        }
    }
}

TEST(Array, emplace_at_to_ref_can_move_construct_bitwise_move_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud::test::array_allocator<alignof(type)>>;
    static_assert(hud::is_bitwise_move_constructible_v<type>);

    // Emplace at the end of empty array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(array.count(), hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], first_element_to_move);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(array.count(), hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0], first_element_to_move);
            ASSERT_EQ(array[1], second_element_to_move);
            ASSERT_EQ(&array[1], &second_element);
        }

        // No reallocation
        {
            // The empty array but with memory reserved
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], first_element_to_move);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(1, hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], first_element_to_move);
            ASSERT_EQ(&array[0], &first_element);
            ASSERT_EQ(array[1], second_element_to_move);
            ASSERT_EQ(&array[1], &second_element);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 4u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The element to emplace by move construct with ID = 1
            type first_element_to_move(3);

            // Emplace first element
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(array[1], 2u);
            ASSERT_EQ(array[2], 3u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3], 4u);
            ASSERT_EQ(array[4], 5u);
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
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 4u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The element to emplace by move construct with ID = 1
            type first_element_to_move(3);

            // Emplace element in the middle
            const type& mid_element = array.emplace_at_to_ref(array.count() / 2u, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the element. Array should be [1,2,3,4,5]
            // Only 4 and 5 are copied 
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 5u);
            ASSERT_EQ(array.max_count(), 5u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], 1u);
            ASSERT_EQ(array[1], 2u);
            ASSERT_EQ(array[2], 3u);
            ASSERT_EQ(&array[2], &mid_element);
            ASSERT_EQ(array[3], 4u);
            ASSERT_EQ(array[4], 5u);
        }
    }

    // Emplace at the begin of an array
    {
        // Reallocation
        {
            // The empty array 
            array_type array;
            ASSERT_EQ(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 0u);
            ASSERT_EQ(array.allocator().allocation_count(), 0u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 1u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], first_element_to_move);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0, hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 2u);
            ASSERT_EQ(array.allocator().free_count(), 1u);
            ASSERT_EQ(array[0], second_element_to_move);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[1], first_element_to_move);
        }

        // No reallocation
        {
            // The empty array 
            array_type array;
            array.reserve(2);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 0u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);

            // The first element to emplace by move construct
            type first_element_to_move(0);

            // Emplace first element
            const type& first_element = array.emplace_at_to_ref(0, hud::move(first_element_to_move));

            // Ensure we correctly emplaced the first element
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 1u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], first_element_to_move);
            ASSERT_EQ(&array[0], &first_element);

            // The second element to emplace by move construct
            type second_element_to_move(1);

            // Emplace second element
            const type& second_element = array.emplace_at_to_ref(0, hud::move(second_element_to_move));

            // Ensure we correctly emplaced the second element and we really reallocate the array
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), 2u);
            ASSERT_EQ(array.max_count(), 2u);
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
            ASSERT_EQ(array[0], second_element_to_move);
            ASSERT_EQ(&array[0], &second_element);
            ASSERT_EQ(array[1], first_element_to_move);
            ASSERT_NE(&array[1], &first_element);
            ASSERT_EQ(&first_element, &second_element);
        }
    }
}
