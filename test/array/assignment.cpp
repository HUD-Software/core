#include <core/containers/array.h>
#include "../misc/allocators.h"
#include "../misc/leak_guard.h"

GTEST_TEST(array, assign_std_initializer_list_of_bitwise_copy_assignable_same_type)
{

    using type = i32;

    static_assert(hud::is_bitwise_copy_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> &&elements_in_assigned, std::initializer_list<type> elements_to_assign)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned);

            assigned = elements_to_assign;

            // Ensures we copy all values correctly
            [[maybe_unused]] bool all_values_are_copied = true;
            for (usize index = 0; index < assigned.count(); index++)
            {
                if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index)))
                {
                    // LCOV_EXCL_START
                    all_values_are_copied = false;
                    break;
                    // LCOV_EXCL_STOP
                }
            }

            return std::tuple {
                assigned.data() != nullptr,
                // Number of element should be equal to the number of element in the std::initializer_list
                assigned.count(),
                // Ensure we do no reallocate for less memory
                assigned.max_count(),
                assigned.allocator().allocation_count(),
                assigned.allocator().free_count()};
        };

        // Non constant
        {
            {const auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
}
}

// Test with extra
{
    const auto test = [](std::initializer_list<type> &&elements_in_assigned, const usize extra, std::initializer_list<type> elements_to_assign)
    {
        hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra);

        assigned = elements_to_assign;

        // Ensures we copy all values correctly
        [[maybe_unused]] bool all_values_are_copied = true;
        for (usize index = 0; index < assigned.count(); index++)
        {
            if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index)))
            {
                // LCOV_EXCL_START
                all_values_are_copied = false;
                break;
                // LCOV_EXCL_STOP
            }
        }

        return std::tuple {
            assigned.data() != nullptr,
            // Number of element should be equal to the number of element in the std::initializer_list
            assigned.count(),
            // Ensure we do no reallocate for less memory
            assigned.max_count(),
            assigned.allocator().allocation_count(),
            assigned.allocator().free_count()};
    };

    // Non constant
    {
        {const auto result = test({}, 0u, {});
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 0u, {1, 2});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 2u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 0u, {4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 2u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test({}, 0u, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 0u, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
}
}
}

GTEST_TEST(array, assign_std_initializer_list_of_bitwise_copy_assignable_different_type)
{

    using source_type = i32;
    using destination_type = u32;
    // Ensure we test with different types
    static_assert(!std::is_same_v<destination_type, source_type>);
    static_assert(hud::is_bitwise_copy_assignable_v<destination_type, source_type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<destination_type> &&elements_in_assigned, std::initializer_list<source_type> elements_to_assign)
        {
            hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned);

            assigned = elements_to_assign;

            // Ensures we copy all values correctly
            [[maybe_unused]] bool all_values_are_copied = true;
            for (usize index = 0; index < assigned.count(); index++)
            {
                if (assigned[index] != static_cast<destination_type>(*(elements_to_assign.begin() + index)))
                {
                    // LCOV_EXCL_START
                    all_values_are_copied = false;
                    break;
                    // LCOV_EXCL_STOP
                }
            }

            return std::tuple {
                assigned.data() != nullptr,
                // Number of element should be equal to the number of element in the std::initializer_list
                assigned.count(),
                // Ensure we do no reallocate for less memory
                assigned.max_count(),
                assigned.allocator().allocation_count(),
                assigned.allocator().free_count()};
        };

        // Non constant
        {
            {const auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
}
}

// Test with extra
{
    const auto test = [](std::initializer_list<destination_type> &&elements_in_assigned, const usize extra, std::initializer_list<source_type> elements_to_assign)
    {
        hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned, extra);

        assigned = elements_to_assign;

        // Ensures we copy all values correctly
        [[maybe_unused]] bool all_values_are_copied = true;
        for (usize index = 0; index < assigned.count(); index++)
        {
            if (assigned[index] != static_cast<destination_type>(*(elements_to_assign.begin() + index)))
            {
                // LCOV_EXCL_START
                all_values_are_copied = false;
                break;
                // LCOV_EXCL_STOP
            }
        }

        return std::tuple {
            assigned.data() != nullptr,
            // Number of element should be equal to the number of element in the std::initializer_list
            assigned.count(),
            // Ensure we do no reallocate for less memory
            assigned.max_count(),
            assigned.allocator().allocation_count(),
            assigned.allocator().free_count()};
    };

    // Non constant
    {
        {const auto result = test({}, 0u, {});
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 0u, {1, 2});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 2u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 0u, {4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 2u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test({}, 0u, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 0u, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
}
}
}

GTEST_TEST(array, assign_std_initializer_list_of_non_bitwise_copy_assignable_same_type)
{

    using type = hud_test::non_bitwise_copy_assignable_type;
    static_assert(!hud::is_bitwise_copy_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> &&elements_in_assigned, std::initializer_list<type> elements_to_assign)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned);

            assigned = elements_to_assign;

            // Ensures we copy all values correctly
            bool all_values_are_copied = true;
            bool all_copy_constructors_are_called = true;
            bool all_copy_assign_are_called = true;
            for (usize index = 0; index < assigned.count(); index++)
            {

                // Ensure we correctly copied the value
                if (assigned[index].id() != (elements_to_assign.begin() + index)->id())
                {
                    // LCOV_EXCL_START
                    all_values_are_copied = false;
                    break;
                    // LCOV_EXCL_STOP
                }

                // Ensure we correctly call copy constructors
                if (elements_in_assigned.size() < elements_to_assign.size())
                {
                    // If we assign more element, we should have reallocate
                    if (assigned[index].copy_constructor_count() != 1u)
                    {
                        // LCOV_EXCL_START
                        all_copy_constructors_are_called = false;
                        break;
                        // LCOV_EXCL_STOP
                    }
                }
                else
                {
                    // We do not reallocate
                    if (index < elements_in_assigned.size())
                    {
                        // For element that are already here, we call operator=
                        if (assigned[index].copy_assign_count() != 1u)
                        {
                            // LCOV_EXCL_START
                            all_copy_assign_are_called = false;
                            break;
                            // LCOV_EXCL_STOP
                        }
                    }
                    else
                    {
                        // For new elements we call copy constructors
                        if (assigned[index].copy_constructor_count() != 1u)
                        {
                            // LCOV_EXCL_START
                            all_copy_constructors_are_called = false;
                            break;
                            // LCOV_EXCL_STOP
                        }
                    }
                }
            }

            return std::tuple {
                assigned.data() != nullptr,
                // Number of element should be equal to the number of element in the std::initializer_list
                assigned.count(),
                // Ensure we do no reallocate for less memory
                assigned.max_count(),
                all_values_are_copied,
                all_copy_constructors_are_called,
                all_copy_assign_are_called,
                assigned.allocator().allocation_count(),
                assigned.allocator().free_count()};
        };

        // Non constant
        {
            {const auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
}
}

