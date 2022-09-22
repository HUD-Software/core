#include <core/containers/array.h>
#include "allocators.h"

TEST(array, emplace_back_can_default_construct_non_trivially_default_constructible_type)
{

    using type = hud_test::default_constructible_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;
    static_assert(!hud::is_trivially_default_constructible_v<type>);
    static_assert(hud::is_default_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            ASSERT_EQ(array.emplace_back(), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), index + 1);
            ASSERT_EQ(array[index].id(), hud_test::default_constructible_type::DEFAULT_ID_VALUE);

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), index + 1);
            ASSERT_EQ(array.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        ASSERT_NE(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), element_count);
        ASSERT_EQ(array.allocator().allocation_count(), 1u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            ASSERT_EQ(array.emplace_back(), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), element_count);
            ASSERT_EQ(array[index].id(), hud_test::default_constructible_type::DEFAULT_ID_VALUE);

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
        }
    }
}

TEST(array, emplace_back_can_default_construct_trivially_default_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;
    static_assert(hud::is_trivially_default_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            ASSERT_EQ(array.emplace_back(), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), index + 1);
            ASSERT_EQ(array[index], 0u); // default value of usize is 0 when construct

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), index + 1);
            ASSERT_EQ(array.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        ASSERT_NE(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), element_count);
        ASSERT_EQ(array.allocator().allocation_count(), 1u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            ASSERT_EQ(array.emplace_back(), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), element_count);
            ASSERT_EQ(array[index], 0u); // default value of usize is 0 when construct

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
        }
    }
}

TEST(array, emplace_back_can_construct_non_trivially_constructible_type)
{

    using type = hud_test::NonDefaultConstructibleType;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;
    static_assert(!hud::is_trivially_constructible_v<type, usize>);
    static_assert(hud::is_constructible_v<type, usize>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            ASSERT_EQ(array.emplace_back(static_cast<i32>(index)), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), index + 1);
            ASSERT_EQ(static_cast<usize>(array[index].id()), index);
            ASSERT_EQ(array[index].constructor_count(), 1u);

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), index + 1);
            ASSERT_EQ(array.allocator().free_count(), index);
        }
    }


    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        ASSERT_NE(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), element_count);
        ASSERT_EQ(array.allocator().allocation_count(), 1u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            ASSERT_EQ(array.emplace_back(static_cast<i32>(index)), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), element_count);
            ASSERT_EQ(static_cast<usize>(array[index].id()), index);
            ASSERT_EQ(array[index].constructor_count(), 1u);

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
        }
    }
}

TEST(array, emplace_back_can_construct_trivially_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;
    static_assert(hud::is_trivially_constructible_v<type, usize>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            ASSERT_EQ(array.emplace_back(index), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), index + 1);
            ASSERT_EQ(array[index], index);

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), index + 1);
            ASSERT_EQ(array.allocator().free_count(), index);
        }
    }


    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        ASSERT_NE(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), element_count);
        ASSERT_EQ(array.allocator().allocation_count(), 1u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            ASSERT_EQ(array.emplace_back(index), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), element_count);
            ASSERT_EQ(array[index], index);

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
        }
    }
}

TEST(array, emplace_back_can_copy_construct_non_bitwise_copy_constructible_type)
{

    using type = hud_test::NonBitwiseCopyConstructibleType;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);
    static_assert(hud::is_copy_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace default construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            const type element_to_copy(index);
            ASSERT_EQ(element_to_copy.copy_constructor_count(), 0u);

            // Emplace copy construct element
            ASSERT_EQ(array.emplace_back(element_to_copy), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), index + 1);
            ASSERT_EQ(static_cast<usize>(array[index].id()), index);
            ASSERT_EQ(array[index].copy_constructor_count(), 1u);

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), index + 1);
            ASSERT_EQ(array.allocator().free_count(), index);
        }
    }


    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        ASSERT_NE(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), element_count);
        ASSERT_EQ(array.allocator().allocation_count(), 1u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace copy construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            const type element_to_copy(index);
            ASSERT_EQ(element_to_copy.copy_constructor_count(), 0u);

            // Emplace copy construct element
            ASSERT_EQ(array.emplace_back(element_to_copy), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), element_count);
            ASSERT_EQ(static_cast<usize>(array[index].id()), index);
            ASSERT_EQ(array[index].copy_constructor_count(), 1u);

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
        }
    }
}

