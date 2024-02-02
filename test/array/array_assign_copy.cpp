#include <core/containers/array.h>
#include "../misc/array_allocators.h"
#include "../misc/leak_guard.h"

// void assign(const array &other, const usize min_slack = 0)
GTEST_TEST(array, copy_assign_array_of_bitwise_copy_assignable_same_type_same_allocator_no_extra)
{

    using type = i32;

    static_assert(hud::is_bitwise_copy_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, std::initializer_list<type> elements_to_assign)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned);
            hud::array<type, hud_test::array_allocator<alignof(type)>> to_assign(elements_to_assign);

            assigned.assign(to_assign);

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
                assigned.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
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
        const auto test = [](std::initializer_list<type> elements_in_assigned, const usize extra_in_assigned, std::initializer_list<type> elements_to_assign, const usize extra_to_assign)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra_in_assigned);
            hud::array<type, hud_test::array_allocator<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

            assigned.assign(to_assign);

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
                assigned.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0u, {}, 0u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 0u, {}, 1u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
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
                constexpr auto result = test({}, 0u, {}, 0u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 0u, {}, 1u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
        }
    }
}

// void assign(const array &other, const usize min_slack = 0)
GTEST_TEST(array, copy_assign_array_of_bitwise_copy_assignable_same_type_same_allocator_with_extra)
{
    using type = i32;

    static_assert(hud::is_bitwise_copy_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, std::initializer_list<type> elements_to_assign)
        {
            constexpr const usize extra_assign = 1;
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned);
            hud::array<type, hud_test::array_allocator<alignof(type)>> to_assign(elements_to_assign);

            assigned.assign(to_assign, extra_assign);

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
                assigned.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
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
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
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
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
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
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
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
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, const usize extra_in_assigned, std::initializer_list<type> elements_to_assign, const usize extra_to_assign)
        {
            constexpr const usize extra_assign = 1;
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra_in_assigned);
            hud::array<type, hud_test::array_allocator<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

            assigned.assign(to_assign, extra_assign);

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
                assigned.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
        }
    }
}

// void assign(const array<u_type_t, u_allocator_t> &other, const usize min_slack = 0)
GTEST_TEST(array, copy_assign_array_of_bitwise_copy_assignable_same_type_different_allocator_no_extra)
{
    using type = i32;

    static_assert(hud::is_bitwise_copy_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, std::initializer_list<type> elements_to_assign)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned);
            hud::array<type, hud_test::array_allocator_2<alignof(type)>> to_assign(elements_to_assign);

            assigned.assign(to_assign);

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
                assigned.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
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
        const auto test = [](std::initializer_list<type> elements_in_assigned, const usize extra_in_assigned, std::initializer_list<type> elements_to_assign, const usize extra_to_assign)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra_in_assigned);
            hud::array<type, hud_test::array_allocator_2<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

            assigned.assign(to_assign);

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
                assigned.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0u, {}, 0u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 0u, {}, 1u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
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
                constexpr auto result = test({}, 0u, {}, 0u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 0u, {}, 1u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
        }
    }
}

// void assign(const array<u_type_t, u_allocator_t> &other, const usize min_slack = 0)
GTEST_TEST(array, copy_assign_array_of_bitwise_copy_assignable_same_type_different_allocator_with_extra)
{
    using type = i32;

    static_assert(hud::is_bitwise_copy_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, std::initializer_list<type> elements_to_assign)
        {
            constexpr const usize extra_assign = 1;
            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned);
            hud::array<type, hud_test::array_allocator_2<alignof(type)>> to_assign(elements_to_assign);

            assigned.assign(to_assign, extra_assign);

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
                assigned.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
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
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
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
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
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
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
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
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, const usize extra_in_assigned, std::initializer_list<type> elements_to_assign, const usize extra_to_assign)
        {
            constexpr const usize extra_assign = 1;

            hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra_in_assigned);
            hud::array<type, hud_test::array_allocator_2<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

            assigned.assign(to_assign, extra_assign);

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
                assigned.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
        }
    }
}

// void assign(const array<u_type_t, u_allocator_t> &other, const usize min_slack = 0)
GTEST_TEST(array, copy_assign_array_of_bitwise_copy_assignable_different_type_same_allocator_no_extra)
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

            assigned.assign(to_assign);

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
                assigned.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
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
        const auto test = [](std::initializer_list<destination_type> &&elements_in_assigned, const usize extra_in_assigned, std::initializer_list<source_type> elements_to_assign, const usize extra_to_assign)
        {
            hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned, extra_in_assigned);
            hud::array<source_type, hud_test::array_allocator<alignof(source_type)>> to_assign(elements_to_assign, extra_to_assign);

            assigned.assign(to_assign);

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
                assigned.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0u, {}, 0u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 0u, {}, 1u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
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
                constexpr auto result = test({}, 0u, {}, 0u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 0u, {}, 1u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
        }
    }
}