// Test with extra
{
    const auto test = [](std::initializer_list<type> &&elements_in_assigned, const usize extra, std::initializer_list<type> elements_to_assign)
    {
        hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra);

        assigned = elements_to_assign;

        // Ensures we copy all values correctly
        bool all_values_are_copied = true;
        bool all_copy_constructors_are_called = true;
        bool all_copy_assign_are_called = true;
        for (usize index = 0; index < assigned.count(); index++)
        {

            // Ensure we correctly copied the value
            if (assigned[index].id() != (elements_to_assign.begin() + index)->id())
            {
                all_values_are_copied = false;
                break;
            }

            // Ensure we correctly call copy constructors
            if ((elements_in_assigned.size() + extra) < elements_to_assign.size())
            {
                // If we assign more element, we should have reallocate
                if (assigned[index].copy_constructor_count() != 1u)
                {
                    all_copy_constructors_are_called = false;
                    break;
                }
            }
            else
            {
                // We do not reallocate
                if (index < elements_in_assigned.size())
                {
                    // For element that are already here, we call operator=
                    if (assigned[index].copy_assign_count() != 1u)
                    {
                        all_copy_assign_are_called = false;
                        break;
                    }
                }
                else
                {
                    // For new elements we call copy constructors
                    if (assigned[index].copy_constructor_count() != 1u)
                    {
                        all_copy_constructors_are_called = false;
                        break;
                    }
                }
            }
        }

        return std::tuple {
            assigned.data() != nullptr,
            // Number of element should be equal to the number of element in the std::initializer_list
            assigned.count(),
            // Ensure we do no reallocate for less memory
            assigned.max_count(),
            all_values_are_copied,
            all_copy_constructors_are_called,
            all_copy_assign_are_called,
            assigned.allocator().allocation_count(),
            assigned.allocator().free_count()};
    };

    // Non constant
    {
        {const auto result = test({}, 0, {});
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1, {});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0, {});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1, {});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 0, {1, 2});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1, {1, 2});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 2u);
    GTEST_ASSERT_EQ(std::get<7>(result), 1u);
}
{
    const auto result = test({1, 2}, 0, {3, 4});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1, {3, 4});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 0, {4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1, {4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0, {3, 4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 2u);
    GTEST_ASSERT_EQ(std::get<7>(result), 1u);
}
{
    const auto result = test({1, 2}, 1, {3, 4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test({}, 0, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 0, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 0, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 0, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
}
}
}

GTEST_TEST(array, assign_std_initializer_list_of_non_bitwise_copy_assignable_different_type)
{

    using destination_type = hud_test::non_bitwise_copy_assignable_type_2;
    using source_type = hud_test::non_bitwise_copy_assignable_type;
    // Ensure we test with different types
    static_assert(!std::is_same_v<destination_type, source_type>);
    static_assert(!hud::is_bitwise_copy_assignable_v<destination_type, source_type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<destination_type> &&elements_in_assigned, std::initializer_list<source_type> elements_to_assign)
        {
            hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned);

            assigned = elements_to_assign;

            // Ensures we copy all values correctly
            bool all_values_are_copied = true;
            bool all_copy_constructors_are_called = true;
            bool all_copy_assign_are_called = true;
            for (usize index = 0; index < assigned.count(); index++)
            {

                // Ensure we correctly copied the value
                if (assigned[index].id() != (elements_to_assign.begin() + index)->id())
                {
                    all_values_are_copied = false;
                    break;
                }

                // Ensure we correctly call copy constructors
                if (elements_in_assigned.size() < elements_to_assign.size())
                {
                    // If we assign more element, we should have reallocate
                    if (assigned[index].copy_constructor_count() != 1u)
                    {
                        all_copy_constructors_are_called = false;
                        break;
                    }
                }
                else
                {
                    // We do not reallocate
                    if (index < elements_in_assigned.size())
                    {
                        // For element that are already here, we call operator=
                        if (assigned[index].copy_assign_count() != 1u)
                        {
                            all_copy_assign_are_called = false;
                            break;
                        }
                    }
                    else
                    {
                        // For new elements we call copy constructors
                        if (assigned[index].copy_constructor_count() != 1u)
                        {
                            all_copy_constructors_are_called = false;
                            break;
                        }
                    }
                }
            }

            return std::tuple {
                assigned.data() != nullptr,
                // Number of element should be equal to the number of element in the std::initializer_list
                assigned.count(),
                // Ensure we do no reallocate for less memory
                assigned.max_count(),
                all_values_are_copied,
                all_copy_constructors_are_called,
                all_copy_assign_are_called,
                assigned.allocator().allocation_count(),
                assigned.allocator().free_count()};
        };

        // Non constant
        {
            {const auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
}
}

// Test with extra
{
    const auto test = [](std::initializer_list<destination_type> &&elements_in_assigned, const usize extra, std::initializer_list<source_type> elements_to_assign)
    {
        hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned, extra);

        assigned = elements_to_assign;

        // Ensures we copy all values correctly
        bool all_values_are_copied = true;
        bool all_copy_constructors_are_called = true;
        bool all_copy_assign_are_called = true;
        for (usize index = 0; index < assigned.count(); index++)
        {

            // Ensure we correctly copied the value
            if (assigned[index].id() != (elements_to_assign.begin() + index)->id())
            {
                all_values_are_copied = false;
                break;
            }

            // Ensure we correctly call copy constructors
            if ((elements_in_assigned.size() + extra) < elements_to_assign.size())
            {
                // If we assign more element, we should have reallocate
                if (assigned[index].copy_constructor_count() != 1u)
                {
                    all_copy_constructors_are_called = false;
                    break;
                }
            }
            else
            {
                // We do not reallocate
                if (index < elements_in_assigned.size())
                {
                    // For element that are already here, we call operator=
                    if (assigned[index].copy_assign_count() != 1u)
                    {
                        all_copy_assign_are_called = false;
                        break;
                    }
                }
                else
                {
                    // For new elements we call copy constructors
                    if (assigned[index].copy_constructor_count() != 1u)
                    {
                        all_copy_constructors_are_called = false;
                        break;
                    }
                }
            }
        }

        return std::tuple {
            assigned.data() != nullptr,
            // Number of element should be equal to the number of element in the std::initializer_list
            assigned.count(),
            // Ensure we do no reallocate for less memory
            assigned.max_count(),
            all_values_are_copied,
            all_copy_constructors_are_called,
            all_copy_assign_are_called,
            assigned.allocator().allocation_count(),
            assigned.allocator().free_count()};
    };

    // Non constant
    {
        {const auto result = test({}, 0, {});
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1, {});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0, {});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1, {});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 0, {1, 2});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1, {1, 2});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 2u);
    GTEST_ASSERT_EQ(std::get<7>(result), 1u);
}
{
    const auto result = test({1, 2}, 0, {3, 4});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1, {3, 4});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 0, {4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1, {4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0, {3, 4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 2u);
    GTEST_ASSERT_EQ(std::get<7>(result), 1u);
}
{
    const auto result = test({1, 2}, 1, {3, 4, 5});
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test({}, 0, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 0, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 0, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 0, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
}
}
}