TEST(array, emplace_back_can_copy_construct_bitwise_copy_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;
    static_assert(hud::is_bitwise_copy_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace default construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            const type element_to_copy(index);

            // Emplace copy construct element
            ASSERT_EQ(array.emplace_back(element_to_copy), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), index + 1);
            ASSERT_EQ(array[index], element_to_copy);

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), index + 1);
            ASSERT_EQ(array.allocator().free_count(), index);
        }
    }


    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        ASSERT_NE(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), element_count);
        ASSERT_EQ(array.allocator().allocation_count(), 1u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace copy construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            const type element_to_copy(index);

            // Emplace copy construct element
            ASSERT_EQ(array.emplace_back(element_to_copy), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), element_count);
            ASSERT_EQ(array[index], element_to_copy);

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
        }
    }
}

TEST(array, emplace_back_can_move_construct_non_bitwise_move_constructible_type)
{

    using type = hud_test::NonBitwiseMoveConstructibleType;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;
    static_assert(!hud::is_bitwise_move_constructible_v<type>);
    static_assert(hud::is_move_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace default construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            type element_to_move(index);
            ASSERT_EQ(element_to_move.move_constructor_count(), 0u);

            // Emplace copy construct element
            ASSERT_EQ(array.emplace_back(hud::move(element_to_move)), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), index + 1);
            ASSERT_EQ(static_cast<usize>(array[index].id()), index);
            ASSERT_EQ(array[index].move_constructor_count(), 1u);

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), index + 1);
            ASSERT_EQ(array.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        ASSERT_NE(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), element_count);
        ASSERT_EQ(array.allocator().allocation_count(), 1u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace copy construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            type element_to_move(index);
            ASSERT_EQ(element_to_move.move_constructor_count(), 0u);

            // Emplace copy construct element
            ASSERT_EQ(array.emplace_back(hud::move(element_to_move)), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), element_count);
            ASSERT_EQ(static_cast<usize>(array[index].id()), index);
            ASSERT_EQ(array[index].move_constructor_count(), 1u);

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
        }
    }
}

TEST(array, emplace_back_can_move_construct_bitwise_move_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;
    static_assert(hud::is_bitwise_move_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace default construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace copy construct element
            ASSERT_EQ(array.emplace_back(type(index)), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), index + 1);
            ASSERT_EQ(array[index], type(index));

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), index + 1);
            ASSERT_EQ(array.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type array;
        ASSERT_EQ(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), 0u);
        ASSERT_EQ(array.allocator().allocation_count(), 0u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        array.reserve(element_count);
        ASSERT_NE(array.data(), nullptr);
        ASSERT_EQ(array.count(), 0u);
        ASSERT_EQ(array.max_count(), element_count);
        ASSERT_EQ(array.allocator().allocation_count(), 1u);
        ASSERT_EQ(array.allocator().free_count(), 0u);

        // Emplace copy construct a list of elements
        for (usize index = 0; index < element_count; index++) {

            // Emplace copy construct element
            ASSERT_EQ(array.emplace_back(type(index)), index);
            ASSERT_NE(array.data(), nullptr);
            ASSERT_EQ(array.count(), index + 1);
            ASSERT_EQ(array.max_count(), element_count);
            ASSERT_EQ(array[index], type(index));

            // Ensure we really reallocate
            ASSERT_EQ(array.allocator().allocation_count(), 1u);
            ASSERT_EQ(array.allocator().free_count(), 0u);
        }
    }
}