// void assign(const array<u_type_t, u_allocator_t> &other, const usize min_slack = 0)
GTEST_TEST(array, copy_assign_array_of_bitwise_copy_assignable_different_type_same_allocator_with_extra)
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
            constexpr const usize extra_assign = 1;
            hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned);
            hud::array<source_type, hud_test::array_allocator<alignof(source_type)>> to_assign(elements_to_assign);

            assigned.assign(to_assign, extra_assign);

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
                assigned.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
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
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
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
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
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
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
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
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<destination_type> &&elements_in_assigned, const usize extra_in_assigned, std::initializer_list<source_type> elements_to_assign, const usize extra_to_assign)
        {
            constexpr const usize extra_assign = 1;
            hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned, extra_in_assigned);
            hud::array<source_type, hud_test::array_allocator<alignof(source_type)>> to_assign(elements_to_assign, extra_to_assign);

            assigned.assign(to_assign, extra_assign);

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
                assigned.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
            }
        }
    }
}

// // array &operator=(const array<u_type_t, u_allocator_t> &other) noexcept
// GTEST_TEST(array, copy_assign_array_of_bitwise_copy_assignable_different_type_different_allocator)
// {

// using source_type = i32;
// using destination_type = u32;
// // Ensure we test with different types
// static_assert(!std::is_same_v<destination_type, source_type>);
// static_assert(hud::is_bitwise_copy_assignable_v<destination_type, source_type>);

// // Test without extra
// {
//     const auto test = [](std::initializer_list<destination_type> &&elements_in_assigned, std::initializer_list<source_type> elements_to_assign)
//     {
//         hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned);
//         hud::array<source_type, hud_test::array_allocator_2<alignof(source_type)>> to_assign(elements_to_assign);

// assigned = to_assign;

// // Ensures we copy all values correctly
// [[maybe_unused]] bool all_values_are_copied = true;
// for (usize index = 0; index < assigned.count(); index++)
// {
//     // LCOV_EXCL_START
//     if (assigned[index] != static_cast<destination_type>(*(elements_to_assign.begin() + index)))
//     {
//         all_values_are_copied = false;
//         break;
//     }
//     // LCOV_EXCL_STOP
// }

// return std::tuple {
//     assigned.data() != nullptr,
//     // Number of element should be equal to the number of element in the std::initializer_list
//     assigned.count(),
//     // Ensure we do no reallocate for less memory
//     assigned.max_count(),
//     assigned.allocator().allocation_count(),
//     assigned.allocator().free_count()
// };
// };

