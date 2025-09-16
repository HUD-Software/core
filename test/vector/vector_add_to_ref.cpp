#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(vector, add_to_ref_by_copy_construct_non_bitwise_copy_constructible_type)
{
    using type = hud_test::non_bitwise_copy_constructible_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_bitwise_copy_constructible_v<type, type>);
    static_assert(hud::is_copy_constructible_v<type, type>);

    // With reallocation
    {
        const auto test = []() {
            array_type vector;
            const type element_to_copy;

            // Add one element
            const type &ref_0 = vector.add_to_ref(element_to_copy);
            const auto first_element_result = std::tuple {
                &ref_0 == vector.data(),
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0].copy_constructor_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count()
            };
            // Add another element
            const type &ref_1 = vector.add_to_ref(element_to_copy);
            const auto second_element_result = std::tuple {
                &ref_1 == vector.data() + 1,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0].copy_constructor_count(),
                vector[1].copy_constructor_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count()
            };
            return std::tuple {
                first_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 2u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
            hud_assert_eq(std::get<7>(second_element_result), 1u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 2u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
            hud_assert_eq(std::get<7>(second_element_result), 1u);
        }
    }

    // Without reallocation
    {
        const auto test = []() {
            array_type vector;
            vector.reserve(2);
            const type element_to_copy;

            // Add one element
            const type &ref_0 = vector.add_to_ref(element_to_copy);
            const auto first_element_result = std::tuple {
                &ref_0 == vector.data(),
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0].copy_constructor_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };
            // Add another element
            const type &ref_1 = vector.add_to_ref(element_to_copy);
            const auto second_element_result = std::tuple {
                &ref_1 == vector.data() + 1,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0].copy_constructor_count(),
                vector[1].copy_constructor_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };
            return std::tuple {
                first_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(vector, add_to_ref_by_copy_construct_bitwise_copy_constructible_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_bitwise_copy_constructible_v<type, type>);

    // With reallocation
    {
        const auto test = []() {
            array_type vector;

            // Add one element
            const type element_to_copy_0(1u);
            const type &ref_0 = vector.add_to_ref(element_to_copy_0);
            const auto first_element_result = std::tuple {
                &ref_0 == vector.data(),
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0],
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };

            // Add another element
            const type element_to_copy_1(2u);
            const type &ref_1 = vector.add_to_ref(element_to_copy_1);
            const auto second_element_result = std::tuple {
                &ref_1 == vector.data() + 1,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0],
                vector[1],
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };
            return std::tuple {
                first_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
            hud_assert_eq(std::get<7>(second_element_result), 1u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
            hud_assert_eq(std::get<7>(second_element_result), 1u);
        }
    }

    // Without reallocation
    {
        const auto test = []() {
            array_type vector;
            vector.reserve(2);

            // Add one element
            const type element_to_copy_0(1u);
            const type &ref_0 = vector.add_to_ref(element_to_copy_0);
            const auto first_element_result = std::tuple {
                &ref_0 == vector.data(),
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0],
                vector.allocator().allocation_count(),
                vector.allocator().free_count()
            };

            // Add another element
            const type element_to_copy_1(2u);
            const type &ref_1 = vector.add_to_ref(element_to_copy_1);
            const auto second_element_result = std::tuple {
                &ref_1 == vector.data() + 1,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0],
                vector[1],
                vector.allocator().allocation_count(),
                vector.allocator().free_count()
            };
            return std::tuple {
                first_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(vector, add_to_ref_by_move_construct_non_bitwise_move_constructible_type)
{

    using type = hud_test::non_bitwise_move_constructible_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_bitwise_move_constructible_v<type>);
    static_assert(hud::is_move_constructible_v<type>);

    // With reallocation
    {
        const auto test = []() {
            array_type vector;

            // Add one element
            const type &ref_0 = vector.add_to_ref(type());
            const auto first_element_result = std::tuple {
                &ref_0 == vector.data(),
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0].move_constructor_count(),
                vector[0].copy_constructor_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };
            // Add another element
            const type &ref_1 = vector.add_to_ref(type());
            const auto second_element_result = std::tuple {
                &ref_1 == vector.data() + 1,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0].move_constructor_count(),
                vector[0].copy_constructor_count(),
                vector[1].move_constructor_count(),
                vector[1].copy_constructor_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };
            return std::tuple {
                first_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 0u);
            hud_assert_eq(std::get<6>(first_element_result), 1u);
            hud_assert_eq(std::get<7>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 2u);
            hud_assert_eq(std::get<5>(second_element_result), 0u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 0u);
            hud_assert_eq(std::get<8>(second_element_result), 2u);
            hud_assert_eq(std::get<9>(second_element_result), 1u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 0u);
            hud_assert_eq(std::get<6>(first_element_result), 1u);
            hud_assert_eq(std::get<7>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 2u);
            hud_assert_eq(std::get<5>(second_element_result), 0u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 0u);
            hud_assert_eq(std::get<8>(second_element_result), 2u);
            hud_assert_eq(std::get<9>(second_element_result), 1u);
        }
    }

    // Without reallocation
    {
        const auto test = []() {
            array_type vector;
            vector.reserve(2);

            // Add one element
            const type &ref_0 = vector.add_to_ref(type());
            const auto first_element_result = std::tuple {
                &ref_0 == vector.data(),
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0].move_constructor_count(),
                vector[0].copy_constructor_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };
            // Add another element
            const type &ref_1 = vector.add_to_ref(type());
            const auto second_element_result = std::tuple {
                &ref_1 == vector.data() + 1,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0].move_constructor_count(),
                vector[0].copy_constructor_count(),
                vector[1].move_constructor_count(),
                vector[1].copy_constructor_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };
            return std::tuple {
                first_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 0u);
            hud_assert_eq(std::get<6>(first_element_result), 1u);
            hud_assert_eq(std::get<7>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 0u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 0u);
            hud_assert_eq(std::get<8>(second_element_result), 1u);
            hud_assert_eq(std::get<9>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 0u);
            hud_assert_eq(std::get<6>(first_element_result), 1u);
            hud_assert_eq(std::get<7>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 0u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 0u);
            hud_assert_eq(std::get<8>(second_element_result), 1u);
            hud_assert_eq(std::get<9>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(vector, add_to_ref_by_move_construct_bitwise_move_constructible_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(hud::is_bitwise_move_constructible_v<type>);

    // With reallocation
    {
        const auto test = []() {
            array_type vector;

            // Add one element
            const type &ref_0 = vector.add_to_ref(1u);
            const auto first_element_result = std::tuple {
                &ref_0 == vector.data(),
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0],
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };

            // Add another element
            const type &ref_1 = vector.add_to_ref(2u);
            const auto second_element_result = std::tuple {
                &ref_1 == vector.data() + 1,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0],
                vector[1],
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };
            return std::tuple {
                first_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
            hud_assert_eq(std::get<7>(second_element_result), 1u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
            hud_assert_eq(std::get<7>(second_element_result), 1u);
        }
    }

    // Without reallocation
    {
        const auto test = []() {
            array_type vector;
            vector.reserve(2);

            // Add one element
            const type &ref_0 = vector.add_to_ref(1u);
            const auto first_element_result = std::tuple {
                &ref_0 == vector.data(),
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0],
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };

            // Add another element
            const type &ref_1 = vector.add_to_ref(2u);
            const auto second_element_result = std::tuple {
                &ref_1 == vector.data() + 1,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0],
                vector[1],
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };
            return std::tuple {
                first_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(vector, add_to_ref_by_move_construct_non_bitwise_copy_constructible_type)
{

    using type = hud_test::non_bitwise_copy_constructible_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);
    static_assert(hud::is_copy_constructible_v<type>);

    // With reallocation
    {
        const auto test = []() {
            array_type vector;
            const type element;

            // Add one element
            const type &ref_0 = vector.add_to_ref(element);
            const auto first_element_result = std::tuple {
                &ref_0 == vector.data(),
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0].copy_constructor_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };
            // Add another element
            const type &ref_1 = vector.add_to_ref(element);
            const auto second_element_result = std::tuple {
                &ref_1 == vector.data() + 1,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0].copy_constructor_count(),
                vector[1].copy_constructor_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };
            return std::tuple {
                first_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 2u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
            hud_assert_eq(std::get<7>(second_element_result), 1u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 2u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
            hud_assert_eq(std::get<7>(second_element_result), 1u);
        }
    }

    // Without reallocation
    {
        const auto test = []() {
            array_type vector;
            vector.reserve(2);

            const type element;

            // Add one element
            const type &ref_0 = vector.add_to_ref(element);
            const auto first_element_result = std::tuple {
                &ref_0 == vector.data(),
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0].copy_constructor_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };
            // Add another element
            const type &ref_1 = vector.add_to_ref(element);
            const auto second_element_result = std::tuple {
                &ref_1 == vector.data() + 1,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector[0].copy_constructor_count(),
                vector[1].copy_constructor_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count(),
            };
            return std::tuple {
                first_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_eq(std::get<2>(second_element_result), 2u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 0u);
        }
    }
}