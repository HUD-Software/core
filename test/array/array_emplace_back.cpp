#include <core/containers/array.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(array, emplace_back_can_default_construct_non_trivially_default_constructible_type)
{

    using type = hud_test::default_constructible_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_trivially_default_constructible_v<type>);
    static_assert(hud::is_default_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++)
        {
            // Emplace default construct element
            hud_assert_eq(array.emplace_back(), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), index + 1);
            hud_assert_eq(array[index].id(), hud_test::default_constructible_type::DEFAULT_ID_VALUE);

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), index + 1);
            hud_assert_eq(array.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        hud_assert_ne(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), element_count);
        hud_assert_eq(array.allocator().allocation_count(), 1u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++)
        {
            // Emplace default construct element
            hud_assert_eq(array.emplace_back(), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), element_count);
            hud_assert_eq(array[index].id(), hud_test::default_constructible_type::DEFAULT_ID_VALUE);

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(array, emplace_back_can_default_construct_trivially_default_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_trivially_default_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++)
        {
            // Emplace default construct element
            hud_assert_eq(array.emplace_back(), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), index + 1);
            hud_assert_eq(array[index], 0u); // default value of usize is 0 when construct

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), index + 1);
            hud_assert_eq(array.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        hud_assert_ne(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), element_count);
        hud_assert_eq(array.allocator().allocation_count(), 1u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++)
        {
            // Emplace default construct element
            hud_assert_eq(array.emplace_back(), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), element_count);
            hud_assert_eq(array[index], 0u); // default value of usize is 0 when construct

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(array, emplace_back_can_construct_non_trivially_constructible_type)
{

    using type = hud_test::NonDefaultConstructibleType;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_trivially_constructible_v<type, usize>);
    static_assert(hud::is_constructible_v<type, usize>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++)
        {
            // Emplace default construct element
            hud_assert_eq(array.emplace_back(static_cast<i32>(index)), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), index + 1);
            hud_assert_eq(static_cast<usize>(array[index].id()), index);
            hud_assert_eq(array[index].constructor_count(), 1u);

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), index + 1);
            hud_assert_eq(array.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        hud_assert_ne(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), element_count);
        hud_assert_eq(array.allocator().allocation_count(), 1u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++)
        {
            // Emplace default construct element
            hud_assert_eq(array.emplace_back(static_cast<i32>(index)), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), element_count);
            hud_assert_eq(static_cast<usize>(array[index].id()), index);
            hud_assert_eq(array[index].constructor_count(), 1u);

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(array, emplace_back_can_construct_trivially_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_trivially_constructible_v<type, usize>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++)
        {
            // Emplace default construct element
            hud_assert_eq(array.emplace_back(index), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), index + 1);
            hud_assert_eq(array[index], index);

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), index + 1);
            hud_assert_eq(array.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        hud_assert_ne(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), element_count);
        hud_assert_eq(array.allocator().allocation_count(), 1u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++)
        {
            // Emplace default construct element
            hud_assert_eq(array.emplace_back(index), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), element_count);
            hud_assert_eq(array[index], index);

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(array, emplace_back_can_copy_construct_non_bitwise_copy_constructible_type)
{

    using type = hud_test::NonBitwiseCopyConstructibleType;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);
    static_assert(hud::is_copy_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace default construct a list of elements
        for (usize index = 0; index < element_count; index++)
        {
            const type element_to_copy(index);
            hud_assert_eq(element_to_copy.copy_constructor_count(), 0u);

            // Emplace copy construct element
            hud_assert_eq(array.emplace_back(element_to_copy), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), index + 1);
            hud_assert_eq(static_cast<usize>(array[index].id()), index);
            hud_assert_eq(array[index].copy_constructor_count(), 1u);

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), index + 1);
            hud_assert_eq(array.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        hud_assert_ne(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), element_count);
        hud_assert_eq(array.allocator().allocation_count(), 1u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace copy construct a list of elements
        for (usize index = 0; index < element_count; index++)
        {
            const type element_to_copy(index);
            hud_assert_eq(element_to_copy.copy_constructor_count(), 0u);

            // Emplace copy construct element
            hud_assert_eq(array.emplace_back(element_to_copy), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), element_count);
            hud_assert_eq(static_cast<usize>(array[index].id()), index);
            hud_assert_eq(array[index].copy_constructor_count(), 1u);

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(array, emplace_back_can_copy_construct_bitwise_copy_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_bitwise_copy_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace default construct a list of elements
        for (usize index = 0; index < element_count; index++)
        {
            const type element_to_copy(index);

            // Emplace copy construct element
            hud_assert_eq(array.emplace_back(element_to_copy), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), index + 1);
            hud_assert_eq(array[index], element_to_copy);

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), index + 1);
            hud_assert_eq(array.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        hud_assert_ne(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), element_count);
        hud_assert_eq(array.allocator().allocation_count(), 1u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace copy construct a list of elements
        for (usize index = 0; index < element_count; index++)
        {
            const type element_to_copy(index);

            // Emplace copy construct element
            hud_assert_eq(array.emplace_back(element_to_copy), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), element_count);
            hud_assert_eq(array[index], element_to_copy);

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(array, emplace_back_can_move_construct_non_bitwise_move_constructible_type)
{

    using type = hud_test::NonBitwiseMoveConstructibleType;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_bitwise_move_constructible_v<type>);
    static_assert(hud::is_move_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace default construct a list of elements
        for (usize index = 0; index < element_count; index++)
        {
            type element_to_move(index);
            hud_assert_eq(element_to_move.move_constructor_count(), 0u);

            // Emplace copy construct element
            hud_assert_eq(array.emplace_back(hud::move(element_to_move)), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), index + 1);
            hud_assert_eq(static_cast<usize>(array[index].id()), index);
            hud_assert_eq(array[index].move_constructor_count(), 1u);

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), index + 1);
            hud_assert_eq(array.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        hud_assert_ne(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), element_count);
        hud_assert_eq(array.allocator().allocation_count(), 1u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace copy construct a list of elements
        for (usize index = 0; index < element_count; index++)
        {
            type element_to_move(index);
            hud_assert_eq(element_to_move.move_constructor_count(), 0u);

            // Emplace copy construct element
            hud_assert_eq(array.emplace_back(hud::move(element_to_move)), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), element_count);
            hud_assert_eq(static_cast<usize>(array[index].id()), index);
            hud_assert_eq(array[index].move_constructor_count(), 1u);

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(array, emplace_back_can_move_construct_bitwise_move_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_bitwise_move_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace default construct a list of elements
        for (usize index = 0; index < element_count; index++)
        {
            // Emplace copy construct element
            hud_assert_eq(array.emplace_back(type(index)), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), index + 1);
            hud_assert_eq(array[index], type(index));

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), index + 1);
            hud_assert_eq(array.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++)
    {
        array_type array;
        hud_assert_eq(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), 0u);
        hud_assert_eq(array.allocator().allocation_count(), 0u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        hud_assert_ne(array.data(), nullptr);
        hud_assert_eq(array.count(), 0u);
        hud_assert_eq(array.max_count(), element_count);
        hud_assert_eq(array.allocator().allocation_count(), 1u);
        hud_assert_eq(array.allocator().free_count(), 0u);

        // Emplace copy construct a list of elements
        for (usize index = 0; index < element_count; index++)
        {

            // Emplace copy construct element
            hud_assert_eq(array.emplace_back(type(index)), index);
            hud_assert_ne(array.data(), nullptr);
            hud_assert_eq(array.count(), index + 1);
            hud_assert_eq(array.max_count(), element_count);
            hud_assert_eq(array[index], type(index));

            // Ensure we really reallocate
            hud_assert_eq(array.allocator().allocation_count(), 1u);
            hud_assert_eq(array.allocator().free_count(), 0u);
        }
    }
}