// // Non constant
// {
//     {
//         const auto result = test({}, {});
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_eq(std::get<3>(result), 0u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         const auto result = test({}, {1, 2});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {3, 4});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2, 3}, {4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {3, 4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 3u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_eq(std::get<3>(result), 2u);
//         hud_assert_eq(std::get<4>(result), 1u);
//     }
// }

// // Constant
// {
//     {
//         constexpr auto result = test({}, {});
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_eq(std::get<3>(result), 0u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, {1, 2});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {3, 4});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2, 3}, {4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {3, 4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 3u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_eq(std::get<3>(result), 2u);
//         hud_assert_eq(std::get<4>(result), 1u);
//     }
// }
// }

// // Test with extra
// {
//     const auto test = [](std::initializer_list<destination_type> &&elements_in_assigned, const usize extra_in_assigned, std::initializer_list<source_type> elements_to_assign, const usize extra_to_assign)
//     {
//         hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned, extra_in_assigned);
//         hud::array<source_type, hud_test::array_allocator_2<alignof(source_type)>> to_assign(elements_to_assign, extra_to_assign);

// assigned = to_assign;

// // Ensures we copy all values correctly
// [[maybe_unused]] bool all_values_are_copied = true;
// for (usize index = 0; index < assigned.count(); index++)
// {
//     // LCOV_EXCL_START
//     if (assigned[index] != static_cast<destination_type>(*(elements_to_assign.begin() + index)))
//     {
//         all_values_are_copied = false;
//         break;
//     }
//     // LCOV_EXCL_STOP
// }

// return std::tuple {
//     assigned.data() != nullptr,
//     // Number of element should be equal to the number of element in the std::initializer_list
//     assigned.count(),
//     // Ensure we do no reallocate for less memory
//     assigned.max_count(),
//     assigned.allocator().allocation_count(),
//     assigned.allocator().free_count()
// };
// };

// // Non constant
// {
//     {
//         const auto result = test({}, 0u, {}, 0u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_eq(std::get<3>(result), 0u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         const auto result = test({}, 1u, {}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         const auto result = test({}, 0u, {}, 1u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_eq(std::get<3>(result), 0u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         const auto result = test({}, 1u, {}, 1u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }

// {
//     const auto result = test({1, 2}, 0u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }

// {
//     const auto result = test({}, 0u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     const auto result = test({}, 1u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 2u);
//     hud_assert_eq(std::get<4>(result), 1u);
// }
// {
//     const auto result = test({}, 0u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     const auto result = test({}, 1u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 2u);
//     hud_assert_eq(std::get<4>(result), 1u);
// }

// {
//     const auto result = test({1, 2}, 0u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }

// {
//     const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }

// {
//     const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 2u);
//     hud_assert_eq(std::get<4>(result), 1u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 2u);
//     hud_assert_eq(std::get<4>(result), 1u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// }

// // Constant
// {
//     {
//         constexpr auto result = test({}, 0u, {}, 0u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_eq(std::get<3>(result), 0u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 1u, {}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 0u, {}, 1u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_eq(std::get<3>(result), 0u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 1u, {}, 1u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }

// {
//     constexpr auto result = test({1, 2}, 0u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }

// {
//     constexpr auto result = test({}, 0u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     constexpr auto result = test({}, 1u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 2u);
//     hud_assert_eq(std::get<4>(result), 1u);
// }
// {
//     constexpr auto result = test({}, 0u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     constexpr auto result = test({}, 1u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 2u);
//     hud_assert_eq(std::get<4>(result), 1u);
// }

// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }

// {
//     constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }

// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 2u);
//     hud_assert_eq(std::get<4>(result), 1u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 2u);
//     hud_assert_eq(std::get<4>(result), 1u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_eq(std::get<3>(result), 1u);
//     hud_assert_eq(std::get<4>(result), 0u);
// }
// }
// }
// }

// // array &operator=(const array &other) noexcept
// GTEST_TEST(array, copy_assign_array_of_non_bitwise_copy_assignable_same_type_same_allocator)
// {

// using type = hud_test::non_bitwise_copy_assignable_type;
// static_assert(!hud::is_bitwise_copy_assignable_v<type>);

// // Test without extra
// {
//     const auto test = [](std::initializer_list<i32> &&elements_in_assigned, std::initializer_list<i32> elements_to_assign)
//     {
//         hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned);
//         hud::array<type, hud_test::array_allocator<alignof(type)>> to_assign(elements_to_assign);

// assigned = to_assign;

// // Ensures we copy all values correctly
// bool all_values_are_copied = true;
// bool all_copy_constructors_are_called = true;
// bool all_copy_assign_are_called = true;
// for (usize index = 0; index < assigned.count(); index++)
// {
//     // LCOV_EXCL_START
//     // Ensure we correctly copied the value
//     if (assigned[index].id() != *(elements_to_assign.begin() + index))
//     {
//         all_values_are_copied = false;
//         break;
//     }

// // Ensure we correctly call copy constructors
// if (elements_in_assigned.size() < elements_to_assign.size())
// {
//     // If we assign more element, we should have reallocate
//     if (assigned[index].copy_constructor_count() != 1u)
//     {
//         all_copy_constructors_are_called = false;
//         break;
//     }
// }
// else
// {
//     // We do not reallocate
//     if (index < elements_in_assigned.size())
//     {
//         // For element that are already here, we call operator=
//         if (assigned[index].copy_assign_count() != 1u)
//         {
//             all_copy_assign_are_called = false;
//             break;
//         }
//     }
//     else
//     {
//         // For new elements we call copy constructors
//         if (assigned[index].copy_constructor_count() != 1u)
//         {
//             all_copy_constructors_are_called = false;
//             break;
//         }
//     }
// }
// // LCOV_EXCL_STOP
// }

// return std::tuple {
//     assigned.data() != nullptr,
//     // Number of element should be equal to the number of element in the std::initializer_list
//     assigned.count(),
//     // Ensure we do no reallocate for less memory
//     assigned.max_count(),
//     all_values_are_copied,
//     all_copy_constructors_are_called,
//     all_copy_assign_are_called,
//     assigned.allocator().allocation_count(),
//     assigned.allocator().free_count()
// };
// };

// // Non constant
// {
//     {
//         const auto result = test({}, {});
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, {1, 2});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {3, 4});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2, 3}, {4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {3, 4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 3u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 2u);
//         hud_assert_eq(std::get<7>(result), 1u);
//     }
// }

// // Constant
// {
//     {
//         constexpr auto result = test({}, {});
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, {1, 2});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {3, 4});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2, 3}, {4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {3, 4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 3u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 2u);
//         hud_assert_eq(std::get<7>(result), 1u);
//     }
// }
// }

// // Test with extra
// {
//     const auto test = [](std::initializer_list<i32> &&elements_in_assigned, const usize extra_in_assigned, std::initializer_list<i32> elements_to_assign, const usize extra_to_assign)
//     {
//         hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra_in_assigned);
//         hud::array<type, hud_test::array_allocator<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

// assigned = to_assign;

// // Ensures we copy all values correctly
// bool all_values_are_copied = true;
// bool all_copy_constructors_are_called = true;
// bool all_copy_assign_are_called = true;
// for (usize index = 0; index < assigned.count(); index++)
// {
//     // LCOV_EXCL_START
//     // Ensure we correctly copied the value
//     if (assigned[index].id() != *(elements_to_assign.begin() + index))
//     {
//         all_values_are_copied = false;
//         break;
//     }

// // Ensure we correctly call copy constructors
// if ((elements_in_assigned.size() + extra_in_assigned) < elements_to_assign.size())
// {
//     // If we assign more element, we should have reallocate
//     if (assigned[index].copy_constructor_count() != 1u)
//     {
//         all_copy_constructors_are_called = false;
//         break;
//     }
// }
// else
// {
//     // We do not reallocate
//     if (index < elements_in_assigned.size())
//     {
//         // For element that are already here, we call operator=
//         if (assigned[index].copy_assign_count() != 1u)
//         {
//             all_copy_assign_are_called = false;
//             break;
//         }
//     }
//     else
//     {
//         // For new elements we call copy constructors
//         if (assigned[index].copy_constructor_count() != 1u)
//         {
//             all_copy_constructors_are_called = false;
//             break;
//         }
//     }
// }
// // LCOV_EXCL_STOP
// }

// return std::tuple {
//     assigned.data() != nullptr,
//     // Number of element should be equal to the number of element in the std::initializer_list
//     assigned.count(),
//     // Ensure we do no reallocate for less memory
//     assigned.max_count(),
//     all_values_are_copied,
//     all_copy_constructors_are_called,
//     all_copy_assign_are_called,
//     assigned.allocator().allocation_count(),
//     assigned.allocator().free_count()
// };
// };

// // Non constant
// {
//     {
//         const auto result = test({}, 0u, {}, 0u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, 1u, {}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, 0u, {}, 1u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, 1u, {}, 1u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }

// {
//     const auto result = test({1, 2}, 0u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     const auto result = test({}, 0u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({}, 1u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     const auto result = test({}, 0u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({}, 1u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }

// {
//     const auto result = test({1, 2}, 0u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// }

// // Constant
// {
//     {
//         constexpr auto result = test({}, 0u, {}, 0u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 1u, {}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 0u, {}, 1u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 1u, {}, 1u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }

// {
//     constexpr auto result = test({1, 2}, 0u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     constexpr auto result = test({}, 0u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({}, 1u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     constexpr auto result = test({}, 0u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({}, 1u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }

// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// }
// }
// }

// // array &operator=(const array<u_type_t, u_allocator_t> &other) noexcept
// GTEST_TEST(array, copy_assign_array_of_non_bitwise_copy_assignable_same_type_different_allocator)
// {

// using type = hud_test::non_bitwise_copy_assignable_type;
// static_assert(!hud::is_bitwise_copy_assignable_v<type>);

// // Test without extra
// {
//     const auto test = [](std::initializer_list<i32> &&elements_in_assigned, std::initializer_list<i32> elements_to_assign)
//     {
//         hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned);
//         hud::array<type, hud_test::array_allocator_2<alignof(type)>> to_assign(elements_to_assign);

// assigned = to_assign;

// // Ensures we copy all values correctly
// bool all_values_are_copied = true;
// bool all_copy_constructors_are_called = true;
// bool all_copy_assign_are_called = true;
// for (usize index = 0; index < assigned.count(); index++)
// {
//     // LCOV_EXCL_START
//     // Ensure we correctly copied the value
//     if (assigned[index].id() != *(elements_to_assign.begin() + index))
//     {
//         all_values_are_copied = false;
//         break;
//     }

// // Ensure we correctly call copy constructors
// if (elements_in_assigned.size() < elements_to_assign.size())
// {
//     // If we assign more element, we should have reallocate
//     if (assigned[index].copy_constructor_count() != 1u)
//     {
//         all_copy_constructors_are_called = false;
//         break;
//     }
// }
// else
// {
//     // We do not reallocate
//     if (index < elements_in_assigned.size())
//     {
//         // For element that are already here, we call operator=
//         if (assigned[index].copy_assign_count() != 1u)
//         {
//             all_copy_assign_are_called = false;
//             break;
//         }
//     }
//     else
//     {
//         // For new elements we call copy constructors
//         if (assigned[index].copy_constructor_count() != 1u)
//         {
//             all_copy_constructors_are_called = false;
//             break;
//         }
//     }
// }
// // LCOV_EXCL_STOP
// }

// return std::tuple {
//     assigned.data() != nullptr,
//     // Number of element should be equal to the number of element in the std::initializer_list
//     assigned.count(),
//     // Ensure we do no reallocate for less memory
//     assigned.max_count(),
//     all_values_are_copied,
//     all_copy_constructors_are_called,
//     all_copy_assign_are_called,
//     assigned.allocator().allocation_count(),
//     assigned.allocator().free_count()
// };
// };

// // Non constant
// {
//     {
//         const auto result = test({}, {});
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, {1, 2});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {3, 4});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2, 3}, {4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {3, 4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 3u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 2u);
//         hud_assert_eq(std::get<7>(result), 1u);
//     }
// }

// // Constant
// {
//     {
//         constexpr auto result = test({}, {});
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, {1, 2});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {3, 4});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2, 3}, {4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {3, 4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 3u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 2u);
//         hud_assert_eq(std::get<7>(result), 1u);
//     }
// }
// }

// // Test with extra
// {
//     const auto test = [](std::initializer_list<i32> &&elements_in_assigned, const usize extra_in_assigned, std::initializer_list<i32> elements_to_assign, const usize extra_to_assign)
//     {
//         hud::array<type, hud_test::array_allocator<alignof(type)>> assigned(elements_in_assigned, extra_in_assigned);
//         hud::array<type, hud_test::array_allocator_2<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

// assigned = to_assign;

// // Ensures we copy all values correctly
// bool all_values_are_copied = true;
// bool all_copy_constructors_are_called = true;
// bool all_copy_assign_are_called = true;
// for (usize index = 0; index < assigned.count(); index++)
// {
//     // LCOV_EXCL_START
//     // Ensure we correctly copied the value
//     if (assigned[index].id() != *(elements_to_assign.begin() + index))
//     {
//         all_values_are_copied = false;
//         break;
//     }

// // Ensure we correctly call copy constructors
// if ((elements_in_assigned.size() + extra_in_assigned) < elements_to_assign.size())
// {
//     // If we assign more element, we should have reallocate
//     if (assigned[index].copy_constructor_count() != 1u)
//     {
//         all_copy_constructors_are_called = false;
//         break;
//     }
// }
// else
// {
//     // We do not reallocate
//     if (index < elements_in_assigned.size())
//     {
//         // For element that are already here, we call operator=
//         if (assigned[index].copy_assign_count() != 1u)
//         {
//             all_copy_assign_are_called = false;
//             break;
//         }
//     }
//     else
//     {
//         // For new elements we call copy constructors
//         if (assigned[index].copy_constructor_count() != 1u)
//         {
//             all_copy_constructors_are_called = false;
//             break;
//         }
//     }
// }
// // LCOV_EXCL_STOP
// }

// return std::tuple {
//     assigned.data() != nullptr,
//     // Number of element should be equal to the number of element in the std::initializer_list
//     assigned.count(),
//     // Ensure we do no reallocate for less memory
//     assigned.max_count(),
//     all_values_are_copied,
//     all_copy_constructors_are_called,
//     all_copy_assign_are_called,
//     assigned.allocator().allocation_count(),
//     assigned.allocator().free_count()
// };
// };

// // Non constant
// {
//     {
//         const auto result = test({}, 0u, {}, 0u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, 1u, {}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, 0u, {}, 1u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, 1u, {}, 1u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }

// {
//     const auto result = test({1, 2}, 0u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     const auto result = test({}, 0u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({}, 1u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     const auto result = test({}, 0u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({}, 1u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }

// {
//     const auto result = test({1, 2}, 0u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// }

// // Constant
// {
//     {
//         constexpr auto result = test({}, 0u, {}, 0u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 1u, {}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 0u, {}, 1u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 1u, {}, 1u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }

// {
//     constexpr auto result = test({1, 2}, 0u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     constexpr auto result = test({}, 0u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({}, 1u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     constexpr auto result = test({}, 0u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({}, 1u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }

// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// }
// }
// }

// // array &operator=(const array<u_type_t, u_allocator_t> &other) noexcept
// GTEST_TEST(array, copy_assign_array_of_non_bitwise_copy_assignable_different_type_same_allocator)
// {

// using destination_type = hud_test::non_bitwise_copy_assignable_type_2;
// using source_type = hud_test::non_bitwise_copy_assignable_type;
// // Ensure we test with different types
// static_assert(!std::is_same_v<destination_type, source_type>);
// static_assert(!hud::is_bitwise_copy_assignable_v<destination_type, source_type>);

// // Test without extra
// {
//     const auto test = [](std::initializer_list<i32> &&elements_in_assigned, std::initializer_list<i32> elements_to_assign)
//     {
//         hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned);
//         hud::array<source_type, hud_test::array_allocator<alignof(source_type)>> to_assign(elements_to_assign);

// assigned = to_assign;

// // Ensures we copy all values correctly
// bool all_values_are_copied = true;
// bool all_copy_constructors_are_called = true;
// bool all_copy_assign_are_called = true;
// for (usize index = 0; index < assigned.count(); index++)
// {
//     // LCOV_EXCL_START
//     // Ensure we correctly copied the value
//     if (assigned[index].id() != *(elements_to_assign.begin() + index))
//     {
//         all_values_are_copied = false;
//         break;
//     }

// // Ensure we correctly call copy constructors
// if (elements_in_assigned.size() < elements_to_assign.size())
// {
//     // If we assign more element, we should have reallocate
//     if (assigned[index].copy_constructor_count() != 1u)
//     {
//         all_copy_constructors_are_called = false;
//         break;
//     }
// }
// else
// {
//     // We do not reallocate
//     if (index < elements_in_assigned.size())
//     {
//         // For element that are already here, we call operator=
//         if (assigned[index].copy_assign_count() != 1u)
//         {
//             all_copy_assign_are_called = false;
//             break;
//         }
//     }
//     else
//     {
//         // For new elements we call copy constructors
//         if (assigned[index].copy_constructor_count() != 1u)
//         {
//             all_copy_constructors_are_called = false;
//             break;
//         }
//     }
// }
// // LCOV_EXCL_STOP
// }

// return std::tuple {
//     assigned.data() != nullptr,
//     // Number of element should be equal to the number of element in the std::initializer_list
//     assigned.count(),
//     // Ensure we do no reallocate for less memory
//     assigned.max_count(),
//     all_values_are_copied,
//     all_copy_constructors_are_called,
//     all_copy_assign_are_called,
//     assigned.allocator().allocation_count(),
//     assigned.allocator().free_count()
// };
// };

// // Non constant
// {
//     {
//         const auto result = test({}, {});
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, {1, 2});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {3, 4});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2, 3}, {4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {3, 4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 3u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 2u);
//         hud_assert_eq(std::get<7>(result), 1u);
//     }
// }

// // Constant
// {
//     {
//         constexpr auto result = test({}, {});
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, {1, 2});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {3, 4});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2, 3}, {4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {3, 4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 3u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 2u);
//         hud_assert_eq(std::get<7>(result), 1u);
//     }
// }
// }

// // Test with extra
// {
//     const auto test = [](std::initializer_list<i32> &&elements_in_assigned, const usize extra_in_assigned, std::initializer_list<i32> elements_to_assign, const usize extra_to_assign)
//     {
//         hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned, extra_in_assigned);
//         hud::array<source_type, hud_test::array_allocator<alignof(source_type)>> to_assign(elements_to_assign, extra_to_assign);

// assigned = to_assign;

// // Ensures we copy all values correctly
// bool all_values_are_copied = true;
// bool all_copy_constructors_are_called = true;
// bool all_copy_assign_are_called = true;
// for (usize index = 0; index < assigned.count(); index++)
// {
//     // LCOV_EXCL_START
//     // Ensure we correctly copied the value
//     if (assigned[index].id() != *(elements_to_assign.begin() + index))
//     {
//         all_values_are_copied = false;
//         break;
//     }

// // Ensure we correctly call copy constructors
// if ((elements_in_assigned.size() + extra_in_assigned) < elements_to_assign.size())
// {
//     // If we assign more element, we should have reallocate
//     if (assigned[index].copy_constructor_count() != 1u)
//     {
//         all_copy_constructors_are_called = false;
//         break;
//     }
// }
// else
// {
//     // We do not reallocate
//     if (index < elements_in_assigned.size())
//     {
//         // For element that are already here, we call operator=
//         if (assigned[index].copy_assign_count() != 1u)
//         {
//             all_copy_assign_are_called = false;
//             break;
//         }
//     }
//     else
//     {
//         // For new elements we call copy constructors
//         if (assigned[index].copy_constructor_count() != 1u)
//         {
//             all_copy_constructors_are_called = false;
//             break;
//         }
//     }
// }
// // LCOV_EXCL_STOP
// }

// return std::tuple {
//     assigned.data() != nullptr,
//     // Number of element should be equal to the number of element in the std::initializer_list
//     assigned.count(),
//     // Ensure we do no reallocate for less memory
//     assigned.max_count(),
//     all_values_are_copied,
//     all_copy_constructors_are_called,
//     all_copy_assign_are_called,
//     assigned.allocator().allocation_count(),
//     assigned.allocator().free_count()
// };
// };

// // Non constant
// {
//     {
//         const auto result = test({}, 0u, {}, 0u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, 1u, {}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, 0u, {}, 1u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, 1u, {}, 1u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }

// {
//     const auto result = test({1, 2}, 0u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     const auto result = test({}, 0u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({}, 1u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     const auto result = test({}, 0u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({}, 1u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }

// {
//     const auto result = test({1, 2}, 0u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// }

// // Constant
// {
//     {
//         constexpr auto result = test({}, 0u, {}, 0u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 1u, {}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 0u, {}, 1u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 1u, {}, 1u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }

// {
//     constexpr auto result = test({1, 2}, 0u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     constexpr auto result = test({}, 0u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({}, 1u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     constexpr auto result = test({}, 0u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({}, 1u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }

// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// }
// }
// }

// // array &operator=(const array<u_type_t, u_allocator_t> &other) noexcept
// GTEST_TEST(array, copy_assign_array_of_non_bitwise_copy_assignable_different_type_different_allocator)
// {

// using destination_type = hud_test::non_bitwise_copy_assignable_type_2;
// using source_type = hud_test::non_bitwise_copy_assignable_type;
// // Ensure we test with different types
// static_assert(!std::is_same_v<destination_type, source_type>);
// static_assert(!hud::is_bitwise_copy_assignable_v<destination_type, source_type>);

// // Test without extra
// {
//     const auto test = [](std::initializer_list<i32> &&elements_in_assigned, std::initializer_list<i32> elements_to_assign)
//     {
//         hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned);
//         hud::array<source_type, hud_test::array_allocator_2<alignof(source_type)>> to_assign(elements_to_assign);

// assigned = to_assign;

// // Ensures we copy all values correctly
// bool all_values_are_copied = true;
// bool all_copy_constructors_are_called = true;
// bool all_copy_assign_are_called = true;
// for (usize index = 0; index < assigned.count(); index++)
// {
//     // LCOV_EXCL_START
//     // Ensure we correctly copied the value
//     if (assigned[index].id() != *(elements_to_assign.begin() + index))
//     {
//         all_values_are_copied = false;
//         break;
//     }

// // Ensure we correctly call copy constructors
// if (elements_in_assigned.size() < elements_to_assign.size())
// {
//     // If we assign more element, we should have reallocate
//     if (assigned[index].copy_constructor_count() != 1u)
//     {
//         all_copy_constructors_are_called = false;
//         break;
//     }
// }
// else
// {
//     // We do not reallocate
//     if (index < elements_in_assigned.size())
//     {
//         // For element that are already here, we call operator=
//         if (assigned[index].copy_assign_count() != 1u)
//         {
//             all_copy_assign_are_called = false;
//             break;
//         }
//     }
//     else
//     {
//         // For new elements we call copy constructors
//         if (assigned[index].copy_constructor_count() != 1u)
//         {
//             all_copy_constructors_are_called = false;
//             break;
//         }
//     }
// }
// // LCOV_EXCL_STOP
// }

// return std::tuple {
//     assigned.data() != nullptr,
//     // Number of element should be equal to the number of element in the std::initializer_list
//     assigned.count(),
//     // Ensure we do no reallocate for less memory
//     assigned.max_count(),
//     all_values_are_copied,
//     all_copy_constructors_are_called,
//     all_copy_assign_are_called,
//     assigned.allocator().allocation_count(),
//     assigned.allocator().free_count()
// };
// };

// // Non constant
// {
//     {
//         const auto result = test({}, {});
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, {1, 2});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {3, 4});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2, 3}, {4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, {3, 4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 3u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 2u);
//         hud_assert_eq(std::get<7>(result), 1u);
//     }
// }

// // Constant
// {
//     {
//         constexpr auto result = test({}, {});
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, {1, 2});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {3, 4});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2, 3}, {4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, {3, 4, 5});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 3u);
//         hud_assert_eq(std::get<2>(result), 3u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 2u);
//         hud_assert_eq(std::get<7>(result), 1u);
//     }
// }
// }

