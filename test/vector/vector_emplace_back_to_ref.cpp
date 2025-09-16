#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(vector, emplace_back_to_ref_can_default_construct_non_trivially_default_constructible_type)
{

    using type = hud_test::default_constructible_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_trivially_default_constructible_v<type>);
    static_assert(hud::is_default_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            const type &ref = vector.emplace_back_to_ref();

            // Ensure the vector grows
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), index + 1);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(vector[index].id(), hud_test::default_constructible_type::DEFAULT_ID_VALUE);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), index + 1);
            hud_assert_eq(vector.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        vector.reserve(element_count);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), element_count);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            const type &ref = vector.emplace_back_to_ref();

            // Ensure the vector has not grown
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), element_count);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(vector[index].id(), hud_test::default_constructible_type::DEFAULT_ID_VALUE);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), 1u);
            hud_assert_eq(vector.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(vector, emplace_back_to_ref_can_default_construct_trivially_default_constructible_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_trivially_default_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            const type &ref = vector.emplace_back_to_ref();

            // Ensure the vector grows
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), index + 1);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(vector[index], 0u); // default value of usize is 0 when construct

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), index + 1);
            hud_assert_eq(vector.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        vector.reserve(element_count);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), element_count);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            const type &ref = vector.emplace_back_to_ref();

            // Ensure the vector has not grown
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), element_count);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(vector[index], 0u); // default value of usize is 0 when construct

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), 1u);
            hud_assert_eq(vector.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(vector, emplace_back_to_ref_can_construct_non_trivially_constructible_type)
{

    using type = hud_test::NonDefaultConstructibleType;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_trivially_constructible_v<type, usize>);
    static_assert(hud::is_constructible_v<type, usize>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            const type &ref = vector.emplace_back_to_ref(static_cast<i32>(index));

            // Ensure the vector grows
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), index + 1);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(static_cast<usize>(ref.id()), index);
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(static_cast<usize>(vector[index].id()), index);
            hud_assert_eq(vector[index].constructor_count(), 1u);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), index + 1);
            hud_assert_eq(vector.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        vector.reserve(element_count);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), element_count);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            const type &ref = vector.emplace_back_to_ref(static_cast<i32>(index));

            // Ensure the vector has not grown
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), element_count);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(static_cast<usize>(ref.id()), index);
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(static_cast<usize>(vector[index].id()), index);
            hud_assert_eq(vector[index].constructor_count(), 1u);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), 1u);
            hud_assert_eq(vector.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(vector, emplace_back_to_ref_can_construct_trivially_constructible_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_trivially_constructible_v<type, usize>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            const type &ref = vector.emplace_back_to_ref(static_cast<i32>(index));

            // Ensure the vector grows
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), index + 1);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(vector[index], index);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), index + 1);
            hud_assert_eq(vector.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        vector.reserve(element_count);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), element_count);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace default construct elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace default construct element
            const type &ref = vector.emplace_back_to_ref(static_cast<i32>(index));

            // Ensure the vector has not grown
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), element_count);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(vector[index], index);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), 1u);
            hud_assert_eq(vector.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(vector, emplace_back_to_ref_can_copy_construct_non_trivially_copy_constructible_type)
{

    using type = hud_test::non_bitwise_copy_constructible_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    static_assert(hud::is_copy_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace default construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            const type element_to_copy(index);
            hud_assert_eq(element_to_copy.copy_constructor_count(), 0u);

            // Emplace copy construct element
            const type &ref = vector.emplace_back_to_ref(element_to_copy);

            // Ensure the vector grows
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), index + 1);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(static_cast<usize>(ref.id()), index);
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(static_cast<usize>(vector[index].id()), index);
            hud_assert_eq(vector[index].copy_constructor_count(), 1u);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), index + 1);
            hud_assert_eq(vector.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        vector.reserve(element_count);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), element_count);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace copy construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            const type element_to_copy(index);
            hud_assert_eq(element_to_copy.copy_constructor_count(), 0u);

            // Emplace copy construct element
            const type &ref = vector.emplace_back_to_ref(element_to_copy);

            // Ensure the vector has not grown
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), element_count);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(static_cast<usize>(ref.id()), index);
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(static_cast<usize>(vector[index].id()), index);
            hud_assert_eq(vector[index].copy_constructor_count(), 1u);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), 1u);
            hud_assert_eq(vector.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(vector, emplace_back_to_ref_can_copy_construct_trivially_copy_constructible_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_trivially_copy_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace default construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace copy construct element
            const type element_to_copy(index);
            const type &ref = vector.emplace_back_to_ref(element_to_copy);

            // Ensure the vector grows
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), index + 1);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(vector[index], element_to_copy);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), index + 1);
            hud_assert_eq(vector.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        vector.reserve(element_count);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), element_count);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace copy construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace copy construct element
            const type element_to_copy(index);
            const type &ref = vector.emplace_back_to_ref(element_to_copy);

            // Ensure the vector has not grown
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), element_count);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(vector[index], element_to_copy);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), 1u);
            hud_assert_eq(vector.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(vector, emplace_back_to_ref_can_move_construct_non_bitwise_move_constructible_type)
{

    using type = hud_test::non_bitwise_move_constructible_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_bitwise_move_constructible_v<type>);
    static_assert(hud::is_move_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace default construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            type element_to_move(index);
            hud_assert_eq(element_to_move.move_constructor_count(), 0u);

            // Emplace copy construct element
            const type &ref = vector.emplace_back_to_ref(hud::move(element_to_move));

            // Ensure the vector grows
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), index + 1);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(static_cast<usize>(ref.id()), index);
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(static_cast<usize>(vector[index].id()), index);
            hud_assert_eq(vector[index].move_constructor_count(), 1u);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), index + 1);
            hud_assert_eq(vector.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        vector.reserve(element_count);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), element_count);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace copy construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            type element_to_move(index);
            hud_assert_eq(element_to_move.move_constructor_count(), 0u);

            // Emplace copy construct element
            const type &ref = vector.emplace_back_to_ref(hud::move(element_to_move));

            // Ensure the vector has not grown
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), element_count);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(static_cast<usize>(ref.id()), index);
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(static_cast<usize>(vector[index].id()), index);
            hud_assert_eq(vector[index].move_constructor_count(), 1u);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), 1u);
            hud_assert_eq(vector.allocator().free_count(), 0u);
        }
    }
}

