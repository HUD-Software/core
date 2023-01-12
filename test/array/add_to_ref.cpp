#include <core/containers/array.h>
#include "allocators.h"

GTEST_TEST(array, add_to_ref_by_copy_construct_non_bitwise_copy_constructible_type)
{

    using type = hud_test::non_bitwise_copy_assignable_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;
    static_assert(!hud::is_bitwise_copy_constructible_v<type, type>);
    static_assert(hud::is_copy_constructible_v<type, type>);

    // With reallocation
    {
        const auto test = []()
        {
            array_type array;
            const type element_to_copy;

            // Add one element
            const type &ref_0 = array.add_to_ref(element_to_copy);
            const auto first_element_result = std::tuple{
                &ref_0 == array.data(),
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0].copy_assign_count(),
                array[0].copy_constructor_count(),
                array.allocator().allocation_count(),
                array.allocator().free_count()};
            // Add another element
            const type &ref_1 = array.add_to_ref(element_to_copy);
            const auto second_element_result = std::tuple{
                &ref_1 == array.data() + 1,
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0].copy_assign_count(),
                array[0].copy_constructor_count(),
                array[1].copy_assign_count(),
                array[1].copy_constructor_count(),
                array.allocator().allocation_count(),
                array.allocator().free_count()};
            return std::tuple{
                first_element_result,
                second_element_result};
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<8>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<9>(second_element_result), 1u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<8>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<9>(second_element_result), 1u);
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            array_type array;
            array.reserve(2);
            const type element_to_copy;

            // Add one element
            const type &ref_0 = array.add_to_ref(element_to_copy);
            const auto first_element_result = std::tuple{
                &ref_0 == array.data(),
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0].copy_assign_count(),
                array[0].copy_constructor_count(),
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };
            // Add another element
            const type &ref_1 = array.add_to_ref(element_to_copy);
            const auto second_element_result = std::tuple{
                &ref_1 == array.data() + 1,
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0].copy_assign_count(),
                array[0].copy_constructor_count(),
                array[1].copy_assign_count(),
                array[1].copy_constructor_count(),
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };
            return std::tuple{
                first_element_result,
                second_element_result};
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<8>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<9>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<8>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<9>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(array, add_to_ref_by_copy_construct_bitwise_copy_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;
    static_assert(hud::is_bitwise_copy_constructible_v<type, type>);

    // With reallocation
    {
        const auto test = []()
        {
            array_type array;

            // Add one element
            const type element_to_copy_0(1u);
            const type &ref_0 = array.add_to_ref(element_to_copy_0);
            const auto first_element_result = std::tuple{
                &ref_0 == array.data(),
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0],
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };

            // Add another element
            const type element_to_copy_1(2u);
            const type &ref_1 = array.add_to_ref(element_to_copy_1);
            const auto second_element_result = std::tuple{
                &ref_1 == array.data() + 1,
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0],
                array[1],
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };
            return std::tuple{
                first_element_result,
                second_element_result};
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 1u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 1u);
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            array_type array;
            array.reserve(2);

            // Add one element
            const type element_to_copy_0(1u);
            const type &ref_0 = array.add_to_ref(element_to_copy_0);
            const auto first_element_result = std::tuple{
                &ref_0 == array.data(),
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0],
                array.allocator().allocation_count(),
                array.allocator().free_count()};

            // Add another element
            const type element_to_copy_1(2u);
            const type &ref_1 = array.add_to_ref(element_to_copy_1);
            const auto second_element_result = std::tuple{
                &ref_1 == array.data() + 1,
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0],
                array[1],
                array.allocator().allocation_count(),
                array.allocator().free_count()};
            return std::tuple{
                first_element_result,
                second_element_result};
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(array, add_to_ref_by_move_construct_non_bitwise_move_constructible_type)
{

    using type = hud_test::NonBitwiseMoveConstructibleType;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;
    static_assert(!hud::is_bitwise_move_constructible_v<type>);
    static_assert(hud::is_move_constructible_v<type>);

    // With reallocation
    {
        const auto test = []()
        {
            array_type array;

            // Add one element
            const type &ref_0 = array.add_to_ref(type());
            const auto first_element_result = std::tuple{
                &ref_0 == array.data(),
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0].move_constructor_count(),
                array[0].copy_constructor_count(),
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };
            // Add another element
            const type &ref_1 = array.add_to_ref(type());
            const auto second_element_result = std::tuple{
                &ref_1 == array.data() + 1,
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0].move_constructor_count(),
                array[0].copy_constructor_count(),
                array[1].move_constructor_count(),
                array[1].copy_constructor_count(),
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };
            return std::tuple{
                first_element_result,
                second_element_result};
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<8>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<9>(second_element_result), 1u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<8>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<9>(second_element_result), 1u);
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            array_type array;
            array.reserve(2);

            // Add one element
            const type &ref_0 = array.add_to_ref(type());
            const auto first_element_result = std::tuple{
                &ref_0 == array.data(),
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0].move_constructor_count(),
                array[0].copy_constructor_count(),
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };
            // Add another element
            const type &ref_1 = array.add_to_ref(type());
            const auto second_element_result = std::tuple{
                &ref_1 == array.data() + 1,
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0].move_constructor_count(),
                array[0].copy_constructor_count(),
                array[1].move_constructor_count(),
                array[1].copy_constructor_count(),
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };
            return std::tuple{
                first_element_result,
                second_element_result};
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<8>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<9>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 0u);
            GTEST_ASSERT_EQ(std::get<8>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<9>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(array, add_to_ref_by_move_construct_bitwise_move_constructible_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;
    static_assert(hud::is_bitwise_move_constructible_v<type>);

    // With reallocation
    {
        const auto test = []()
        {
            array_type array;

            // Add one element
            const type &ref_0 = array.add_to_ref(1u);
            const auto first_element_result = std::tuple{
                &ref_0 == array.data(),
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0],
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };

            // Add another element
            const type &ref_1 = array.add_to_ref(2u);
            const auto second_element_result = std::tuple{
                &ref_1 == array.data() + 1,
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0],
                array[1],
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };
            return std::tuple{
                first_element_result,
                second_element_result};
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 1u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 1u);
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            array_type array;
            array.reserve(2);

            // Add one element
            const type &ref_0 = array.add_to_ref(1u);
            const auto first_element_result = std::tuple{
                &ref_0 == array.data(),
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0],
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };

            // Add another element
            const type &ref_1 = array.add_to_ref(2u);
            const auto second_element_result = std::tuple{
                &ref_1 == array.data() + 1,
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0],
                array[1],
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };
            return std::tuple{
                first_element_result,
                second_element_result};
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(array, add_to_ref_by_move_construct_non_bitwise_copy_constructible_type)
{

    using type = hud_test::NonBitwiseCopyConstructibleType;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);
    static_assert(hud::is_copy_constructible_v<type>);

    // With reallocation
    {
        const auto test = []()
        {
            array_type array;
            const type element;

            // Add one element
            const type &ref_0 = array.add_to_ref(element);
            const auto first_element_result = std::tuple{
                &ref_0 == array.data(),
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0].copy_constructor_count(),
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };
            // Add another element
            const type &ref_1 = array.add_to_ref(element);
            const auto second_element_result = std::tuple{
                &ref_1 == array.data() + 1,
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0].copy_constructor_count(),
                array[1].copy_constructor_count(),
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };
            return std::tuple{
                first_element_result,
                second_element_result};
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 1u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 1u);
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            array_type array;
            array.reserve(2);

            const type element;

            // Add one element
            const type &ref_0 = array.add_to_ref(element);
            const auto first_element_result = std::tuple{
                &ref_0 == array.data(),
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0].copy_constructor_count(),
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };
            // Add another element
            const type &ref_1 = array.add_to_ref(element);
            const auto second_element_result = std::tuple{
                &ref_1 == array.data() + 1,
                array.data() != nullptr,
                array.count(),
                array.max_count(),
                array[0].copy_constructor_count(),
                array[1].copy_constructor_count(),
                array.allocator().allocation_count(),
                array.allocator().free_count(),
            };
            return std::tuple{
                first_element_result,
                second_element_result};
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            GTEST_ASSERT_TRUE(std::get<0>(first_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(first_element_result));
            GTEST_ASSERT_EQ(std::get<2>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<3>(first_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(first_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(first_element_result), 0u);

            // Second element is correctly added
            const auto second_element_result = std::get<1>(result);
            GTEST_ASSERT_TRUE(std::get<0>(second_element_result));
            GTEST_ASSERT_TRUE(std::get<1>(second_element_result));
            GTEST_ASSERT_EQ(std::get<2>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<3>(second_element_result), 2u);
            GTEST_ASSERT_EQ(std::get<4>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<5>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<6>(second_element_result), 1u);
            GTEST_ASSERT_EQ(std::get<7>(second_element_result), 0u);
        }
    }
}