// // Test with extra
// {
//     const auto test = [](std::initializer_list<i32> &&elements_in_assigned, const usize extra_in_assigned, std::initializer_list<i32> elements_to_assign, const usize extra_to_assign)
//     {
//         hud::array<destination_type, hud_test::array_allocator<alignof(destination_type)>> assigned(elements_in_assigned, extra_in_assigned);
//         hud::array<source_type, hud_test::array_allocator_2<alignof(source_type)>> to_assign(elements_to_assign, extra_to_assign);

// assigned = to_assign;

// // Ensures we copy all values correctly
// bool all_values_are_copied = true;
// bool all_copy_constructors_are_called = true;
// bool all_copy_assign_are_called = true;
// for (usize index = 0; index < assigned.count(); index++)
// {
//     // LCOV_EXCL_START
//     // Ensure we correctly copied the value
//     if (assigned[index].id() != *(elements_to_assign.begin() + index))
//     {
//         all_values_are_copied = false;
//         break;
//     }

// // Ensure we correctly call copy constructors
// if ((elements_in_assigned.size() + extra_in_assigned) < elements_to_assign.size())
// {
//     // If we assign more element, we should have reallocate
//     if (assigned[index].copy_constructor_count() != 1u)
//     {
//         all_copy_constructors_are_called = false;
//         break;
//     }
// }
// else
// {
//     // We do not reallocate
//     if (index < elements_in_assigned.size())
//     {
//         // For element that are already here, we call operator=
//         if (assigned[index].copy_assign_count() != 1u)
//         {
//             all_copy_assign_are_called = false;
//             break;
//         }
//     }
//     else
//     {
//         // For new elements we call copy constructors
//         if (assigned[index].copy_constructor_count() != 1u)
//         {
//             all_copy_constructors_are_called = false;
//             break;
//         }
//     }
// }
// // LCOV_EXCL_STOP
// }