GTEST_TEST(vector, emplace_back_to_ref_can_move_construct_bitwise_move_constructible_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_bitwise_move_constructible_v<type>);

    // test with reallocation
    for (usize element_count = 0; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace default construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace copy construct element
            const type &ref = vector.emplace_back_to_ref(type(index));

            // Ensure the vector grows
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), index + 1);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(vector[index], index);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), index + 1);
            hud_assert_eq(vector.allocator().free_count(), index);
        }
    }

    // test with no reallocation
    for (usize element_count = 1; element_count < 5; element_count++) {
        array_type vector;
        hud_assert_eq(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), 0u);
        hud_assert_eq(vector.allocator().allocation_count(), 0u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        vector.reserve(element_count);
        hud_assert_ne(vector.data(), nullptr);
        hud_assert_eq(vector.count(), 0u);
        hud_assert_eq(vector.max_count(), element_count);
        hud_assert_eq(vector.allocator().allocation_count(), 1u);
        hud_assert_eq(vector.allocator().free_count(), 0u);

        // Emplace copy construct a list of elements
        for (usize index = 0; index < element_count; index++) {
            // Emplace copy construct element
            const type &ref = vector.emplace_back_to_ref(type(index));

            // Ensure the vector has not grown
            hud_assert_ne(vector.data(), nullptr);
            hud_assert_eq(vector.count(), index + 1);
            hud_assert_eq(vector.max_count(), element_count);

            // Ensure the returned ref is refering to the correct element
            hud_assert_eq(&ref, &vector[index]);
            hud_assert_eq(vector[index], index);

            // Ensure we really reallocate
            hud_assert_eq(vector.allocator().allocation_count(), 1u);
            hud_assert_eq(vector.allocator().free_count(), 0u);
        }
    }
}