GTEST_TEST(array, assign_std_initializer_list_call_destructor_of_elements)
{

    // Test without extra
    {
        const auto test = [](const usize count_in_assigned, const usize count_to_assigned)
        {
            i32 *dtor_assigned_counter = nullptr;
            i32 **dtor_assigned_ptr_counter = nullptr;

            if (count_in_assigned > 0)
            {
                dtor_assigned_counter = hud::memory::allocate_array<i32>(count_in_assigned);
                hud::memory::set_zero(dtor_assigned_counter, count_in_assigned * sizeof(i32));
                dtor_assigned_ptr_counter = hud::memory::allocate_array<i32 *>(count_in_assigned);
                hud::memory::set_zero(dtor_assigned_ptr_counter, count_in_assigned * sizeof(i32 *));
                for (usize index = 0; index < count_in_assigned; index++)
                {
                    dtor_assigned_ptr_counter[index] = dtor_assigned_counter + index;
                }
            }
            hud_test::LeakArrayGuard guard_assigned_counter(dtor_assigned_counter, count_in_assigned);
            hud_test::LeakArrayGuard guard_assigned_ptr_counter(dtor_assigned_ptr_counter, count_in_assigned);

            i32 *dtor_to_assigned_counter = nullptr;
            i32 **dtor_to_assigned_ptr_counter = nullptr;

            if (count_to_assigned > 0)
            {
                dtor_to_assigned_counter = hud::memory::allocate_array<i32>(count_to_assigned);
                hud::memory::set_zero(dtor_to_assigned_counter, count_to_assigned * sizeof(i32));
                dtor_to_assigned_ptr_counter = hud::memory::allocate_array<i32 *>(count_to_assigned);
                hud::memory::set_zero(dtor_to_assigned_ptr_counter, count_to_assigned * sizeof(i32 *));
                for (usize index = 0; index < count_to_assigned; index++)
                {
                    dtor_to_assigned_ptr_counter[index] = dtor_to_assigned_counter + index;
                }
            }
            hud_test::LeakArrayGuard guard_to_assigned_counter(dtor_to_assigned_counter, count_to_assigned);
            hud_test::LeakArrayGuard guard_to_assigned_ptr_counter(dtor_to_assigned_ptr_counter, count_to_assigned);

            hud::array<hud_test::SetBoolToTrueWhenDestroyed, hud_test::array_allocator<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> assigned(dtor_assigned_ptr_counter, count_in_assigned);

            switch (count_to_assigned)
            {
                case 0:
                    assigned = std::initializer_list<i32 *> {};
                    break;
                case 2:
                    assigned = {dtor_to_assigned_ptr_counter[0], dtor_to_assigned_ptr_counter[1]};
                    break;
                case 3:
                    assigned = {dtor_to_assigned_ptr_counter[0], dtor_to_assigned_ptr_counter[1], dtor_to_assigned_ptr_counter[2]};
                    break;
            }

            bool all_destructors_are_called = true;
            if (count_to_assigned > count_in_assigned)
            {
                // If reallocation is done, all in assigned should be destroyed
                for (usize index = 0; index < count_in_assigned; index++)
                {
                    if (dtor_assigned_counter[index] != 1u)
                    {
                        all_destructors_are_called = false;
                        break;
                    }
                }
            }
            else if (count_to_assigned < count_in_assigned)
            {
                // Only element that are after count_to_assigned should be destroyed
                for (usize index = 0; index < count_in_assigned; index++)
                {
                    // We should not destroy element already present( They are assigned )
                    if (index < count_to_assigned)
                    {
                        if (dtor_assigned_counter[index] != 0u)
                        {
                            all_destructors_are_called = false;
                            break;
                        }
                    }
                    else
                    { // We should destroy element that are after count_to_assigned
                        if (dtor_assigned_counter[index] != 1u)
                        {
                            all_destructors_are_called = false;
                            break;
                        }
                    }
                }
            }
            else
            { // count_to_assigned == count_in_assigned
                // If we assigned same count of element, none should be destroyed
                for (usize index = 0; index < count_in_assigned; index++)
                {
                    if (dtor_assigned_counter[index] != 0u)
                    {
                        all_destructors_are_called = false;
                        break;
                    }
                }
            }

            // Ensure non of newly assigned are destroyed
            bool assigned_are_not_destroyed = true;
            for (usize index = 0; index < count_to_assigned; index++)
            {
                if (dtor_to_assigned_counter[index] != 0u)
                {
                    assigned_are_not_destroyed = false;
                    break;
                }
            }

            return std::tuple {
                all_destructors_are_called,
                assigned_are_not_destroyed,
                assigned.allocator().allocation_count(),
                assigned.allocator().free_count()};
        };

        // Non constant
        {
            {const auto result = test(0, 0);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        const auto result = test(2, 0);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        const auto result = test(0, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        const auto result = test(2, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        const auto result = test(3, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        const auto result = test(2, 3);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test(0, 0);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 0);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(3, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 3);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    }
}
}

// Test with extra
{
    const auto test = [](const usize count_in_assigned, const usize extra, const usize count_to_assigned)
    {
        i32 *dtor_assigned_counter = nullptr;
        i32 **dtor_assigned_ptr_counter = nullptr;

        if (count_in_assigned > 0)
        {
            dtor_assigned_counter = hud::memory::allocate_array<i32>(count_in_assigned);
            hud::memory::set_zero(dtor_assigned_counter, count_in_assigned * sizeof(i32));
            dtor_assigned_ptr_counter = hud::memory::allocate_array<i32 *>(count_in_assigned);
            hud::memory::set_zero(dtor_assigned_ptr_counter, count_in_assigned * sizeof(i32 *));
            for (usize index = 0; index < count_in_assigned; index++)
            {
                dtor_assigned_ptr_counter[index] = dtor_assigned_counter + index;
            }
        }
        hud_test::LeakArrayGuard guard_assigned_counter(dtor_assigned_counter, count_in_assigned);
        hud_test::LeakArrayGuard guard_assigned_ptr_counter(dtor_assigned_ptr_counter, count_in_assigned);

        i32 *dtor_to_assigned_counter = nullptr;
        i32 **dtor_to_assigned_ptr_counter = nullptr;

        if (count_to_assigned > 0)
        {
            dtor_to_assigned_counter = hud::memory::allocate_array<i32>(count_to_assigned);
            hud::memory::set_zero(dtor_to_assigned_counter, count_to_assigned * sizeof(i32));
            dtor_to_assigned_ptr_counter = hud::memory::allocate_array<i32 *>(count_to_assigned);
            hud::memory::set_zero(dtor_to_assigned_ptr_counter, count_to_assigned * sizeof(i32 *));
            for (usize index = 0; index < count_to_assigned; index++)
            {
                dtor_to_assigned_ptr_counter[index] = dtor_to_assigned_counter + index;
            }
        }
        hud_test::LeakArrayGuard guard_to_assigned_counter(dtor_to_assigned_counter, count_to_assigned);
        hud_test::LeakArrayGuard guard_to_assigned_ptr_counter(dtor_to_assigned_ptr_counter, count_to_assigned);

        hud::array<hud_test::SetBoolToTrueWhenDestroyed, hud_test::array_allocator<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> assigned(dtor_assigned_ptr_counter, count_in_assigned, extra);

        switch (count_to_assigned)
        {
            case 0:
                assigned = std::initializer_list<i32 *> {};
                break;
            case 2:
                assigned = {dtor_to_assigned_ptr_counter[0], dtor_to_assigned_ptr_counter[1]};
                break;
            case 3:
                assigned = {dtor_to_assigned_ptr_counter[0], dtor_to_assigned_ptr_counter[1], dtor_to_assigned_ptr_counter[2]};
                break;
        }

        bool all_destructors_are_called = true;
        if (count_to_assigned > count_in_assigned + extra)
        {
            // If reallocation is done, all in assigned should be destroyed
            for (usize index = 0; index < count_in_assigned; index++)
            {
                if (dtor_assigned_counter[index] != 1u)
                {
                    all_destructors_are_called = false;
                    break;
                }
            }
        }
        else if (count_to_assigned < count_in_assigned)
        {
            // Only element that are after count_to_assigned should be destroyed
            for (usize index = 0; index < count_in_assigned; index++)
            {
                // We should not destroy element already present( They are assigned )
                if (index < count_to_assigned)
                {
                    if (dtor_assigned_counter[index] != 0u)
                    {
                        all_destructors_are_called = false;
                        break;
                    }
                }
                else
                { // We should destroy element that are after count_to_assigned
                    if (dtor_assigned_counter[index] != 1u)
                    {
                        all_destructors_are_called = false;
                        break;
                    }
                }
            }
        }
        else
        { // count_to_assigned == count_in_assigned
            // If we assigned same count of element, none should be destroyed
            for (usize index = 0; index < count_in_assigned; index++)
            {
                if (dtor_assigned_counter[index] != 0u)
                {
                    all_destructors_are_called = false;
                    break;
                }
            }
        }

        // Ensure non of newly assigned are destroyed
        bool assigned_are_not_destroyed = true;
        for (usize index = 0; index < count_to_assigned; index++)
        {
            if (dtor_to_assigned_counter[index] != 0u)
            {
                assigned_are_not_destroyed = false;
                break;
            }
        }

        return std::tuple {
            all_destructors_are_called,
            assigned_are_not_destroyed,
            assigned.allocator().allocation_count(),
            assigned.allocator().free_count()};
    };

    // Non constant
    {
        {const auto result = test(0, 0, 0);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(0, 1, 0);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 0, 0);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 1, 0);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(0, 0, 2);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(0, 1, 2);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
}
{
    const auto result = test(2, 0, 2);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 1, 2);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(3, 0, 2);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(3, 1, 2);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 0, 3);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
}
{
    const auto result = test(2, 1, 3);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test(0, 0, 0);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 1, 0);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 0, 0);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 1, 0);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 0, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 1, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    }
    {
        constexpr auto result = test(2, 0, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 1, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(3, 0, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(3, 1, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 0, 3);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    }
    {
        constexpr auto result = test(2, 1, 3);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
}
}
}

GTEST_TEST(array, assign_array_of_bitwise_copy_assignable_same_type)
{

    using type = i32;

    static_assert(hud::is_bitwise_copy_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> &&elements_in_assigned, std::initializer_list<type> elements_to_assign)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned);
            hud::array<type, hud_test::array_allocator<alignof(type)>> to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensures we copy all values correctly
            [[maybe_unused]] bool all_values_are_copied = true;
            for (usize index = 0; index < assigned.count(); index++)
            {
                if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index)))
                {
                    all_values_are_copied = false;
                    break;
                }
            }

            return std::tuple {
                assigned.data() != nullptr,
                // Number of element should be equal to the number of element in the std::initializer_list
                assigned.count(),
                // Ensure we do no reallocate for less memory
                assigned.max_count(),
                assigned.allocator().allocation_count(),
                assigned.allocator().free_count()};
        };

        // Non constant
        {
            {const auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
}
}

// Test with extra
{
    const auto test = [](std::initializer_list<type> &&elements_in_assigned, const usize extra_in_assigned, std::initializer_list<type> elements_to_assign, const usize extra_to_assign)
    {
        hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra_in_assigned);
        hud::array<type, hud_test::array_allocator<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

        assigned = to_assign;

        // Ensures we copy all values correctly
        [[maybe_unused]] bool all_values_are_copied = true;
        for (usize index = 0; index < assigned.count(); index++)
        {
            if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index)))
            {
                all_values_are_copied = false;
                break;
            }
        }

        return std::tuple {
            assigned.data() != nullptr,
            // Number of element should be equal to the number of element in the std::initializer_list
            assigned.count(),
            // Ensure we do no reallocate for less memory
            assigned.max_count(),
            assigned.allocator().allocation_count(),
            assigned.allocator().free_count()};
    };

    // Non constant
    {
        {const auto result = test({}, 0u, {}, 0u);
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 0u, {}, 1u);
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}