// return std::tuple {
//     assigned.data() != nullptr,
//     // Number of element should be equal to the number of element in the std::initializer_list
//     assigned.count(),
//     // Ensure we do no reallocate for less memory
//     assigned.max_count(),
//     all_values_are_copied,
//     all_copy_constructors_are_called,
//     all_copy_assign_are_called,
//     assigned.allocator().allocation_count(),
//     assigned.allocator().free_count()
// };
// };

// // Non constant
// {
//     {
//         const auto result = test({}, 0u, {}, 0u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, 1u, {}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, 0u, {}, 1u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         const auto result = test({}, 1u, {}, 1u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }

// {
//     const auto result = test({1, 2}, 0u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     const auto result = test({}, 0u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({}, 1u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     const auto result = test({}, 0u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({}, 1u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }

// {
//     const auto result = test({1, 2}, 0u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// }

// // Constant
// {
//     {
//         constexpr auto result = test({}, 0u, {}, 0u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 1u, {}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 0u, {}, 1u);
//         hud_assert_false(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 0u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 1u, {}, 1u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_true(std::get<3>(result));
//         hud_assert_true(std::get<4>(result));
//         hud_assert_true(std::get<5>(result));
//         hud_assert_eq(std::get<6>(result), 1u);
//         hud_assert_eq(std::get<7>(result), 0u);
//     }

