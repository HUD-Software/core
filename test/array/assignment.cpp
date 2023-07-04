#include <core/containers/array.h>
#include "../misc/allocators.h"
#include "../misc/leak_guard.h"

GTEST_TEST(array, assign_std_initializer_list_of_bitwise_copy_assignable_same_type)
{

    using type = i32;

    static_assert(hud::is_bitwise_copy_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, std::initializer_list<type> elements_to_assign)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned);

            assigned = elements_to_assign;

            // Ensures we copy all values correctly
            [[maybe_unused]] bool all_values_are_copied = true;
            for (usize index = 0; index < assigned.count(); index++)
            {
                // LCOV_EXCL_START
                if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index)))
                {
                    all_values_are_copied = false;
                    break;
                }
                // LCOV_EXCL_STOP
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
        hud_assert_false(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        const auto result = test({}, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2, 3}, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 2u);
        hud_assert_eq(std::get<4>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test({}, {});
        hud_assert_false(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 2u);
        hud_assert_eq(std::get<4>(result), 1u);
    }
}
}

// Test with extra
{
    const auto test = [](std::initializer_list<type> elements_in_assigned, const usize extra, std::initializer_list<type> elements_to_assign)
    {
        hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra);

        assigned = elements_to_assign;

        // Ensures we copy all values correctly
        [[maybe_unused]] bool all_values_are_copied = true;
        for (usize index = 0; index < assigned.count(); index++)
        {
            // LCOV_EXCL_START
            if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index)))
            {
                all_values_are_copied = false;
                break;
            }
            // LCOV_EXCL_STOP
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
    hud_assert_false(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 0u);
    hud_assert_eq(std::get<3>(result), 0u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 1u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 0u, {1, 2});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_eq(std::get<3>(result), 2u);
    hud_assert_eq(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 0u, {4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 4u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 3u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_eq(std::get<3>(result), 2u);
    hud_assert_eq(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 3u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test({}, 0u, {});
        hud_assert_false(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 0u, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 2u);
        hud_assert_eq(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 4u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 2u);
        hud_assert_eq(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
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
                // LCOV_EXCL_START
                if (assigned[index] != static_cast<destination_type>(*(elements_to_assign.begin() + index)))
                {
                    all_values_are_copied = false;
                    break;
                }
                // LCOV_EXCL_STOP
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
        hud_assert_false(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        const auto result = test({}, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2, 3}, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 2u);
        hud_assert_eq(std::get<4>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test({}, {});
        hud_assert_false(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 2u);
        hud_assert_eq(std::get<4>(result), 1u);
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
            // LCOV_EXCL_START
            if (assigned[index] != static_cast<destination_type>(*(elements_to_assign.begin() + index)))
            {
                all_values_are_copied = false;
                break;
            }
            // LCOV_EXCL_STOP
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
    hud_assert_false(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 0u);
    hud_assert_eq(std::get<3>(result), 0u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 1u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 0u, {1, 2});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({}, 1u, {1, 2});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_eq(std::get<3>(result), 2u);
    hud_assert_eq(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 0u, {4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1u, {4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 4u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
{
    const auto result = test({1, 2}, 0u, {3, 4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 3u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_eq(std::get<3>(result), 2u);
    hud_assert_eq(std::get<4>(result), 1u);
}
{
    const auto result = test({1, 2}, 1u, {3, 4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 3u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_eq(std::get<3>(result), 1u);
    hud_assert_eq(std::get<4>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test({}, 0u, {});
        hud_assert_false(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 0u, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1u, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 2u);
        hud_assert_eq(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 0u, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1u, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 4u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0u, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 2u);
        hud_assert_eq(std::get<4>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1u, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
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
        const auto test = [](std::initializer_list<type> elements_in_assigned, std::initializer_list<type> elements_to_assign)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned);

            assigned = elements_to_assign;

            // Ensures we copy all values correctly
            bool all_values_are_copied = true;
            bool all_copy_constructors_are_called = true;
            bool all_copy_assign_are_called = true;
            for (usize index = 0; index < assigned.count(); index++)
            {
                // LCOV_EXCL_START
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
                // LCOV_EXCL_STOP
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
        hud_assert_false(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        const auto result = test({}, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2, 3}, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 2u);
        hud_assert_eq(std::get<7>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test({}, {});
        hud_assert_false(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 2u);
        hud_assert_eq(std::get<7>(result), 1u);
    }
}
}

// Test with extra
{
    const auto test = [](std::initializer_list<type> elements_in_assigned, const usize extra, std::initializer_list<type> elements_to_assign)
    {
        hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra);

        assigned = elements_to_assign;

        // Ensures we copy all values correctly
        bool all_values_are_copied = true;
        bool all_copy_constructors_are_called = true;
        bool all_copy_assign_are_called = true;
        for (usize index = 0; index < assigned.count(); index++)
        {
            // LCOV_EXCL_START
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
            // LCOV_EXCL_STOP
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
    hud_assert_false(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 0u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 0u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1, {});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 1u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0, {});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1, {});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 0, {1, 2});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1, {1, 2});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 2u);
    hud_assert_eq(std::get<7>(result), 1u);
}
{
    const auto result = test({1, 2}, 0, {3, 4});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1, {3, 4});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 0, {4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1, {4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 4u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0, {3, 4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 3u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 2u);
    hud_assert_eq(std::get<7>(result), 1u);
}
{
    const auto result = test({1, 2}, 1, {3, 4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 3u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test({}, 0, {});
        hud_assert_false(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 0, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 2u);
        hud_assert_eq(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 0, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 0, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 4u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 2u);
        hud_assert_eq(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
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
                // LCOV_EXCL_START
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
                // LCOV_EXCL_STOP
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
        hud_assert_false(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        const auto result = test({}, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2, 3}, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        const auto result = test({1, 2}, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 2u);
        hud_assert_eq(std::get<7>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test({}, {});
        hud_assert_false(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 2u);
        hud_assert_eq(std::get<7>(result), 1u);
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
            // LCOV_EXCL_START
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
            // LCOV_EXCL_STOP
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
    hud_assert_false(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 0u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 0u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1, {});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 1u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0, {});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1, {});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 0u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 0, {1, 2});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({}, 1, {1, 2});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 2u);
    hud_assert_eq(std::get<7>(result), 1u);
}
{
    const auto result = test({1, 2}, 0, {3, 4});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 1, {3, 4});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 0, {4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2, 3}, 1, {4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 2u);
    hud_assert_eq(std::get<2>(result), 4u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
{
    const auto result = test({1, 2}, 0, {3, 4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 3u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 2u);
    hud_assert_eq(std::get<7>(result), 1u);
}
{
    const auto result = test({1, 2}, 1, {3, 4, 5});
    hud_assert_true(std::get<0>(result));
    hud_assert_eq(std::get<1>(result), 3u);
    hud_assert_eq(std::get<2>(result), 3u);
    hud_assert_true(std::get<3>(result));
    hud_assert_true(std::get<4>(result));
    hud_assert_true(std::get<5>(result));
    hud_assert_eq(std::get<6>(result), 1u);
    hud_assert_eq(std::get<7>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test({}, 0, {});
        hud_assert_false(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1, {});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 0, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({}, 1, {1, 2});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 2u);
        hud_assert_eq(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 0, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 1, {3, 4});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 0, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2, 3}, 1, {4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 2u);
        hud_assert_eq(std::get<2>(result), 4u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
    {
        constexpr auto result = test({1, 2}, 0, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 2u);
        hud_assert_eq(std::get<7>(result), 1u);
    }
    {
        constexpr auto result = test({1, 2}, 1, {3, 4, 5});
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 3u);
        hud_assert_eq(std::get<2>(result), 3u);
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
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

            // LCOV_EXCL_START
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
                // If we assigned same count of elements, none should be destroyed
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
            // LCOV_EXCL_STOP

            return std::tuple {
                all_destructors_are_called,
                assigned_are_not_destroyed,
                assigned.allocator().allocation_count(),
                assigned.allocator().free_count()};
        };

        // Non constant
        {
            {const auto result = test(0, 0);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        const auto result = test(2, 0);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        const auto result = test(0, 2);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        const auto result = test(2, 2);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        const auto result = test(3, 2);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        const auto result = test(2, 3);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
    }
}

// Constant
{
    {
        constexpr auto result = test(0, 0);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 0);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 2);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 2);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(3, 2);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 3);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
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

        // LCOV_EXCL_START
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
            // If we assigned same count of elements, none should be destroyed
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
        // LCOV_EXCL_STOP

        return std::tuple {
            all_destructors_are_called,
            assigned_are_not_destroyed,
            assigned.allocator().allocation_count(),
            assigned.allocator().free_count()};
    };

    // Non constant
    {
        {const auto result = test(0, 0, 0);
    hud_assert_true(std::get<0>(result));
    hud_assert_true(std::get<1>(result));
    hud_assert_eq(std::get<2>(result), 0u);
    hud_assert_eq(std::get<3>(result), 0u);
}
{
    const auto result = test(0, 1, 0);
    hud_assert_true(std::get<0>(result));
    hud_assert_true(std::get<1>(result));
    hud_assert_eq(std::get<2>(result), 1u);
    hud_assert_eq(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 0, 0);
    hud_assert_true(std::get<0>(result));
    hud_assert_true(std::get<1>(result));
    hud_assert_eq(std::get<2>(result), 1u);
    hud_assert_eq(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 1, 0);
    hud_assert_true(std::get<0>(result));
    hud_assert_true(std::get<1>(result));
    hud_assert_eq(std::get<2>(result), 1u);
    hud_assert_eq(std::get<3>(result), 0u);
}
{
    const auto result = test(0, 0, 2);
    hud_assert_true(std::get<0>(result));
    hud_assert_true(std::get<1>(result));
    hud_assert_eq(std::get<2>(result), 1u);
    hud_assert_eq(std::get<3>(result), 0u);
}
{
    const auto result = test(0, 1, 2);
    hud_assert_true(std::get<0>(result));
    hud_assert_true(std::get<1>(result));
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_eq(std::get<3>(result), 1u);
}
{
    const auto result = test(2, 0, 2);
    hud_assert_true(std::get<0>(result));
    hud_assert_true(std::get<1>(result));
    hud_assert_eq(std::get<2>(result), 1u);
    hud_assert_eq(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 1, 2);
    hud_assert_true(std::get<0>(result));
    hud_assert_true(std::get<1>(result));
    hud_assert_eq(std::get<2>(result), 1u);
    hud_assert_eq(std::get<3>(result), 0u);
}
{
    const auto result = test(3, 0, 2);
    hud_assert_true(std::get<0>(result));
    hud_assert_true(std::get<1>(result));
    hud_assert_eq(std::get<2>(result), 1u);
    hud_assert_eq(std::get<3>(result), 0u);
}
{
    const auto result = test(3, 1, 2);
    hud_assert_true(std::get<0>(result));
    hud_assert_true(std::get<1>(result));
    hud_assert_eq(std::get<2>(result), 1u);
    hud_assert_eq(std::get<3>(result), 0u);
}
{
    const auto result = test(2, 0, 3);
    hud_assert_true(std::get<0>(result));
    hud_assert_true(std::get<1>(result));
    hud_assert_eq(std::get<2>(result), 2u);
    hud_assert_eq(std::get<3>(result), 1u);
}
{
    const auto result = test(2, 1, 3);
    hud_assert_true(std::get<0>(result));
    hud_assert_true(std::get<1>(result));
    hud_assert_eq(std::get<2>(result), 1u);
    hud_assert_eq(std::get<3>(result), 0u);
}
}

// Constant
{
    {
        constexpr auto result = test(0, 0, 0);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 1, 0);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 0, 0);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 1, 0);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 0, 2);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(0, 1, 2);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
    }
    {
        constexpr auto result = test(2, 0, 2);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 1, 2);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(3, 0, 2);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(3, 1, 2);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
    {
        constexpr auto result = test(2, 0, 3);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 2u);
        hud_assert_eq(std::get<3>(result), 1u);
    }
    {
        constexpr auto result = test(2, 1, 3);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
    }
}
}
}