{
    const auto result = test({1, 2}, 0u, {}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}

{
    const auto result = test({}, 0u, {1, 2}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 2u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({}, 0u, {1, 2}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 2u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}

{
    const auto result = test({1, 2}, 0u, {3, 4}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}

{
    const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}

{
    const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 2u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 2u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test({}, 0u, {}, 0u);
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 0u, {}, 1u);
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }

    {
        constexpr auto result = test({}, 0u, {1, 2}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({}, 0u, {1, 2}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
}
}
}

GTEST_TEST(array, assign_array_of_bitwise_copy_assignable_different_type)
{

    using source_type = i32;
    using destination_type = u32;
    // Ensure we test with different types
    static_assert(!std::is_same_v<destination_type, source_type>);
    static_assert(hud::is_bitwise_copy_assignable_v<destination_type, source_type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<destination_type> &&elements_in_assigned, std::initializer_list<source_type> elements_to_assign)
        {
            hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned);
            hud::array<source_type, hud_test::array_allocator<alignof(source_type)>> to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensures we copy all values correctly
            [[maybe_unused]] bool all_values_are_copied = true;
            for (usize index = 0; index < assigned.count(); index++)
            {
                if (assigned[index] != static_cast<destination_type>(*(elements_to_assign.begin() + index)))
                {
                    all_values_are_copied = false;
                    break;
                }
            }

            return std::tuple {
                assigned.data() != nullptr,
                // Number of element should be equal to the number of element in the std::initializer_list
                assigned.count(),
                // Ensure we do no reallocate for less memory
                assigned.max_count(),
                assigned.allocator().allocation_count(),
                assigned.allocator().free_count()};
        };

        // Non constant
        {
            {const auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
}
}

// Test with extra
{
    const auto test = [](std::initializer_list<destination_type> &&elements_in_assigned, const usize extra_in_assigned, std::initializer_list<source_type> elements_to_assign, const usize extra_to_assign)
    {
        hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned, extra_in_assigned);
        hud::array<source_type, hud_test::array_allocator<alignof(source_type)>> to_assign(elements_to_assign, extra_to_assign);

        assigned = to_assign;

        // Ensures we copy all values correctly
        [[maybe_unused]] bool all_values_are_copied = true;
        for (usize index = 0; index < assigned.count(); index++)
        {
            if (assigned[index] != static_cast<destination_type>(*(elements_to_assign.begin() + index)))
            {
                all_values_are_copied = false;
                break;
            }
        }

        return std::tuple {
            assigned.data() != nullptr,
            // Number of element should be equal to the number of element in the std::initializer_list
            assigned.count(),
            // Ensure we do no reallocate for less memory
            assigned.max_count(),
            assigned.allocator().allocation_count(),
            assigned.allocator().free_count()};
    };

    // Non constant
    {
        {const auto result = test({}, 0u, {}, 0u);
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 0u, {}, 1u);
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}

{
    const auto result = test({1, 2}, 0u, {}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}

{
    const auto result = test({}, 0u, {1, 2}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 2u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({}, 0u, {1, 2}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 2u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}

{
    const auto result = test({1, 2}, 0u, {3, 4}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}

{
    const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}

{
    const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 2u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 2u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test({}, 0u, {}, 0u);
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 0u, {}, 1u);
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }

    {
        constexpr auto result = test({}, 0u, {1, 2}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({}, 0u, {1, 2}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
}
}
}

GTEST_TEST(array, assign_array_of_non_bitwise_copy_assignable_same_type)
{

    using type = hud_test::non_bitwise_copy_assignable_type;
    static_assert(!hud::is_bitwise_copy_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<i32> &&elements_in_assigned, std::initializer_list<i32> elements_to_assign)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned);
            hud::array<type, hud_test::array_allocator<alignof(type)>> to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensures we copy all values correctly
            bool all_values_are_copied = true;
            bool all_copy_constructors_are_called = true;
            bool all_copy_assign_are_called = true;
            for (usize index = 0; index < assigned.count(); index++)
            {

                // Ensure we correctly copied the value
                if (assigned[index].id() != *(elements_to_assign.begin() + index))
                {
                    all_values_are_copied = false;
                    break;
                }

                // Ensure we correctly call copy constructors
                if (elements_in_assigned.size() < elements_to_assign.size())
                {
                    // If we assign more element, we should have reallocate
                    if (assigned[index].copy_constructor_count() != 1u)
                    {
                        all_copy_constructors_are_called = false;
                        break;
                    }
                }
                else
                {
                    // We do not reallocate
                    if (index < elements_in_assigned.size())
                    {
                        // For element that are already here, we call operator=
                        if (assigned[index].copy_assign_count() != 1u)
                        {
                            all_copy_assign_are_called = false;
                            break;
                        }
                    }
                    else
                    {
                        // For new elements we call copy constructors
                        if (assigned[index].copy_constructor_count() != 1u)
                        {
                            all_copy_constructors_are_called = false;
                            break;
                        }
                    }
                }
            }

            return std::tuple {
                assigned.data() != nullptr,
                // Number of element should be equal to the number of element in the std::initializer_list
                assigned.count(),
                // Ensure we do no reallocate for less memory
                assigned.max_count(),
                all_values_are_copied,
                all_copy_constructors_are_called,
                all_copy_assign_are_called,
                assigned.allocator().allocation_count(),
                assigned.allocator().free_count()};
        };

        // Non constant
        {
            {const auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
}
}

// Test with extra
{
    const auto test = [](std::initializer_list<i32> &&elements_in_assigned, const usize extra_in_assigned, std::initializer_list<i32> elements_to_assign, const usize extra_to_assign)
    {
        hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra_in_assigned);
        hud::array<type, hud_test::array_allocator<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

        assigned = to_assign;

        // Ensures we copy all values correctly
        bool all_values_are_copied = true;
        bool all_copy_constructors_are_called = true;
        bool all_copy_assign_are_called = true;
        for (usize index = 0; index < assigned.count(); index++)
        {

            // Ensure we correctly copied the value
            if (assigned[index].id() != *(elements_to_assign.begin() + index))
            {
                all_values_are_copied = false;
                break;
            }

            // Ensure we correctly call copy constructors
            if ((elements_in_assigned.size() + extra_in_assigned) < elements_to_assign.size())
            {
                // If we assign more element, we should have reallocate
                if (assigned[index].copy_constructor_count() != 1u)
                {
                    all_copy_constructors_are_called = false;
                    break;
                }
            }
            else
            {
                // We do not reallocate
                if (index < elements_in_assigned.size())
                {
                    // For element that are already here, we call operator=
                    if (assigned[index].copy_assign_count() != 1u)
                    {
                        all_copy_assign_are_called = false;
                        break;
                    }
                }
                else
                {
                    // For new elements we call copy constructors
                    if (assigned[index].copy_constructor_count() != 1u)
                    {
                        all_copy_constructors_are_called = false;
                        break;
                    }
                }
            }
        }

        return std::tuple {
            assigned.data() != nullptr,
            // Number of element should be equal to the number of element in the std::initializer_list
            assigned.count(),
            // Ensure we do no reallocate for less memory
            assigned.max_count(),
            all_values_are_copied,
            all_copy_constructors_are_called,
            all_copy_assign_are_called,
            assigned.allocator().allocation_count(),
            assigned.allocator().free_count()};
    };

    // Non constant
    {
        {const auto result = test({}, 0u, {}, 0u);
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1u, {}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 0u, {}, 1u);
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}

{
    const auto result = test({1, 2}, 0u, {}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}

{
    const auto result = test({}, 0u, {1, 2}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 2u);
    GTEST_ASSERT_EQ(std::get<7>(result), 1u);
}
{
    const auto result = test({}, 0u, {1, 2}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 2u);
    GTEST_ASSERT_EQ(std::get<7>(result), 1u);
}

{
    const auto result = test({1, 2}, 0u, {3, 4}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}

{
    const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}

{
    const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 2u);
    GTEST_ASSERT_EQ(std::get<7>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 2u);
    GTEST_ASSERT_EQ(std::get<7>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test({}, 0u, {}, 0u);
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 0u, {}, 1u);
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }

    {
        constexpr auto result = test({}, 0u, {1, 2}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({}, 0u, {1, 2}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
}
}
}

GTEST_TEST(array, assign_array_of_non_bitwise_copy_assignable_different_type)
{

    using destination_type = hud_test::non_bitwise_copy_assignable_type_2;
    using source_type = hud_test::non_bitwise_copy_assignable_type;
    // Ensure we test with different types
    static_assert(!std::is_same_v<destination_type, source_type>);
    static_assert(!hud::is_bitwise_copy_assignable_v<destination_type, source_type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<i32> &&elements_in_assigned, std::initializer_list<i32> elements_to_assign)
        {
            hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned);
            hud::array<source_type, hud_test::array_allocator<alignof(source_type)>> to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensures we copy all values correctly
            bool all_values_are_copied = true;
            bool all_copy_constructors_are_called = true;
            bool all_copy_assign_are_called = true;
            for (usize index = 0; index < assigned.count(); index++)
            {

                // Ensure we correctly copied the value
                if (assigned[index].id() != *(elements_to_assign.begin() + index))
                {
                    all_values_are_copied = false;
                    break;
                }

                // Ensure we correctly call copy constructors
                if (elements_in_assigned.size() < elements_to_assign.size())
                {
                    // If we assign more element, we should have reallocate
                    if (assigned[index].copy_constructor_count() != 1u)
                    {
                        all_copy_constructors_are_called = false;
                        break;
                    }
                }
                else
                {
                    // We do not reallocate
                    if (index < elements_in_assigned.size())
                    {
                        // For element that are already here, we call operator=
                        if (assigned[index].copy_assign_count() != 1u)
                        {
                            all_copy_assign_are_called = false;
                            break;
                        }
                    }
                    else
                    {
                        // For new elements we call copy constructors
                        if (assigned[index].copy_constructor_count() != 1u)
                        {
                            all_copy_constructors_are_called = false;
                            break;
                        }
                    }
                }
            }

            return std::tuple {
                assigned.data() != nullptr,
                // Number of element should be equal to the number of element in the std::initializer_list
                assigned.count(),
                // Ensure we do no reallocate for less memory
                assigned.max_count(),
                all_values_are_copied,
                all_copy_constructors_are_called,
                all_copy_assign_are_called,
                assigned.allocator().allocation_count(),
                assigned.allocator().free_count()};
        };

        // Non constant
        {
            {const auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
}
}

// Test with extra
{
    const auto test = [](std::initializer_list<i32> &&elements_in_assigned, const usize extra_in_assigned, std::initializer_list<i32> elements_to_assign, const usize extra_to_assign)
    {
        hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned, extra_in_assigned);
        hud::array<source_type, hud_test::array_allocator<alignof(source_type)>> to_assign(elements_to_assign, extra_to_assign);

        assigned = to_assign;

        // Ensures we copy all values correctly
        bool all_values_are_copied = true;
        bool all_copy_constructors_are_called = true;
        bool all_copy_assign_are_called = true;
        for (usize index = 0; index < assigned.count(); index++)
        {

            // Ensure we correctly copied the value
            if (assigned[index].id() != *(elements_to_assign.begin() + index))
            {
                all_values_are_copied = false;
                break;
            }

            // Ensure we correctly call copy constructors
            if ((elements_in_assigned.size() + extra_in_assigned) < elements_to_assign.size())
            {
                // If we assign more element, we should have reallocate
                if (assigned[index].copy_constructor_count() != 1u)
                {
                    all_copy_constructors_are_called = false;
                    break;
                }
            }
            else
            {
                // We do not reallocate
                if (index < elements_in_assigned.size())
                {
                    // For element that are already here, we call operator=
                    if (assigned[index].copy_assign_count() != 1u)
                    {
                        all_copy_assign_are_called = false;
                        break;
                    }
                }
                else
                {
                    // For new elements we call copy constructors
                    if (assigned[index].copy_constructor_count() != 1u)
                    {
                        all_copy_constructors_are_called = false;
                        break;
                    }
                }
            }
        }

        return std::tuple {
            assigned.data() != nullptr,
            // Number of element should be equal to the number of element in the std::initializer_list
            assigned.count(),
            // Ensure we do no reallocate for less memory
            assigned.max_count(),
            all_values_are_copied,
            all_copy_constructors_are_called,
            all_copy_assign_are_called,
            assigned.allocator().allocation_count(),
            assigned.allocator().free_count()};
    };

    // Non constant
    {
        {const auto result = test({}, 0u, {}, 0u);
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1u, {}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 0u, {}, 1u);
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}

{
    const auto result = test({1, 2}, 0u, {}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}

{
    const auto result = test({}, 0u, {1, 2}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 2u);
    GTEST_ASSERT_EQ(std::get<7>(result), 1u);
}
{
    const auto result = test({}, 0u, {1, 2}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 2u);
    GTEST_ASSERT_EQ(std::get<7>(result), 1u);
}

{
    const auto result = test({1, 2}, 0u, {3, 4}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}

{
    const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}

{
    const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 2u);
    GTEST_ASSERT_EQ(std::get<7>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 2u);
    GTEST_ASSERT_EQ(std::get<7>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_TRUE(std::get<3>(result));
    GTEST_ASSERT_TRUE(std::get<4>(result));
    GTEST_ASSERT_TRUE(std::get<5>(result));
    GTEST_ASSERT_EQ(std::get<6>(result), 1u);
    GTEST_ASSERT_EQ(std::get<7>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test({}, 0u, {}, 0u);
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 0u, {}, 1u);
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }

    {
        constexpr auto result = test({}, 0u, {1, 2}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({}, 0u, {1, 2}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 2u);
        GTEST_ASSERT_EQ(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_EQ(std::get<6>(result), 1u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
}
}
}

GTEST_TEST(array, assign_array_call_destructor_of_elements)
{

    // Test without extra
    {
        const auto test = [](const usize count_in_assigned, const usize count_to_assigned)
        {
            i32 *dtor_assigned_counter = nullptr;
            i32 **dtor_assigned_ptr_counter = nullptr;

            if (count_in_assigned > 0)
            {
                dtor_assigned_counter = hud::memory::allocate_array<i32>(count_in_assigned);
                hud::memory::set_zero(dtor_assigned_counter, count_in_assigned * sizeof(i32));
                dtor_assigned_ptr_counter = hud::memory::allocate_array<i32 *>(count_in_assigned);
                hud::memory::set_zero(dtor_assigned_ptr_counter, count_in_assigned * sizeof(i32 *));
                for (usize index = 0; index < count_in_assigned; index++)
                {
                    dtor_assigned_ptr_counter[index] = dtor_assigned_counter + index;
                }
            }
            hud_test::LeakArrayGuard guard_assigned_counter(dtor_assigned_counter, count_in_assigned);
            hud_test::LeakArrayGuard guard_assigned_ptr_counter(dtor_assigned_ptr_counter, count_in_assigned);

            i32 *dtor_to_assigned_counter = nullptr;
            i32 **dtor_to_assigned_ptr_counter = nullptr;

            if (count_to_assigned > 0)
            {
                dtor_to_assigned_counter = hud::memory::allocate_array<i32>(count_to_assigned);
                hud::memory::set_zero(dtor_to_assigned_counter, count_to_assigned * sizeof(i32));
                dtor_to_assigned_ptr_counter = hud::memory::allocate_array<i32 *>(count_to_assigned);
                hud::memory::set_zero(dtor_to_assigned_ptr_counter, count_to_assigned * sizeof(i32 *));
                for (usize index = 0; index < count_to_assigned; index++)
                {
                    dtor_to_assigned_ptr_counter[index] = dtor_to_assigned_counter + index;
                }
            }
            hud_test::LeakArrayGuard guard_to_assigned_counter(dtor_to_assigned_counter, count_to_assigned);
            hud_test::LeakArrayGuard guard_to_assigned_ptr_counter(dtor_to_assigned_ptr_counter, count_to_assigned);

            hud::array<hud_test::SetBoolToTrueWhenDestroyed, hud_test::array_allocator<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> assigned(dtor_assigned_ptr_counter, count_in_assigned);
            hud::array<hud_test::SetBoolToTrueWhenDestroyed, hud_test::array_allocator<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> to_assigned(dtor_to_assigned_ptr_counter, count_to_assigned);

            assigned = to_assigned;

            bool all_destructors_are_called = true;
            if (count_to_assigned > count_in_assigned)
            {
                // If reallocation is done, all in assigned should be destroyed
                for (usize index = 0; index < count_in_assigned; index++)
                {
                    if (dtor_assigned_counter[index] != 1u)
                    {
                        all_destructors_are_called = false;
                        break;
                    }
                }
            }
            else if (count_to_assigned < count_in_assigned)
            {
                // Only element that are after count_to_assigned should be destroyed
                for (usize index = 0; index < count_in_assigned; index++)
                {
                    // We should not destroy element already present( They are assigned )
                    if (index < count_to_assigned)
                    {
                        if (dtor_assigned_counter[index] != 0u)
                        {
                            all_destructors_are_called = false;
                            break;
                        }
                    }
                    else
                    { // We should destroy element that are after count_to_assigned
                        if (dtor_assigned_counter[index] != 1u)
                        {
                            all_destructors_are_called = false;
                            break;
                        }
                    }
                }
            }
            else
            { // count_to_assigned == count_in_assigned
                // If we assigned same count of element, none should be destroyed
                for (usize index = 0; index < count_in_assigned; index++)
                {
                    if (dtor_assigned_counter[index] != 0u)
                    {
                        all_destructors_are_called = false;
                        break;
                    }
                }
            }

            // Ensure non of newly assigned are destroyed
            bool assigned_are_not_destroyed = true;
            for (usize index = 0; index < count_to_assigned; index++)
            {
                if (dtor_to_assigned_counter[index] != 0u)
                {
                    assigned_are_not_destroyed = false;
                    break;
                }
            }

            return std::tuple {
                all_destructors_are_called,
                assigned_are_not_destroyed,
                assigned.allocator().allocation_count(),
                assigned.allocator().free_count()};
        };

        // Non constant
        {
            {const auto result = test(0, 0);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        const auto result = test(2, 0);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        const auto result = test(0, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        const auto result = test(2, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        const auto result = test(3, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        const auto result = test(2, 3);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test(0, 0);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 0);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(3, 2);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 3);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    }
}
}

// Test with extra
{
    const auto test = [](const usize count_in_assigned, const usize extra_in_assigned, const usize count_to_assign, const usize extra_to_assign)
    {
        i32 *dtor_assigned_counter = nullptr;
        i32 **dtor_assigned_ptr_counter = nullptr;

        if (count_in_assigned > 0)
        {
            dtor_assigned_counter = hud::memory::allocate_array<i32>(count_in_assigned);
            hud::memory::set_zero(dtor_assigned_counter, count_in_assigned * sizeof(i32));
            dtor_assigned_ptr_counter = hud::memory::allocate_array<i32 *>(count_in_assigned);
            hud::memory::set_zero(dtor_assigned_ptr_counter, count_in_assigned * sizeof(i32 *));
            for (usize index = 0; index < count_in_assigned; index++)
            {
                dtor_assigned_ptr_counter[index] = dtor_assigned_counter + index;
            }
        }
        hud_test::LeakArrayGuard guard_assigned_counter(dtor_assigned_counter, count_in_assigned);
        hud_test::LeakArrayGuard guard_assigned_ptr_counter(dtor_assigned_ptr_counter, count_in_assigned);

        i32 *dtor_to_assigned_counter = nullptr;
        i32 **dtor_to_assigned_ptr_counter = nullptr;

        if (count_to_assign > 0)
        {
            dtor_to_assigned_counter = hud::memory::allocate_array<i32>(count_to_assign);
            hud::memory::set_zero(dtor_to_assigned_counter, count_to_assign * sizeof(i32));
            dtor_to_assigned_ptr_counter = hud::memory::allocate_array<i32 *>(count_to_assign);
            hud::memory::set_zero(dtor_to_assigned_ptr_counter, count_to_assign * sizeof(i32 *));
            for (usize index = 0; index < count_to_assign; index++)
            {
                dtor_to_assigned_ptr_counter[index] = dtor_to_assigned_counter + index;
            }
        }
        hud_test::LeakArrayGuard guard_to_assigned_counter(dtor_to_assigned_counter, count_to_assign);
        hud_test::LeakArrayGuard guard_to_assigned_ptr_counter(dtor_to_assigned_ptr_counter, count_to_assign);

        hud::array<hud_test::SetBoolToTrueWhenDestroyed, hud_test::array_allocator<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> assigned(dtor_assigned_ptr_counter, count_in_assigned, extra_in_assigned);
        hud::array<hud_test::SetBoolToTrueWhenDestroyed, hud_test::array_allocator<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> to_assigned(dtor_to_assigned_ptr_counter, count_to_assign, extra_to_assign);

        assigned = to_assigned;

        bool all_destructors_are_called = true;
        if (count_to_assign > count_in_assigned + extra_in_assigned)
        {
            // If reallocation is done, all in assigned should be destroyed
            for (usize index = 0; index < count_in_assigned; index++)
            {
                if (dtor_assigned_counter[index] != 1u)
                {
                    all_destructors_are_called = false;
                    break;
                }
            }
        }
        else if (count_to_assign < count_in_assigned)
        {
            // Only element that are after count_to_assigned should be destroyed
            for (usize index = 0; index < count_in_assigned; index++)
            {
                // We should not destroy element already present( They are assigned )
                if (index < count_to_assign)
                {
                    if (dtor_assigned_counter[index] != 0u)
                    {
                        all_destructors_are_called = false;
                        break;
                    }
                }
                else
                { // We should destroy element that are after count_to_assigned
                    if (dtor_assigned_counter[index] != 1u)
                    {
                        all_destructors_are_called = false;
                        break;
                    }
                }
            }
        }
        else
        { // count_to_assigned == count_in_assigned
            // If we assigned same count of element, none should be destroyed
            for (usize index = 0; index < count_in_assigned; index++)
            {
                if (dtor_assigned_counter[index] != 0u)
                {
                    all_destructors_are_called = false;
                    break;
                }
            }
        }

        // Ensure non of newly assigned are destroyed
        bool assigned_are_not_destroyed = true;
        for (usize index = 0; index < count_to_assign; index++)
        {
            if (dtor_to_assigned_counter[index] != 0u)
            {
                assigned_are_not_destroyed = false;
                break;
            }
        }

        return std::tuple {
            all_destructors_are_called,
            assigned_are_not_destroyed,
            assigned.allocator().allocation_count(),
            assigned.allocator().free_count()};
    };

    // Non constant
    {
        {const auto result = test(0, 0u, 0, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(0, 1u, 0, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(0, 0u, 0, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(0, 1u, 0, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}

{
    const auto result = test(2, 0u, 0, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 1u, 0, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 0u, 0, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 1u, 0, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}

{
    const auto result = test(0, 0u, 2, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(0, 1u, 2, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
}
{
    const auto result = test(0, 0u, 2, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(0, 1u, 2, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
}

{
    const auto result = test(2, 0u, 2, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 1u, 2, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 0u, 2, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 1u, 2, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}

{
    const auto result = test(3, 0u, 2, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(3, 1u, 2, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(3, 0u, 2, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(3, 1u, 2, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}

{
    const auto result = test(2, 0u, 3, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
}
{
    const auto result = test(2, 1u, 3, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 0u, 3, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
}
{
    const auto result = test(2, 1u, 3, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test(0, 0u, 0, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 1u, 0, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 0u, 0, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 1u, 0, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }

    {
        constexpr auto result = test(2, 0u, 0, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 1u, 0, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 0u, 0, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 1u, 0, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }

    {
        constexpr auto result = test(0, 0u, 2, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 1u, 2, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    }
    {
        constexpr auto result = test(0, 0u, 2, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 1u, 2, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    }

    {
        constexpr auto result = test(2, 0u, 2, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 1u, 2, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 0u, 2, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 1u, 2, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }

    {
        constexpr auto result = test(3, 0u, 2, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(3, 1u, 2, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(3, 0u, 2, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(3, 1u, 2, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }

    {
        constexpr auto result = test(2, 0u, 3, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    }
    {
        constexpr auto result = test(2, 1u, 3, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 0u, 3, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    }
    {
        constexpr auto result = test(2, 1u, 3, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    }
}
}
}

GTEST_TEST(array, move_assign_array_of_bitwise_move_assignable_same_type)
{

    using type = i32;

    static_assert(hud::is_bitwise_move_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> &&elements_in_assigned, std::initializer_list<type> elements_to_assign)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned);
            hud::array<type, hud_test::array_allocator<alignof(type)>> to_assign(elements_to_assign);

            assigned = hud::move(to_assign);

            // Ensures we move all values correctly
            [[maybe_unused]] bool all_values_are_moved = true;
            for (usize index = 0; index < assigned.count(); index++)
            {
                if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index)))
                {
                    all_values_are_moved = false;
                    break;
                }
            }

            return std::tuple {
                assigned.data() != nullptr,
                // Number of element should be equal to the number of element in the std::initializer_list
                assigned.count(),
                // Ensure we do no reallocate for less memory
                assigned.max_count(),
                assigned.allocator().allocation_count(),
                assigned.allocator().free_count()};
        };

        // Non constant
        {
            {const auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        const auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        const auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        const auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test({}, {});
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, {1, 2});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, {4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4, 5});
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
}
}

// Test with extra
{
    const auto test = [](std::initializer_list<type> &&elements_in_assigned, const usize extra_in_assigned, std::initializer_list<type> elements_to_assign, const usize extra_to_assign)
    {
        hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra_in_assigned);
        hud::array<type, hud_test::array_allocator<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

        assigned = hud::move(to_assign);

        // Ensures we move all values correctly
        [[maybe_unused]] bool all_values_are_moved = true;
        for (usize index = 0; index < assigned.count(); index++)
        {
            if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index)))
            {
                all_values_are_moved = false;
                break;
            }
        }

        return std::tuple {
            assigned.data() != nullptr,
            // Number of element should be equal to the number of element in the std::initializer_list
            assigned.count(),
            // Ensure we do no reallocate for less memory
            assigned.max_count(),
            assigned.allocator().allocation_count(),
            assigned.allocator().free_count()};
    };

    // Non constant
    {
        {const auto result = test({}, 0u, {}, 0u);
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {}, 0u);
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({}, 0u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}

{
    const auto result = test({1, 2}, 0u, {}, 0u);
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {}, 0u);
    GTEST_ASSERT_FALSE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 0u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 0u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    GTEST_ASSERT_EQ(std::get<2>(result), 1u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}

{
    const auto result = test({}, 0u, {1, 2}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({}, 0u, {1, 2}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 0u);
    GTEST_ASSERT_EQ(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}

{
    const auto result = test({1, 2}, 0u, {3, 4}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}

{
    const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 2u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}

{
    const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 3u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    GTEST_ASSERT_EQ(std::get<2>(result), 4u);
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 1u);
}
}

// Constant
{
    {
        constexpr auto result = test({}, 0u, {}, 0u);
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 0u, {}, 1u);
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 0u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }

    {
        constexpr auto result = test({}, 0u, {1, 2}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({}, 0u, {1, 2}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 2u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 4u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }

    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 2u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 3u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    }
}
}
}

GTEST_TEST(array, move_assign_array_of_bitwise_move_assignable_different_type)
{

    using Type1 = i32;
    using Type2 = u32;
    // Ensure we test with different types
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(hud::is_bitwise_copy_assignable_v<Type2, Type1>);

    auto test_assign = [](std::initializer_list<Type1> &&elements_in_assigned, usize extra, std::initializer_list<Type2> elements_to_assign, usize extra_2)
    {
        hud::array<Type1, hud_test::array_allocator<alignof(Type1)>> assigned(elements_in_assigned, extra);
        hud::array<Type2, hud_test::array_allocator<alignof(Type2)>> to_assign(elements_to_assign, extra_2);

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0)
        {
            // Ensure we are allocating only one time
            GTEST_ASSERT_EQ(assigned.allocator().allocation_count(), 1u);
            GTEST_ASSERT_EQ(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }

        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0)
        {
            GTEST_ASSERT_NE(assigned.data(), nullptr);
        }
        else
        {
            GTEST_ASSERT_EQ(assigned.data(), nullptr);
        }
        GTEST_ASSERT_EQ(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        GTEST_ASSERT_EQ(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0)
        {
            GTEST_ASSERT_NE(assigned_buffer_address, to_assigned_buffer_address);
        }

        assigned = std::move(to_assign);

        // Number of element should be equal to the number of element in the std::initializer_list
        GTEST_ASSERT_EQ(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // Ensure we keep all allocated memory from the move pointer
        GTEST_ASSERT_EQ(assigned.max_count(), elements_to_assign.size() + extra_2);

        // Ensures we move all values correctly
        for (usize index = 0; index < assigned.count(); index++)
        {
            GTEST_ASSERT_EQ(assigned[index], static_cast<Type1>(*(elements_to_assign.begin() + index)));
        }

        // Allocation is not supposed to be done, the type is bitwise moveable, the array should just take the allocation buffer ( move the pointer to the allocation )
        // assigned array should free the buffer if he have one
        // Ensure we really stole the buffer and do not allocate
        GTEST_ASSERT_EQ(assigned.allocator().allocation_count(), previous_allocation_count);
        if ((elements_in_assigned.size() + extra) != 0)
        {
            GTEST_ASSERT_EQ(assigned.allocator().free_count(), previous_free_count + 1);
        }

        // Ensure we really stole the buffer by just moving the pointer
        GTEST_ASSERT_EQ(reinterpret_cast<const uptr>(assigned.data()), to_assigned_buffer_address);

        // Ensure that the moved array is set to empty
        GTEST_ASSERT_EQ(to_assign.count(), 0u);
        GTEST_ASSERT_EQ(to_assign.max_count(), 0u);
        GTEST_ASSERT_EQ(to_assign.data(), nullptr);

        // Ensure the moved array was not freed and only one allocation was done ( at initilisation of the array )
        if ((elements_to_assign.size() + extra_2) > 0)
        {
            GTEST_ASSERT_EQ(to_assign.allocator().allocation_count(), 1u);
        }
        GTEST_ASSERT_EQ(to_assign.allocator().free_count(), 0u);
    };

    for (usize extra = 0; extra < 5; extra++)
    {
        for (usize extra2 = 0; extra2 < 5; extra2++)
        {
            test_assign({}, extra, {}, extra2);
            test_assign({}, extra, {Type2 {0}, Type2 {1}, Type2 {2}, Type2 {3}}, extra2);
            test_assign({Type1 {10}, Type1 {20}}, extra, {Type2 {0}, Type2 {1}, Type2 {2}, Type2 {3}}, extra2);
            test_assign({Type1 {10}, Type1 {20}, Type1 {30}, Type1 {40}}, extra, {Type2 {0}, Type2 {1}, Type2 {2}, Type2 {3}}, extra2);
            test_assign({Type1 {10}, Type1 {20}, Type1 {30}, Type1 {40}, Type1 {50}}, extra, {Type2 {0}, Type2 {1}, Type2 {2}, Type2 {3}}, extra2);
            test_assign({Type1 {10}, Type1 {20}, Type1 {30}, Type1 {40}, Type1 {50}}, extra, {Type2 {0}, Type2 {1}}, extra2);
            test_assign({Type1 {10}, Type1 {20}, Type1 {30}, Type1 {40}, Type1 {50}}, extra, {}, extra2);
        }
    }
}

GTEST_TEST(array, move_assign_array_of_non_bitwise_move_assignable_same_type)
{

    using type = hud_test::NonBitwiseMoveAssignableType;
    static_assert(!hud::is_bitwise_move_assignable_v<type>);

    auto test_assign = [](std::initializer_list<type> &&elements_in_assigned, usize extra, std::initializer_list<type> elements_to_assign, usize extra_2)
    {
        // Ensure that all counters are set to 0
        for (const type &element : elements_in_assigned)
        {
            GTEST_ASSERT_EQ(element.move_assign_count(), 0u);
            GTEST_ASSERT_EQ(element.move_constructor_count(), 0u);
            GTEST_ASSERT_EQ(element.copy_constructor_count(), 0u);
        }
        for (const type &element : elements_to_assign)
        {
            GTEST_ASSERT_EQ(element.move_assign_count(), 0u);
            GTEST_ASSERT_EQ(element.move_constructor_count(), 0u);
            GTEST_ASSERT_EQ(element.copy_constructor_count(), 0u);
        }

        hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra);
        hud::array<type, hud_test::array_allocator<alignof(type)>> to_assign(elements_to_assign, extra_2);

        // Ensure assigned copy constructor is set to 1
        for (const type &element : assigned)
        {
            GTEST_ASSERT_EQ(element.move_assign_count(), 0u);
            GTEST_ASSERT_EQ(element.move_constructor_count(), 0u);
            GTEST_ASSERT_EQ(element.copy_constructor_count(), 1u);
        }
        for (const type &element : to_assign)
        {
            GTEST_ASSERT_EQ(element.move_assign_count(), 0u);
            GTEST_ASSERT_EQ(element.move_constructor_count(), 0u);
            GTEST_ASSERT_EQ(element.copy_constructor_count(), 1u);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0)
        {
            // Ensure we are allocating only one time
            GTEST_ASSERT_EQ(assigned.allocator().allocation_count(), 1u);
            GTEST_ASSERT_EQ(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }

        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0)
        {
            GTEST_ASSERT_NE(assigned.data(), nullptr);
        }
        else
        {
            GTEST_ASSERT_EQ(assigned.data(), nullptr);
        }
        GTEST_ASSERT_EQ(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        GTEST_ASSERT_EQ(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0)
        {
            GTEST_ASSERT_NE(assigned_buffer_address, to_assigned_buffer_address);
        }

        assigned = std::move(to_assign);

        // Number of element should be equal to the number of element in the std::initializer_list
        GTEST_ASSERT_EQ(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // Ensure we keep all allocated memory from the move pointer
        GTEST_ASSERT_EQ(assigned.max_count(), static_cast<usize>(std::max(elements_in_assigned.size() + extra, elements_to_assign.size())));

        // Ensures we move all values correctly
        for (usize index = 0; index < assigned.count(); index++)
        {
            GTEST_ASSERT_EQ(assigned[index].id(), (elements_to_assign.begin() + index)->id());
        }

        const bool should_reallocate = elements_to_assign.size() > (elements_in_assigned.size() + extra);

        // Allocation is supposed to reallocate if we assign more elements than was allocated
        if (should_reallocate)
        {
            GTEST_ASSERT_EQ(assigned.allocator().allocation_count(), previous_allocation_count + 1);
            if (previous_allocation_count > 0)
            {
                GTEST_ASSERT_EQ(assigned.allocator().free_count(), previous_free_count + 1);
            }
        }

        // Ensures we move all values correctly
        // If the a reallocation was done ( We must an array with a bigger capacity )
        // All elements are destroyed and the move constructor of all elements is called instead of move assignement
        if (should_reallocate)
        {
            for (usize index = 0; index < assigned.count(); index++)
            {
                const type &element = assigned[index];
                GTEST_ASSERT_EQ(element.id(), (elements_to_assign.begin() + index)->id());
                GTEST_ASSERT_EQ(element.move_assign_count(), 0u);
                GTEST_ASSERT_EQ(element.move_constructor_count(), 1u);
                GTEST_ASSERT_EQ(element.copy_constructor_count(), 1u);
            }
        }
        // If not reallocation was done, all element in current count should call move assign,
        // else all elements assign after the count of element before the assignement should be move construct
        else
        {
            for (usize index = 0; index < assigned.count(); index++)
            {
                const type &element = assigned[index];
                if (index < elements_in_assigned.size())
                {
                    GTEST_ASSERT_EQ(element.move_assign_count(), 1u);
                    GTEST_ASSERT_EQ(element.move_constructor_count(), 0u);
                    GTEST_ASSERT_EQ(element.copy_constructor_count(), 1u);
                }
                else
                {
                    GTEST_ASSERT_EQ(element.move_assign_count(), 0u);
                    GTEST_ASSERT_EQ(element.move_constructor_count(), 1u);
                    GTEST_ASSERT_EQ(element.copy_constructor_count(), 1u);
                }
            }
        }

        // Ensure that the moved array is set to empty
        GTEST_ASSERT_EQ(to_assign.count(), 0u);
        GTEST_ASSERT_EQ(to_assign.max_count(), 0u);
        GTEST_ASSERT_EQ(to_assign.data(), nullptr);

        // Ensure the moved array was freed and only one allocation was done ( at initilisation of the array )
        if ((elements_to_assign.size() + extra_2) > 0)
        {
            GTEST_ASSERT_EQ(to_assign.allocator().allocation_count(), 1u);
            GTEST_ASSERT_EQ(to_assign.allocator().free_count(), 1u);
        }
        else
        {
            GTEST_ASSERT_EQ(to_assign.allocator().allocation_count(), 0u);
            GTEST_ASSERT_EQ(to_assign.allocator().free_count(), 0u);
        }
    };

    for (usize extra = 0; extra < 5; extra++)
    {
        for (usize extra2 = 0; extra2 < 5; extra2++)
        {
            test_assign({}, extra, {}, extra2);
            test_assign({}, extra, {type {0}, type {1}, type {2}, type {3}}, extra2);
            test_assign({type {10}, type {20}}, extra, {type {0}, type {1}, type {2}, type {3}}, extra2);
            test_assign({type {10}, type {20}, type {30}, type {40}}, extra, {type {0}, type {1}, type {2}, type {3}}, extra2);
            test_assign({type {10}, type {20}, type {30}, type {40}, type {50}}, extra, {type {0}, type {1}, type {2}, type {3}}, extra2);
            test_assign({type {10}, type {20}, type {30}, type {40}, type {50}}, extra, {type {0}, type {1}}, extra2);
            test_assign({type {10}, type {20}, type {30}, type {40}, type {50}}, extra, {}, extra2);
        }
    }
}

GTEST_TEST(array, move_assign_array_of_non_bitwise_move_assignable_different_type)
{

    using Type1 = hud_test::NonBitwiseMoveAssignableType2;
    using Type2 = hud_test::NonBitwiseMoveAssignableType;
    // Ensure we test with different types
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(!hud::is_bitwise_copy_assignable_v<Type2, Type1>);

    auto test_assign = [](std::initializer_list<Type1> &&elements_in_assigned, usize extra, std::initializer_list<Type2> elements_to_assign, usize extra_2)
    {
        // Ensure that all counters are set to 0
        for (const Type1 &element : elements_in_assigned)
        {
            GTEST_ASSERT_EQ(element.move_assign_count(), 0u);
            GTEST_ASSERT_EQ(element.move_constructor_count(), 0u);
            GTEST_ASSERT_EQ(element.copy_constructor_count(), 0u);
        }
        for (const Type2 &element : elements_to_assign)
        {
            GTEST_ASSERT_EQ(element.move_assign_count(), 0u);
            GTEST_ASSERT_EQ(element.move_constructor_count(), 0u);
            GTEST_ASSERT_EQ(element.copy_constructor_count(), 0u);
        }

        hud::array<Type1, hud_test::array_allocator<alignof(Type1)>> assigned(elements_in_assigned, extra);
        hud::array<Type2, hud_test::array_allocator<alignof(Type2)>> to_assign(elements_to_assign, extra_2);

        // Ensure assigned copy constructor is set to 1
        for (const Type1 &element : assigned)
        {
            GTEST_ASSERT_EQ(element.move_assign_count(), 0u);
            GTEST_ASSERT_EQ(element.move_constructor_count(), 0u);
            GTEST_ASSERT_EQ(element.copy_constructor_count(), 1u);
        }
        for (const Type2 &element : to_assign)
        {
            GTEST_ASSERT_EQ(element.move_assign_count(), 0u);
            GTEST_ASSERT_EQ(element.move_constructor_count(), 0u);
            GTEST_ASSERT_EQ(element.copy_constructor_count(), 1u);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0)
        {
            // Ensure we are allocating only one time
            GTEST_ASSERT_EQ(assigned.allocator().allocation_count(), 1u);
            GTEST_ASSERT_EQ(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }

        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0)
        {
            GTEST_ASSERT_NE(assigned.data(), nullptr);
        }
        else
        {
            GTEST_ASSERT_EQ(assigned.data(), nullptr);
        }
        GTEST_ASSERT_EQ(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        GTEST_ASSERT_EQ(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0)
        {
            GTEST_ASSERT_NE(assigned_buffer_address, to_assigned_buffer_address);
        }

        assigned = std::move(to_assign);

        // Number of element should be equal to the number of element in the std::initializer_list
        GTEST_ASSERT_EQ(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // Ensure we keep all allocated memory from the move pointer
        GTEST_ASSERT_EQ(assigned.max_count(), static_cast<usize>(std::max(elements_in_assigned.size() + extra, elements_to_assign.size())));

        // Ensures we move all values correctly
        for (usize index = 0; index < assigned.count(); index++)
        {
            GTEST_ASSERT_EQ(assigned[index].id(), (elements_to_assign.begin() + index)->id());
        }

        const bool should_reallocate = elements_to_assign.size() > (elements_in_assigned.size() + extra);

        // Allocation is supposed to reallocate if we assign more elements than was allocated
        if (should_reallocate)
        {
            GTEST_ASSERT_EQ(assigned.allocator().allocation_count(), previous_allocation_count + 1);
            if (previous_allocation_count > 0)
            {
                GTEST_ASSERT_EQ(assigned.allocator().free_count(), previous_free_count + 1);
            }
        }

        // Ensures we move all values correctly
        // If the a reallocation was done ( We must an array with a bigger capacity )
        // All elements are destroyed and the move constructor of all elements is called instead of move assignement
        if (should_reallocate)
        {
            for (usize index = 0; index < assigned.count(); index++)
            {
                const Type1 &element = assigned[index];
                GTEST_ASSERT_EQ(element.id(), (elements_to_assign.begin() + index)->id());
                GTEST_ASSERT_EQ(element.move_assign_count(), 0u);
                GTEST_ASSERT_EQ(element.move_constructor_count(), 1u);
                GTEST_ASSERT_EQ(element.copy_constructor_count(), 1u);
            }
        }
        // If not reallocation was done, all element in current count should call move assign,
        // else all elements assign after the count of element before the assignement should be move construct
        else
        {
            for (usize index = 0; index < assigned.count(); index++)
            {
                const Type1 &element = assigned[index];
                if (index < elements_in_assigned.size())
                {
                    GTEST_ASSERT_EQ(element.move_assign_count(), 1u);
                    GTEST_ASSERT_EQ(element.move_constructor_count(), 0u);
                    GTEST_ASSERT_EQ(element.copy_constructor_count(), 1u);
                }
                else
                {
                    GTEST_ASSERT_EQ(element.move_assign_count(), 0u);
                    GTEST_ASSERT_EQ(element.move_constructor_count(), 1u);
                    GTEST_ASSERT_EQ(element.copy_constructor_count(), 1u);
                }
            }
        }

        // Ensure that the moved array is set to empty
        GTEST_ASSERT_EQ(to_assign.count(), 0u);
        GTEST_ASSERT_EQ(to_assign.max_count(), 0u);
        GTEST_ASSERT_EQ(to_assign.data(), nullptr);

        // Ensure the moved array was freed and only one allocation was done ( at initilisation of the array )
        if ((elements_to_assign.size() + extra_2) > 0)
        {
            GTEST_ASSERT_EQ(to_assign.allocator().allocation_count(), 1u);
            GTEST_ASSERT_EQ(to_assign.allocator().free_count(), 1u);
        }
        else
        {
            GTEST_ASSERT_EQ(to_assign.allocator().allocation_count(), 0u);
            GTEST_ASSERT_EQ(to_assign.allocator().free_count(), 0u);
        }
    };

    for (usize extra = 0; extra < 5; extra++)
    {
        for (usize extra2 = 0; extra2 < 5; extra2++)
        {
            test_assign({}, extra, {}, extra2);
            test_assign({}, extra, {Type2 {0}, Type2 {1}, Type2 {2}, Type2 {3}}, extra2);
            test_assign({Type1 {10}, Type1 {20}}, extra, {Type2 {0}, Type2 {1}, Type2 {2}, Type2 {3}}, extra2);
            test_assign({Type1 {10}, Type1 {20}, Type1 {30}, Type1 {40}}, extra, {Type2 {0}, Type2 {1}, Type2 {2}, Type2 {3}}, extra2);
            test_assign({Type1 {10}, Type1 {20}, Type1 {30}, Type1 {40}, Type1 {50}}, extra, {Type2 {0}, Type2 {1}, Type2 {2}, Type2 {3}}, extra2);
            test_assign({Type1 {10}, Type1 {20}, Type1 {30}, Type1 {40}, Type1 {50}}, extra, {Type2 {0}, Type2 {1}}, extra2);
            test_assign({Type1 {10}, Type1 {20}, Type1 {30}, Type1 {40}, Type1 {50}}, extra, {}, extra2);
        }
    }
}

GTEST_TEST(array, move_assign_array_call_destructor_of_elements)
{

    auto test_assign = [](std::initializer_list<i32 *> &&elements_in_assigned, usize extra, std::initializer_list<i32 *> elements_to_assign)
    {
        // Ensure that all destructors is not called
        for (i32 *destructor_counter : elements_in_assigned)
        {
            GTEST_ASSERT_NE(destructor_counter, nullptr);
            *destructor_counter = 0;
        }
        for (i32 *destructor_counter : elements_to_assign)
        {
            GTEST_ASSERT_NE(destructor_counter, nullptr);
            *destructor_counter = 0;
        }

        hud::array<hud_test::SetBoolToTrueWhenDestroyed, hud_test::array_allocator<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> assigned(elements_in_assigned, extra);
        hud::array<hud_test::SetBoolToTrueWhenDestroyed, hud_test::array_allocator<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> to_assign(elements_to_assign);

        // Ensure assigned copy constructor is set to 1
        for (const hud_test::SetBoolToTrueWhenDestroyed &element : assigned)
        {
            GTEST_ASSERT_NE(element.ptr(), nullptr);
            GTEST_ASSERT_EQ(*element.ptr(), 0);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0)
        {
            // Ensure we are allocating only one time
            GTEST_ASSERT_EQ(assigned.allocator().allocation_count(), 1u);
            GTEST_ASSERT_EQ(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }

        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0)
        {
            GTEST_ASSERT_NE(assigned.data(), nullptr);
        }
        else
        {
            GTEST_ASSERT_EQ(assigned.data(), nullptr);
        }
        GTEST_ASSERT_EQ(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        GTEST_ASSERT_EQ(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));

        assigned = std::move(to_assign);

        // Allocation should never be released if already done
        // The only case we have no allocation at all is when assigning empty std::initializer_list to empty array
        if ((elements_in_assigned.size() + extra) == 0 && elements_to_assign.size() == 0)
        {
            GTEST_ASSERT_EQ(assigned.data(), nullptr);
        }
        else
        {
            GTEST_ASSERT_NE(assigned.data(), nullptr);
        }

        // Number of element should be equal to the number of element in the std::initializer_list
        GTEST_ASSERT_EQ(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // Ensure we do no reallocate for less memory
        GTEST_ASSERT_EQ(assigned.max_count(), static_cast<usize>(std::max((elements_in_assigned.size() + extra), elements_to_assign.size())));

        // Allocation is supposed to reallocate if we assign more elements than was allocated
        const bool should_reallocate = elements_to_assign.size() > (elements_in_assigned.size() + extra);
        if (should_reallocate)
        {
            GTEST_ASSERT_EQ(assigned.allocator().allocation_count(), previous_allocation_count + 1);
            if (previous_allocation_count > 0)
            {
                GTEST_ASSERT_EQ(assigned.allocator().free_count(), previous_free_count + 1);
            }
        }

        // Ensure we called destructors
        // If the a reallocation was done,
        // All elements are destroyed
        if (should_reallocate)
        {
            for (const i32 *element : elements_in_assigned)
            {
                GTEST_ASSERT_EQ(*element, 1);
            }
        }
        // If no reallocation was done, all elements in current count should not be destroyed
        else
        {
            for (usize index = 0; index < elements_in_assigned.size(); index++)
            {
                if (index < elements_to_assign.size())
                {
                    GTEST_ASSERT_EQ(*(*(elements_in_assigned.begin() + index)), 0);
                }
                else
                {
                    GTEST_ASSERT_EQ(*(*(elements_in_assigned.begin() + index)), 1);
                }
            }
        }

        // Ensure the copied elements are not destroyed
        for (usize index = 0; index < elements_to_assign.size(); index++)
        {
            GTEST_ASSERT_EQ(*(*(elements_to_assign.begin() + index)), 0);
        }
    };

    i32 assigned[5];
    i32 to_assign[5];

    for (usize extra = 0; extra < 5; extra++)
    {
        test_assign({}, extra, {});
        test_assign({}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {&to_assign[0], &to_assign[1]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {});
    }
}