// {
//     constexpr auto result = test({1, 2}, 0u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 0u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     constexpr auto result = test({}, 0u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({}, 1u, {1, 2}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     constexpr auto result = test({}, 0u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({}, 1u, {1, 2}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }

// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 2u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 2u);
//     hud_assert_eq(std::get<2>(result), 4u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }

// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// {
//     constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 2u);
//     hud_assert_eq(std::get<7>(result), 1u);
// }
// {
//     constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
//     hud_assert_true(std::get<0>(result));
//     hud_assert_eq(std::get<1>(result), 3u);
//     hud_assert_eq(std::get<2>(result), 3u);
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_eq(std::get<6>(result), 1u);
//     hud_assert_eq(std::get<7>(result), 0u);
// }
// }
// }
// }

// // array &operator=(const array &other) noexcept
// GTEST_TEST(array, copy_assign_array_of_bitwise_copy_assignable_to_self)
// {

// using type = i32;

// static_assert(hud::is_bitwise_copy_assignable_v<type>);

// // Test without extra
// {
//     const auto test = [](std::initializer_list<type> elements_in_assigned)
//     {
//         hud::array<type, hud_test::array_allocator<alignof(type)>> arr(elements_in_assigned);

// const type *data_before = arr.data();
// arr = arr;

// return std::tuple {
//     arr.data() == data_before,
//     // Number of element should be equal to the number of element in the std::initializer_list
//     arr.count(),
//     // Ensure we do no reallocate for less memory
//     arr.max_count(),
//     arr.allocator().allocation_count(),
//     arr.allocator().free_count()
// };
// };

// // Non constant
// {
//     {
//         const auto result = test({});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_eq(std::get<3>(result), 0u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
// }

// // Constant
// {
//     {
//         constexpr auto result = test({});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_eq(std::get<3>(result), 0u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2});
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
// }
// }

// // Test with extra
// {
//     const auto test = [](std::initializer_list<type> elements_in_assigned, const usize extra_in_assigned)
//     {
//         hud::array<type, hud_test::array_allocator<alignof(type)>> arr(elements_in_assigned, extra_in_assigned);

// const type *data_before = arr.data();
// arr = arr;

// return std::tuple {
//     arr.data() == data_before,
//     // Number of element should be equal to the number of element in the std::initializer_list
//     arr.count(),
//     // Ensure we do no reallocate for less memory
//     arr.max_count(),
//     arr.allocator().allocation_count(),
//     arr.allocator().free_count()
// };
// };

// // Non constant
// {
//     {
//         const auto result = test({}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_eq(std::get<3>(result), 0u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         const auto result = test({}, 1u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         const auto result = test({1, 2}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
// }

// // Constant
// {
//     {
//         constexpr auto result = test({}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 0u);
//         hud_assert_eq(std::get<3>(result), 0u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         constexpr auto result = test({}, 1u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 0u);
//         hud_assert_eq(std::get<2>(result), 1u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
//     {
//         constexpr auto result = test({1, 2}, 0u);
//         hud_assert_true(std::get<0>(result));
//         hud_assert_eq(std::get<1>(result), 2u);
//         hud_assert_eq(std::get<2>(result), 2u);
//         hud_assert_eq(std::get<3>(result), 1u);
//         hud_assert_eq(std::get<4>(result), 0u);
//     }
// }
// }
// }

// // array &operator=(const array &other) noexcept
// GTEST_TEST(array, copy_assign_array_of_non_bitwise_copy_assignable_to_self)