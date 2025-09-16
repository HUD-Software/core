#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"
#include "../misc/leak_guard.h"

// void assign(vector &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_bitwise_move_assignable_same_type_same_allocator_no_min_slack)
{
    using type = i32;

    static_assert(hud::is_bitwise_move_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, std::initializer_list<type> elements_to_assign) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> assigned(elements_in_assigned);
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> to_assign(elements_to_assign);

            assigned.assign(hud::move(to_assign));

            // Ensures we move all values correctly
            [[maybe_unused]] bool all_values_are_moved = true;
            for (usize index = 0; index < assigned.count(); index++) {
                // LCOV_EXCL_START
                if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index))) {
                    all_values_are_moved = false;
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
                assigned.allocator().free_count(),
                to_assign.data() == nullptr,
                to_assign.count(),
                to_assign.max_count(),
                to_assign.allocator().allocation_count(),
                to_assign.allocator().free_count(),
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, {});
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, {1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, {4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, {3, 4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({}, {1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, {4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, {3, 4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, const usize extra_in_assigned, std::initializer_list<type> elements_to_assign, const usize extra_to_assign) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> assigned(elements_in_assigned, extra_in_assigned);
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

            assigned.assign(hud::move(to_assign));

            // Ensures we move all values correctly
            [[maybe_unused]] bool all_values_are_moved = true;
            for (usize index = 0; index < assigned.count(); index++) {
                // LCOV_EXCL_START
                if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index))) {
                    all_values_are_moved = false;
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
                assigned.allocator().free_count(),
                to_assign.data() == nullptr,
                to_assign.count(),
                to_assign.max_count(),
                to_assign.allocator().allocation_count(),
                to_assign.allocator().free_count(),
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 0u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }

            {
                const auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }

            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({}, 0u, {}, 1u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
        }
    }
}

// void assign(vector &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_bitwise_move_assignable_same_type_same_allocator_with_min_slack)
{

    using type = i32;

    static_assert(hud::is_bitwise_move_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, std::initializer_list<type> elements_to_assign) {
            constexpr const usize min_slack = 1;
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> assigned(elements_in_assigned);
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> to_assign(elements_to_assign);

            assigned.assign(hud::move(to_assign), min_slack);

            // Ensures we move all values correctly
            [[maybe_unused]] bool all_values_are_moved = true;
            for (usize index = 0; index < assigned.count(); index++) {
                // LCOV_EXCL_START
                if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index))) {
                    all_values_are_moved = false;
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
                assigned.allocator().free_count(),
                to_assign.data() == nullptr,
                to_assign.count(),
                to_assign.max_count(),
                to_assign.allocator().allocation_count(),
                to_assign.allocator().free_count(),
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, {1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2, 3}, {4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, {3, 4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({}, {1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, {4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, {3, 4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, const usize extra_in_assigned, std::initializer_list<type> elements_to_assign, const usize extra_to_assign) {
            constexpr const usize min_slack = 1;
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> assigned(elements_in_assigned, extra_in_assigned);
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

            assigned.assign(hud::move(to_assign), min_slack);

            // Ensures we move all values correctly
            [[maybe_unused]] bool all_values_are_moved = true;
            for (usize index = 0; index < assigned.count(); index++) {
                // LCOV_EXCL_START
                if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index))) {
                    all_values_are_moved = false;
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
                assigned.allocator().free_count(),
                to_assign.data() == nullptr,
                to_assign.count(),
                to_assign.max_count(),
                to_assign.allocator().allocation_count(),
                to_assign.allocator().free_count(),
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }

            {
                const auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }

            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 0u);
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
        }
    }
}

// void move_assign(vector<u_type_t, allocator_t> &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_bitwise_move_assignable_same_type_different_allocator_no_min_slack)
{

    using type = i32;

    static_assert(hud::is_bitwise_move_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, std::initializer_list<type> elements_to_assign) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> assigned(elements_in_assigned);
            hud::vector<type, hud_test::allocator_watcher_2<alignof(type)>> to_assign(elements_to_assign);

            assigned.assign(hud::move(to_assign));

            // Ensures we move all values correctly
            [[maybe_unused]] bool all_values_are_moved = true;
            for (usize index = 0; index < assigned.count(); index++) {
                // LCOV_EXCL_START
                if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index))) {
                    all_values_are_moved = false;
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
                assigned.allocator().free_count(),
                to_assign.data() == nullptr,
                to_assign.count(),
                to_assign.max_count(),
                to_assign.allocator().allocation_count(),
                to_assign.allocator().free_count(),
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, {1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2, 3}, {4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, {3, 4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({}, {1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, {4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, {3, 4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
        }
    }

    // Test with extra
    {

        const auto test = [](std::initializer_list<type> elements_in_assigned, const usize extra_in_assigned, std::initializer_list<type> elements_to_assign, const usize extra_to_assign) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> assigned(elements_in_assigned, extra_in_assigned);
            hud::vector<type, hud_test::allocator_watcher_2<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

            assigned.assign(hud::move(to_assign));

            // Ensures we move all values correctly
            [[maybe_unused]] bool all_values_are_moved = true;
            for (usize index = 0; index < assigned.count(); index++) {
                // LCOV_EXCL_START
                if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index))) {
                    all_values_are_moved = false;
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
                assigned.allocator().free_count(),
                to_assign.data() == nullptr,
                to_assign.count(),
                to_assign.max_count(),
                to_assign.allocator().allocation_count(),
                to_assign.allocator().free_count(),
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 0u, {}, 1u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                const auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                const auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({}, 0u, {}, 1u);
                hud_assert_false(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
        }
    }
}

// void move_assign(vector<u_type_t, allocator_t> &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_bitwise_move_assignable_same_type_different_allocator_with_min_slack)
{

    using type = i32;

    static_assert(hud::is_bitwise_move_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, std::initializer_list<type> elements_to_assign) {
            constexpr const usize min_slack = 1;
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> assigned(elements_in_assigned);
            hud::vector<type, hud_test::allocator_watcher_2<alignof(type)>> to_assign(elements_to_assign);

            assigned.assign(hud::move(to_assign), min_slack);

            // Ensures we move all values correctly
            [[maybe_unused]] bool all_values_are_moved = true;
            for (usize index = 0; index < assigned.count(); index++) {
                // LCOV_EXCL_START
                if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index))) {
                    all_values_are_moved = false;
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
                assigned.allocator().free_count(),
                to_assign.data() == nullptr,
                to_assign.count(),
                to_assign.max_count(),
                to_assign.allocator().allocation_count(),
                to_assign.allocator().free_count(),
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, {1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2, 3}, {4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, {3, 4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({}, {1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, {3, 4});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, {4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, {3, 4, 5});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
        }
    }

    // Test with extra
    {

        const auto test = [](std::initializer_list<type> elements_in_assigned, const usize extra_in_assigned, std::initializer_list<type> elements_to_assign, const usize extra_to_assign) {
            constexpr const usize min_slack = 1;
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> assigned(elements_in_assigned, extra_in_assigned);
            hud::vector<type, hud_test::allocator_watcher_2<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

            assigned.assign(hud::move(to_assign), min_slack);

            // Ensures we move all values correctly
            [[maybe_unused]] bool all_values_are_moved = true;
            for (usize index = 0; index < assigned.count(); index++) {
                // LCOV_EXCL_START
                if (assigned[index] != static_cast<type>(*(elements_to_assign.begin() + index))) {
                    all_values_are_moved = false;
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
                assigned.allocator().free_count(),
                to_assign.data() == nullptr,
                to_assign.count(),
                to_assign.max_count(),
                to_assign.allocator().allocation_count(),
                to_assign.allocator().free_count(),
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                const auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                const auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                const auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
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
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 0u);
                hud_assert_eq(std::get<9>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({}, 0u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 0u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({}, 1u, {1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 0u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2, 3}, 1u, {4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }

            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 0u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
            {
                constexpr auto result = test({1, 2}, 1u, {3, 4, 5}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 3u);
                hud_assert_eq(std::get<2>(result), 4u);
                hud_assert_eq(std::get<3>(result), 2u);
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_true(std::get<5>(result));
                hud_assert_eq(std::get<6>(result), 0u);
                hud_assert_eq(std::get<7>(result), 0u);
                hud_assert_eq(std::get<8>(result), 1u);
                hud_assert_eq(std::get<9>(result), 1u);
            }
        }
    }
}

// void move_assign(vector<u_type_t, allocator_t> &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_bitwise_move_assignable_different_type_same_allocator_no_min_slack)
{
    using type1 = i32;
    using type2 = u32;
    // Ensure we test with different types
    static_assert(!std::is_same_v<type1, type2>);
    static_assert(hud::is_bitwise_copy_assignable_v<type2, type1>);

    auto test_assign = [](std::initializer_list<type1> elements_in_assigned, usize extra, std::initializer_list<type2> elements_to_assign, usize extra_to_assign) noexcept {
        hud::vector<type1, hud_test::allocator_watcher<alignof(type1)>> assigned(elements_in_assigned, extra);
        hud::vector<type2, hud_test::allocator_watcher<alignof(type2)>> to_assign(elements_to_assign, extra_to_assign);

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }
        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0) {
            hud_assert_ne(assigned_buffer_address, to_assigned_buffer_address);
        }

        assigned.assign(hud::move(to_assign));

        // Number of element should be equal to the number of element in the std::initializer_list
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // Ensure we keep all allocated memory from the move pointer
        hud_assert_eq(assigned.max_count(), elements_to_assign.size() + extra_to_assign);

        // Ensures we move all values correctly
        for (usize index = 0; index < assigned.count(); index++) {
            hud_assert_eq(assigned[index], static_cast<type1>(*(elements_to_assign.begin() + index)));
        }

        // Allocation is not supposed to be done, the type is bitwise movable, the vector should just take the allocation buffer ( move the pointer to the allocation )
        // assigned vector should free the buffer if he have one
        // Ensure we really stole the buffer and do not allocate
        hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count);
        if ((elements_in_assigned.size() + extra) != 0) {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
        }

        // Ensure we really stole the buffer by just moving the pointer
        hud_assert_eq(reinterpret_cast<const uptr>(assigned.data()), to_assigned_buffer_address);

        // Ensure that the moved vector is set to empty
        hud_assert_eq(to_assign.count(), 0u);
        hud_assert_eq(to_assign.max_count(), 0u);
        hud_assert_eq(to_assign.data(), nullptr);

        // Ensure the moved vector was not freed and only one allocation was done ( at initilisation of the vector )
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().allocation_count(), 1u);
        }
        hud_assert_eq(to_assign.allocator().free_count(), 0u);
    };

    for (usize extra = 0; extra < 5; extra++) {
        for (usize extra2 = 0; extra2 < 5; extra2++) {
            test_assign({}, extra, {}, extra2);
            test_assign({}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {}, extra2);
        }
    }
}

// void move_assign(vector<u_type_t, allocator_t> &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_bitwise_move_assignable_different_type_same_allocator_with_min_slack)
{
    using type1 = i32;
    using type2 = u32;
    // Ensure we test with different types
    static_assert(!std::is_same_v<type1, type2>);
    static_assert(hud::is_bitwise_copy_assignable_v<type2, type1>);

    auto test_assign = [](std::initializer_list<type1> elements_in_assigned, usize extra, std::initializer_list<type2> elements_to_assign, usize extra_to_assign) noexcept {
        constexpr const usize min_slack = 1;
        hud::vector<type1, hud_test::allocator_watcher<alignof(type1)>> assigned(elements_in_assigned, extra);
        hud::vector<type2, hud_test::allocator_watcher<alignof(type2)>> to_assign(elements_to_assign, extra_to_assign);

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;

        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }
        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0) {
            hud_assert_ne(assigned_buffer_address, to_assigned_buffer_address);
        }

        /****** void assign(vector &&other, const usize min_slack = 0) */
        assigned.assign(hud::move(to_assign), min_slack);

        // We should have the same count as element to assign
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));
        // We should have at least the min_slack as slack
        hud_assert_ge(assigned.slack(), hud::max(extra_to_assign, min_slack));
        hud_assert_ge(assigned.max_count(), elements_to_assign.size() + hud::max(extra_to_assign, min_slack));

        // Ensure elements are all copied correctly
        for (usize index = 0; index < assigned.count(); index++) {
            hud_assert_eq(assigned[index], static_cast<type1>(*(elements_to_assign.begin() + index)));
        }

        // As we move bitwise movable type we can just stole the pointer buffer
        // A reallocation is done only if the allocation can't be stolen ( Don't meet the min_slack requirements )
        // and if the size of the element in assigned buffer with the extra slack is not big enough to contains the element to assign + min_slack
        bool should_reallocate = false;
        bool is_allocation_stolen = extra_to_assign >= min_slack;
        if (!is_allocation_stolen) {
            should_reallocate = elements_to_assign.size() + hud::max(extra_to_assign, min_slack) > elements_in_assigned.size() + extra;
        }
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1u);
        }
        else {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count);
        }

        // We free the buffer in assigned element if we have elements and if we reallocate the buffer or we stole it
        if (elements_in_assigned.size() + extra > 0 && (should_reallocate || is_allocation_stolen)) {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
        }
        else {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count);
        }

        // Ensure that the moved vector is set to empty
        hud_assert_eq(to_assign.count(), 0u);
        hud_assert_eq(to_assign.max_count(), 0u);
        hud_assert_eq(to_assign.data(), nullptr);
        // Ensure we don't do extra allocation in the assigned vector
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().allocation_count(), 1u);
        }
        else {
            hud_assert_eq(to_assign.allocator().allocation_count(), 0u);
        }
        // Ensure we freed the buffer in the assigned vector except if we stole the buffer
        if ((elements_to_assign.size() + extra_to_assign) > 0 && !is_allocation_stolen) {
            hud_assert_eq(to_assign.allocator().free_count(), 1u);
        }
        else {

            hud_assert_eq(to_assign.allocator().free_count(), 0u);
        }
    };

    /** Run tests. */
    for (usize extra = 0; extra < 5; extra++) {
        for (usize extra2 = 0; extra2 < 5; extra2++) {
            test_assign({}, extra, {}, extra2);
            test_assign({}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {}, extra2);
        }
    }
}

// void assign(vector<u_type_t, u_allocator_t> &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_bitwise_move_assignable_different_type_different_allocator_no_min_slack)
{
    using type1 = i32;
    using type2 = u32;
    // Ensure we test with different types
    static_assert(!std::is_same_v<type1, type2>);
    static_assert(hud::is_bitwise_copy_assignable_v<type2, type1>);

    auto test_assign = [](std::initializer_list<type1> elements_in_assigned, usize extra, std::initializer_list<type2> elements_to_assign, usize extra_to_assign) noexcept {
        hud::vector<type1, hud_test::allocator_watcher<alignof(type1)>> assigned(elements_in_assigned, extra);
        hud::vector<type2, hud_test::allocator_watcher_2<alignof(type2)>> to_assign(elements_to_assign, extra_to_assign);

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }
        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0) {
            hud_assert_ne(assigned_buffer_address, to_assigned_buffer_address);
        }

        assigned.assign(hud::move(to_assign));

        // Number of element should be equal to the number of element in the std::initializer_list
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // max_count must grown only if we don't have enough memory in assigned vector
        if (elements_to_assign.size() > (elements_in_assigned.size() + extra)) {
            hud_assert_eq(assigned.max_count(), elements_to_assign.size());
        }
        else {
            hud_assert_eq(assigned.max_count(), elements_in_assigned.size() + extra);
        }

        // Ensures we move all values correctly
        for (usize index = 0; index < assigned.count(); index++) {
            hud_assert_eq(assigned[index], static_cast<type1>(*(elements_to_assign.begin() + index)));
        }

        // If we have elements to assign, a new allocation should be done
        if (elements_to_assign.size() > (elements_in_assigned.size() + extra)) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1);
        }
        else {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count);
        }

        // If we already have allocated data and the allocation size can't fit the assigned data, a free should be done
        if ((elements_in_assigned.size() + extra) > 0 && (elements_in_assigned.size() + extra) < elements_to_assign.size()) {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
        }
        else {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count);
        }

        // Ensure that the moved vector is set to empty
        hud_assert_eq(to_assign.count(), 0u);
        hud_assert_eq(to_assign.max_count(), 0u);
        hud_assert_eq(to_assign.data(), nullptr);

        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().allocation_count(), 1u);
            hud_assert_eq(to_assign.allocator().free_count(), 1u);
        }
        else {
            hud_assert_eq(to_assign.allocator().allocation_count(), 0u);
            hud_assert_eq(to_assign.allocator().free_count(), 0u);
        }
    };

    for (usize extra = 0; extra < 5; extra++) {
        for (usize extra2 = 0; extra2 < 5; extra2++) {
            test_assign({}, extra, {}, extra2);
            test_assign({}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {}, extra2);
        }
    }
}

// void assign(vector<u_type_t, u_allocator_t> &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_bitwise_move_assignable_different_type_different_allocator_with_min_slack)
{
    using type1 = i32;
    using type2 = u32;
    // Ensure we test with different types
    static_assert(!std::is_same_v<type1, type2>);
    static_assert(hud::is_bitwise_copy_assignable_v<type2, type1>);

    auto test_assign = [](std::initializer_list<type1> elements_in_assigned, usize extra, std::initializer_list<type2> elements_to_assign, usize extra_to_assign) noexcept {
        constexpr const usize min_slack = 1;

        hud::vector<type1, hud_test::allocator_watcher<alignof(type1)>> assigned(elements_in_assigned, extra);
        hud::vector<type2, hud_test::allocator_watcher_2<alignof(type2)>> to_assign(elements_to_assign, extra_to_assign);

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;

        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }
        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0) {
            hud_assert_ne(assigned_buffer_address, to_assigned_buffer_address);
        }

        /****** void assign(vector &&other, const usize min_slack = 0) */
        assigned.assign(hud::move(to_assign), min_slack);

        // We should have the same count as element to assign
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));
        // We should have at least the min_slack as slack
        hud_assert_ge(assigned.slack(), min_slack);
        hud_assert_ge(assigned.max_count(), elements_to_assign.size() + min_slack);

        // Ensure elements are all copied correctly
        for (usize index = 0; index < assigned.count(); index++) {
            hud_assert_eq(assigned[index], static_cast<type1>(*(elements_to_assign.begin() + index)));
        }

        // As we move bitwise movable type we can just stole the pointer buffer
        // A reallocation is done only if the allocation can't be stolen ( Don't meet the min_slack requirements )
        // and if the size of the element in assigned buffer with the extra slack is not big enough to contains the element to assign + min_slack
        bool should_reallocate = elements_to_assign.size() + min_slack > elements_in_assigned.size() + extra;
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1u);
        }
        else {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count);
        }

        // We free the buffer in assigned element if we have elements and if we reallocate the buffer or we stole it
        if (elements_in_assigned.size() + extra > 0 && should_reallocate) {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
        }
        else {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count);
        }

        // Ensure that the moved vector is set to empty
        hud_assert_eq(to_assign.count(), 0u);
        hud_assert_eq(to_assign.max_count(), 0u);
        hud_assert_eq(to_assign.data(), nullptr);
        // Ensure we don't do extra allocation in the assigned vector
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().allocation_count(), 1u);
        }
        else {
            hud_assert_eq(to_assign.allocator().allocation_count(), 0u);
        }
        // Ensure we freed the buffer in the assigned vector except if we stole the buffer
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().free_count(), 1u);
        }
        else {

            hud_assert_eq(to_assign.allocator().free_count(), 0u);
        }
    };

    for (usize extra = 0; extra < 5; extra++) {
        for (usize extra2 = 0; extra2 < 5; extra2++) {
            test_assign({}, extra, {}, extra2);
            test_assign({}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {}, extra2);
        }
    }
}

// void assign(vector &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_non_bitwise_move_assignable_same_type_same_allocator_no_min_slack)
{

    using type = hud_test::non_bitwise_move_assignable_type;
    static_assert(!hud::is_bitwise_move_assignable_v<type>);

    auto test_assign = [](std::initializer_list<type> elements_in_assigned, usize extra, std::initializer_list<type> elements_to_assign, usize extra_to_assign) {
        // Ensure that all counters are set to 0
        for (const type &element : elements_in_assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }
        for (const type &element : elements_to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }

        hud::vector<type, hud_test::allocator_watcher<alignof(type)>> assigned(elements_in_assigned, extra);
        hud::vector<type, hud_test::allocator_watcher<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

        // Ensure assigned copy constructor is set to 1
        for (const type &element : assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }
        for (const type &element : to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }

        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0) {
            hud_assert_ne(assigned_buffer_address, to_assigned_buffer_address);
        }

        assigned.assign(hud::move(to_assign));

        // Number of element should be equal to the number of element in the std::initializer_list
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // Ensure we keep all allocated memory from the move pointer
        if (elements_in_assigned.size() + extra >= elements_to_assign.size()) {
            hud_assert_eq(assigned.max_count(), elements_in_assigned.size() + extra);
        }
        else {
            hud_assert_eq(assigned.max_count(), elements_to_assign.size());
        }

        // Ensures we move all values correctly
        for (usize index = 0; index < assigned.count(); index++) {
            hud_assert_eq(assigned[index].id(), (elements_to_assign.begin() + index)->id());
        }

        const bool should_reallocate = elements_to_assign.size() > (elements_in_assigned.size() + extra);

        // Allocation is supposed to reallocate if we assign more elements than was allocated
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1);
            if (previous_allocation_count > 0) {
                hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
            }
        }

        // Ensures we move all values correctly
        // If the a reallocation was done ( We must an vector with a bigger capacity )
        // All elements are destroyed and the move constructor of all elements is called instead of move assignement
        if (should_reallocate) {
            for (usize index = 0; index < assigned.count(); index++) {
                const type &element = assigned[index];
                hud_assert_eq(element.id(), (elements_to_assign.begin() + index)->id());
                hud_assert_eq(element.move_assign_count(), 0u);
                hud_assert_eq(element.move_constructor_count(), 1u);
                hud_assert_eq(element.copy_constructor_count(), 1u);
            }
        }
        // If not reallocation was done, all element in current count should call move assign,
        // else all elements assign after the count of elements before the assignement should be move construct
        else {
            for (usize index = 0; index < assigned.count(); index++) {
                const type &element = assigned[index];
                if (index < elements_in_assigned.size()) {
                    hud_assert_eq(element.move_assign_count(), 1u);
                    hud_assert_eq(element.move_constructor_count(), 0u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
                else {
                    hud_assert_eq(element.move_assign_count(), 0u);
                    hud_assert_eq(element.move_constructor_count(), 1u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
            }
        }

        // Ensure that the moved vector is set to empty
        hud_assert_eq(to_assign.count(), 0u);
        hud_assert_eq(to_assign.max_count(), 0u);
        hud_assert_eq(to_assign.data(), nullptr);

        // Ensure the moved vector was freed and only one allocation was done ( at initilisation of the vector )
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().allocation_count(), 1u);
            hud_assert_eq(to_assign.allocator().free_count(), 1u);
        }
        else {
            hud_assert_eq(to_assign.allocator().allocation_count(), 0u);
            hud_assert_eq(to_assign.allocator().free_count(), 0u);
        }
    };

    for (usize extra = 0; extra < 5; extra++) {
        for (usize extra2 = 0; extra2 < 5; extra2++) {
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

// void assign(vector &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_non_bitwise_move_assignable_same_type_same_allocator_with_min_slack)
{

    using type = hud_test::non_bitwise_move_assignable_type;
    static_assert(!hud::is_bitwise_move_assignable_v<type>);

    auto test_assign = [](std::initializer_list<type> elements_in_assigned, usize extra, std::initializer_list<type> elements_to_assign, usize extra_to_assign) {
        constexpr const usize min_slack = 1;

        // Ensure that all counters are set to 0
        for (const type &element : elements_in_assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }
        for (const type &element : elements_to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }

        hud::vector<type, hud_test::allocator_watcher<alignof(type)>> assigned(elements_in_assigned, extra);
        hud::vector<type, hud_test::allocator_watcher<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

        // Ensure assigned copy constructor is set to 1
        for (const type &element : assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }
        for (const type &element : to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;

        // if we have elements to copy, then 1 allocation should be dones
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }
        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0) {
            hud_assert_ne(assigned_buffer_address, to_assigned_buffer_address);
        }

        /****** void assign(vector &&other, const usize min_slack = 0) */
        assigned.assign(hud::move(to_assign), min_slack);

        // We should have the same count as element to assign
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));
        // We should have at least the min_slack as slack
        hud_assert_ge(assigned.slack(), min_slack);
        hud_assert_ge(assigned.max_count(), elements_to_assign.size() + min_slack);

        // Ensure elements are all copied correctly
        // If the a reallocation was done ( We must an vector with a bigger capacity )
        // All elements are destroyed and the move constructor of all elements is called instead of move assignement
        bool should_reallocate = elements_to_assign.size() + min_slack > elements_in_assigned.size() + extra;
        if (should_reallocate) {
            for (usize index = 0; index < assigned.count(); index++) {
                const type &element = assigned[index];
                hud_assert_eq(element.id(), (elements_to_assign.begin() + index)->id());
                hud_assert_eq(element.move_assign_count(), 0u);
                hud_assert_eq(element.move_constructor_count(), 1u);
                hud_assert_eq(element.copy_constructor_count(), 1u);
            }
        }
        // If not reallocation was done, all element in current count should call move assign,
        // else all elements assign after the count of elements before the assignement should be move construct
        else {
            for (usize index = 0; index < assigned.count(); index++) {
                const type &element = assigned[index];
                if (index < elements_in_assigned.size()) {
                    hud_assert_eq(element.move_assign_count(), 1u);
                    hud_assert_eq(element.move_constructor_count(), 0u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
                else {
                    hud_assert_eq(element.move_assign_count(), 0u);
                    hud_assert_eq(element.move_constructor_count(), 1u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
            }
        }

        // As we move bitwise movable type we can just stole the pointer buffer
        // A reallocation is done only if the allocation can't be stolen ( Don't meet the min_slack requirements )
        // and if the size of the element in assigned buffer with the extra slack is not big enough to contains the element to assign + min_slack
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1u);
        }
        else {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count);
        }

        // We free the buffer in assigned element if we have elements and if we reallocate the buffer or we stole it
        if (elements_in_assigned.size() + extra > 0 && should_reallocate) {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
        }
        else {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count);
        }

        // Ensure that the moved vector is set to empty
        hud_assert_eq(to_assign.count(), 0u);
        hud_assert_eq(to_assign.max_count(), 0u);
        hud_assert_eq(to_assign.data(), nullptr);
        // Ensure we don't do extra allocation in the assigned vector
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().allocation_count(), 1u);
        }
        else {
            hud_assert_eq(to_assign.allocator().allocation_count(), 0u);
        }
        // Ensure we freed the buffer in the assigned vector except if we stole the buffer
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().free_count(), 1u);
        }
        else {

            hud_assert_eq(to_assign.allocator().free_count(), 0u);
        }
    };

    for (usize extra = 0; extra < 5; extra++) {
        for (usize extra2 = 0; extra2 < 5; extra2++) {
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

// void assign(vector<u_type_t, u_allocator_t> &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_non_bitwise_move_assignable_same_type_different_allocator_no_min_slack)
{

    using type = hud_test::non_bitwise_move_assignable_type;
    static_assert(!hud::is_bitwise_move_assignable_v<type>);

    auto test_assign = [](std::initializer_list<type> elements_in_assigned, usize extra, std::initializer_list<type> elements_to_assign, usize extra_to_assign) {
        // Ensure that all counters are set to 0
        for (const type &element : elements_in_assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }
        for (const type &element : elements_to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }

        hud::vector<type, hud_test::allocator_watcher<alignof(type)>> assigned(elements_in_assigned, extra);
        hud::vector<type, hud_test::allocator_watcher_2<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

        // Ensure assigned copy constructor is set to 1
        for (const type &element : assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }
        for (const type &element : to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }

        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0) {
            hud_assert_ne(assigned_buffer_address, to_assigned_buffer_address);
        }

        assigned.assign(hud::move(to_assign));

        // Number of element should be equal to the number of element in the std::initializer_list
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // Ensure we keep all allocated memory from the move pointer
        if (elements_in_assigned.size() + extra >= elements_to_assign.size()) {
            hud_assert_eq(assigned.max_count(), elements_in_assigned.size() + extra);
        }
        else {
            hud_assert_eq(assigned.max_count(), elements_to_assign.size());
        }

        // Ensures we move all values correctly
        for (usize index = 0; index < assigned.count(); index++) {
            hud_assert_eq(assigned[index].id(), (elements_to_assign.begin() + index)->id());
        }

        const bool should_reallocate = elements_to_assign.size() > (elements_in_assigned.size() + extra);

        // Allocation is supposed to reallocate if we assign more elements than was allocated
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1);
            if (previous_allocation_count > 0) {
                hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
            }
        }

        // Ensures we move all values correctly
        // If the a reallocation was done ( We must an vector with a bigger capacity )
        // All elements are destroyed and the move constructor of all elements is called instead of move assignement
        if (should_reallocate) {
            for (usize index = 0; index < assigned.count(); index++) {
                const type &element = assigned[index];
                hud_assert_eq(element.id(), (elements_to_assign.begin() + index)->id());
                hud_assert_eq(element.move_assign_count(), 0u);
                hud_assert_eq(element.move_constructor_count(), 1u);
                hud_assert_eq(element.copy_constructor_count(), 1u);
            }
        }
        // If not reallocation was done, all element in current count should call move assign,
        // else all elements assign after the count of elements before the assignement should be move construct
        else {
            for (usize index = 0; index < assigned.count(); index++) {
                const type &element = assigned[index];
                if (index < elements_in_assigned.size()) {
                    hud_assert_eq(element.move_assign_count(), 1u);
                    hud_assert_eq(element.move_constructor_count(), 0u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
                else {
                    hud_assert_eq(element.move_assign_count(), 0u);
                    hud_assert_eq(element.move_constructor_count(), 1u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
            }
        }

        // Ensure that the moved vector is set to empty
        hud_assert_eq(to_assign.count(), 0u);
        hud_assert_eq(to_assign.max_count(), 0u);
        hud_assert_eq(to_assign.data(), nullptr);

        // Ensure the moved vector was freed and only one allocation was done ( at initilisation of the vector )
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().allocation_count(), 1u);
            hud_assert_eq(to_assign.allocator().free_count(), 1u);
        }
        else {
            hud_assert_eq(to_assign.allocator().allocation_count(), 0u);
            hud_assert_eq(to_assign.allocator().free_count(), 0u);
        }
    };

    for (usize extra = 0; extra < 5; extra++) {
        for (usize extra2 = 0; extra2 < 5; extra2++) {
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

// void assign(vector<u_type_t, u_allocator_t> &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_non_bitwise_move_assignable_same_type_different_allocator_with_min_slack)
{

    using type = hud_test::non_bitwise_move_assignable_type;
    static_assert(!hud::is_bitwise_move_assignable_v<type>);

    auto test_assign = [](std::initializer_list<type> elements_in_assigned, usize extra, std::initializer_list<type> elements_to_assign, usize extra_to_assign) {
        constexpr const usize min_slack = 1;

        // Ensure that all counters are set to 0
        for (const type &element : elements_in_assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }
        for (const type &element : elements_to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }

        hud::vector<type, hud_test::allocator_watcher<alignof(type)>> assigned(elements_in_assigned, extra);
        hud::vector<type, hud_test::allocator_watcher_2<alignof(type)>> to_assign(elements_to_assign, extra_to_assign);

        // Ensure assigned copy constructor is set to 1
        for (const type &element : assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }
        for (const type &element : to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;

        // if we have elements to copy, then 1 allocation should be dones
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }
        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0) {
            hud_assert_ne(assigned_buffer_address, to_assigned_buffer_address);
        }

        /****** void assign(vector &&other, const usize min_slack = 0) */
        assigned.assign(hud::move(to_assign), min_slack);

        // We should have the same count as element to assign
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));
        // We should have at least the min_slack as slack
        hud_assert_ge(assigned.slack(), min_slack);
        hud_assert_ge(assigned.max_count(), elements_to_assign.size() + min_slack);

        // Ensure elements are all copied correctly
        // If the a reallocation was done ( We must an vector with a bigger capacity )
        // All elements are destroyed and the move constructor of all elements is called instead of move assignement
        bool should_reallocate = elements_to_assign.size() + min_slack > elements_in_assigned.size() + extra;
        if (should_reallocate) {
            for (usize index = 0; index < assigned.count(); index++) {
                const type &element = assigned[index];
                hud_assert_eq(element.id(), (elements_to_assign.begin() + index)->id());
                hud_assert_eq(element.move_assign_count(), 0u);
                hud_assert_eq(element.move_constructor_count(), 1u);
                hud_assert_eq(element.copy_constructor_count(), 1u);
            }
        }
        // If not reallocation was done, all element in current count should call move assign,
        // else all elements assign after the count of elements before the assignement should be move construct
        else {
            for (usize index = 0; index < assigned.count(); index++) {
                const type &element = assigned[index];
                if (index < elements_in_assigned.size()) {
                    hud_assert_eq(element.move_assign_count(), 1u);
                    hud_assert_eq(element.move_constructor_count(), 0u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
                else {
                    hud_assert_eq(element.move_assign_count(), 0u);
                    hud_assert_eq(element.move_constructor_count(), 1u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
            }
        }

        // As we move bitwise movable type we can just stole the pointer buffer
        // A reallocation is done only if the allocation can't be stolen ( Don't meet the min_slack requirements )
        // and if the size of the element in assigned buffer with the extra slack is not big enough to contains the element to assign + min_slack
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1u);
        }
        else {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count);
        }

        // We free the buffer in assigned element if we have elements and if we reallocate the buffer or we stole it
        if (elements_in_assigned.size() + extra > 0 && should_reallocate) {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
        }
        else {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count);
        }

        // Ensure that the moved vector is set to empty
        hud_assert_eq(to_assign.count(), 0u);
        hud_assert_eq(to_assign.max_count(), 0u);
        hud_assert_eq(to_assign.data(), nullptr);
        // Ensure we don't do extra allocation in the assigned vector
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().allocation_count(), 1u);
        }
        else {
            hud_assert_eq(to_assign.allocator().allocation_count(), 0u);
        }
        // Ensure we freed the buffer in the assigned vector except if we stole the buffer
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().free_count(), 1u);
        }
        else {

            hud_assert_eq(to_assign.allocator().free_count(), 0u);
        }
    };

    for (usize extra = 0; extra < 5; extra++) {
        for (usize extra2 = 0; extra2 < 5; extra2++) {
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

// void assign(vector<u_type_t, u_allocator_t> &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_non_bitwise_move_assignable_different_type_same_allocator_no_min_slack)
{

    using type1 = hud_test::non_bitwise_move_assignable_type2;
    using type2 = hud_test::non_bitwise_move_assignable_type;
    // Ensure we test with different types
    static_assert(!std::is_same_v<type1, type2>);
    static_assert(!hud::is_bitwise_copy_assignable_v<type2, type1>);

    auto test_assign = [](std::initializer_list<type1> elements_in_assigned, usize extra, std::initializer_list<type2> elements_to_assign, usize extra_to_assign) {
        // Ensure that all counters are set to 0
        for (const type1 &element : elements_in_assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }
        for (const type2 &element : elements_to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }

        hud::vector<type1, hud_test::allocator_watcher<alignof(type1)>> assigned(elements_in_assigned, extra);
        hud::vector<type2, hud_test::allocator_watcher<alignof(type2)>> to_assign(elements_to_assign, extra_to_assign);

        // Ensure assigned copy constructor is set to 1
        for (const type1 &element : assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }
        for (const type2 &element : to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }

        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0) {
            hud_assert_ne(assigned_buffer_address, to_assigned_buffer_address);
        }

        assigned = hud::move(to_assign);

        // Number of element should be equal to the number of element in the std::initializer_list
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // Ensure we keep all allocated memory from the move pointer
        if (elements_in_assigned.size() + extra >= elements_to_assign.size()) {
            hud_assert_eq(assigned.max_count(), elements_in_assigned.size() + extra);
        }
        else {
            hud_assert_eq(assigned.max_count(), elements_to_assign.size());
        }

        // Ensures we move all values correctly
        for (usize index = 0; index < assigned.count(); index++) {
            hud_assert_eq(assigned[index].id(), (elements_to_assign.begin() + index)->id());
        }

        const bool should_reallocate = elements_to_assign.size() > (elements_in_assigned.size() + extra);

        // Allocation is supposed to reallocate if we assign more elements than was allocated
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1);
            if (previous_allocation_count > 0) {
                hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
            }
        }

        // Ensures we move all values correctly
        // If the a reallocation was done ( We must an vector with a bigger capacity )
        // All elements are destroyed and the move constructor of all elements is called instead of move assignement
        if (should_reallocate) {
            for (usize index = 0; index < assigned.count(); index++) {
                const type1 &element = assigned[index];
                hud_assert_eq(element.id(), (elements_to_assign.begin() + index)->id());
                hud_assert_eq(element.move_assign_count(), 0u);
                hud_assert_eq(element.move_constructor_count(), 1u);
                hud_assert_eq(element.copy_constructor_count(), 1u);
            }
        }
        // If not reallocation was done, all element in current count should call move assign,
        // else all elements assign after the count of elements before the assignement should be move construct
        else {
            for (usize index = 0; index < assigned.count(); index++) {
                const type1 &element = assigned[index];
                if (index < elements_in_assigned.size()) {
                    hud_assert_eq(element.move_assign_count(), 1u);
                    hud_assert_eq(element.move_constructor_count(), 0u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
                else {
                    hud_assert_eq(element.move_assign_count(), 0u);
                    hud_assert_eq(element.move_constructor_count(), 1u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
            }
        }

        // Ensure that the moved vector is set to empty
        hud_assert_eq(to_assign.count(), 0u);
        hud_assert_eq(to_assign.max_count(), 0u);
        hud_assert_eq(to_assign.data(), nullptr);

        // Ensure the moved vector was freed and only one allocation was done ( at initilisation of the vector )
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().allocation_count(), 1u);
            hud_assert_eq(to_assign.allocator().free_count(), 1u);
        }
        else {
            hud_assert_eq(to_assign.allocator().allocation_count(), 0u);
            hud_assert_eq(to_assign.allocator().free_count(), 0u);
        }
    };

    for (usize extra = 0; extra < 5; extra++) {
        for (usize extra2 = 0; extra2 < 5; extra2++) {
            test_assign({}, extra, {}, extra2);
            test_assign({}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {}, extra2);
        }
    }
}

// void assign(vector<u_type_t, u_allocator_t> &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_non_bitwise_move_assignable_different_type_same_allocator_with_min_slack)
{

    using type1 = hud_test::non_bitwise_move_assignable_type2;
    using type2 = hud_test::non_bitwise_move_assignable_type;
    // Ensure we test with different types
    static_assert(!std::is_same_v<type1, type2>);
    static_assert(!hud::is_bitwise_copy_assignable_v<type2, type1>);

    auto test_assign = [](std::initializer_list<type1> elements_in_assigned, usize extra, std::initializer_list<type2> elements_to_assign, usize extra_to_assign) {
        constexpr const usize min_slack = 1;
        // Ensure that all counters are set to 0
        for (const type1 &element : elements_in_assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }
        for (const type2 &element : elements_to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }

        hud::vector<type1, hud_test::allocator_watcher<alignof(type1)>> assigned(elements_in_assigned, extra);
        hud::vector<type2, hud_test::allocator_watcher<alignof(type2)>> to_assign(elements_to_assign, extra_to_assign);

        // Ensure assigned copy constructor is set to 1
        for (const type1 &element : assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }
        for (const type2 &element : to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;

        // if we have elements to copy, then 1 allocation should be dones
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }
        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0) {
            hud_assert_ne(assigned_buffer_address, to_assigned_buffer_address);
        }

        /****** void assign(vector &&other, const usize min_slack = 0) */
        assigned.assign(hud::move(to_assign), min_slack);

        // We should have the same count as element to assign
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));
        // We should have at least the min_slack as slack
        hud_assert_ge(assigned.slack(), min_slack);
        hud_assert_ge(assigned.max_count(), elements_to_assign.size() + min_slack);

        // Ensure elements are all copied correctly
        // If the a reallocation was done ( We must an vector with a bigger capacity )
        // All elements are destroyed and the move constructor of all elements is called instead of move assignement
        bool should_reallocate = elements_to_assign.size() + min_slack > elements_in_assigned.size() + extra;
        if (should_reallocate) {
            for (usize index = 0; index < assigned.count(); index++) {
                const type1 &element = assigned[index];
                hud_assert_eq(element.id(), (elements_to_assign.begin() + index)->id());
                hud_assert_eq(element.move_assign_count(), 0u);
                hud_assert_eq(element.move_constructor_count(), 1u);
                hud_assert_eq(element.copy_constructor_count(), 1u);
            }
        }
        // If not reallocation was done, all element in current count should call move assign,
        // else all elements assign after the count of elements before the assignement should be move construct
        else {
            for (usize index = 0; index < assigned.count(); index++) {
                const type2 &element = assigned[index];
                if (index < elements_in_assigned.size()) {
                    hud_assert_eq(element.move_assign_count(), 1u);
                    hud_assert_eq(element.move_constructor_count(), 0u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
                else {
                    hud_assert_eq(element.move_assign_count(), 0u);
                    hud_assert_eq(element.move_constructor_count(), 1u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
            }
        }

        // As we move bitwise movable type we can just stole the pointer buffer
        // A reallocation is done only if the allocation can't be stolen ( Don't meet the min_slack requirements )
        // and if the size of the element in assigned buffer with the extra slack is not big enough to contains the element to assign + min_slack
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1u);
        }
        else {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count);
        }

        // We free the buffer in assigned element if we have elements and if we reallocate the buffer or we stole it
        if (elements_in_assigned.size() + extra > 0 && should_reallocate) {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
        }
        else {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count);
        }

        // Ensure that the moved vector is set to empty
        hud_assert_eq(to_assign.count(), 0u);
        hud_assert_eq(to_assign.max_count(), 0u);
        hud_assert_eq(to_assign.data(), nullptr);
        // Ensure we don't do extra allocation in the assigned vector
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().allocation_count(), 1u);
        }
        else {
            hud_assert_eq(to_assign.allocator().allocation_count(), 0u);
        }
        // Ensure we freed the buffer in the assigned vector except if we stole the buffer
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().free_count(), 1u);
        }
        else {

            hud_assert_eq(to_assign.allocator().free_count(), 0u);
        }
    };

    for (usize extra = 0; extra < 5; extra++) {
        for (usize extra2 = 0; extra2 < 5; extra2++) {
            test_assign({}, extra, {}, extra2);
            test_assign({}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {}, extra2);
        }
    }
}

// void assign(vector<u_type_t, u_allocator_t> &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_non_bitwise_move_assignable_different_type_different_allocator_no_min_slack)
{

    using type1 = hud_test::non_bitwise_move_assignable_type2;
    using type2 = hud_test::non_bitwise_move_assignable_type;
    // Ensure we test with different types
    static_assert(!std::is_same_v<type1, type2>);
    static_assert(!hud::is_bitwise_copy_assignable_v<type2, type1>);

    auto test_assign = [](std::initializer_list<type1> elements_in_assigned, usize extra, std::initializer_list<type2> elements_to_assign, usize extra_to_assign) {
        // Ensure that all counters are set to 0
        for (const type1 &element : elements_in_assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }
        for (const type2 &element : elements_to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }

        hud::vector<type1, hud_test::allocator_watcher<alignof(type1)>> assigned(elements_in_assigned, extra);
        hud::vector<type2, hud_test::allocator_watcher_2<alignof(type2)>> to_assign(elements_to_assign, extra_to_assign);

        // Ensure assigned copy constructor is set to 1
        for (const type1 &element : assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }
        for (const type2 &element : to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }

        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0) {
            hud_assert_ne(assigned_buffer_address, to_assigned_buffer_address);
        }

        assigned.assign(hud::move(to_assign));

        // Number of element should be equal to the number of element in the std::initializer_list
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // Ensure we keep all allocated memory from the move pointer
        if (elements_in_assigned.size() + extra >= elements_to_assign.size()) {
            hud_assert_eq(assigned.max_count(), elements_in_assigned.size() + extra);
        }
        else {
            hud_assert_eq(assigned.max_count(), elements_to_assign.size());
        }

        // Ensures we move all values correctly
        for (usize index = 0; index < assigned.count(); index++) {
            hud_assert_eq(assigned[index].id(), (elements_to_assign.begin() + index)->id());
        }

        const bool should_reallocate = elements_to_assign.size() > (elements_in_assigned.size() + extra);

        // Allocation is supposed to reallocate if we assign more elements than was allocated
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1);
            if (previous_allocation_count > 0) {
                hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
            }
        }

        // Ensures we move all values correctly
        // If the a reallocation was done ( We must an vector with a bigger capacity )
        // All elements are destroyed and the move constructor of all elements is called instead of move assignement
        if (should_reallocate) {
            for (usize index = 0; index < assigned.count(); index++) {
                const type1 &element = assigned[index];
                hud_assert_eq(element.id(), (elements_to_assign.begin() + index)->id());
                hud_assert_eq(element.move_assign_count(), 0u);
                hud_assert_eq(element.move_constructor_count(), 1u);
                hud_assert_eq(element.copy_constructor_count(), 1u);
            }
        }
        // If not reallocation was done, all element in current count should call move assign,
        // else all elements assign after the count of elements before the assignement should be move construct
        else {
            for (usize index = 0; index < assigned.count(); index++) {
                const type1 &element = assigned[index];
                if (index < elements_in_assigned.size()) {
                    hud_assert_eq(element.move_assign_count(), 1u);
                    hud_assert_eq(element.move_constructor_count(), 0u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
                else {
                    hud_assert_eq(element.move_assign_count(), 0u);
                    hud_assert_eq(element.move_constructor_count(), 1u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
            }
        }

        // Ensure that the moved vector is set to empty
        hud_assert_eq(to_assign.count(), 0u);
        hud_assert_eq(to_assign.max_count(), 0u);
        hud_assert_eq(to_assign.data(), nullptr);

        // Ensure the moved vector was freed and only one allocation was done ( at initilisation of the vector )
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().allocation_count(), 1u);
            hud_assert_eq(to_assign.allocator().free_count(), 1u);
        }
        else {
            hud_assert_eq(to_assign.allocator().allocation_count(), 0u);
            hud_assert_eq(to_assign.allocator().free_count(), 0u);
        }
    };

    for (usize extra = 0; extra < 5; extra++) {
        for (usize extra2 = 0; extra2 < 5; extra2++) {
            test_assign({}, extra, {}, extra2);
            test_assign({}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {}, extra2);
        }
    }
}

// void assign(vector<u_type_t, u_allocator_t> &&other, const usize min_slack = 0)
GTEST_TEST(vector, move_assign_array_of_non_bitwise_move_assignable_different_type_different_allocator_with_min_slack)
{

    using type1 = hud_test::non_bitwise_move_assignable_type2;
    using type2 = hud_test::non_bitwise_move_assignable_type;
    // Ensure we test with different types
    static_assert(!std::is_same_v<type1, type2>);
    static_assert(!hud::is_bitwise_copy_assignable_v<type2, type1>);

    auto test_assign = [](std::initializer_list<type1> elements_in_assigned, usize extra, std::initializer_list<type2> elements_to_assign, usize extra_to_assign) {
        constexpr const usize min_slack = 1;

        // Ensure that all counters are set to 0
        for (const type1 &element : elements_in_assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }
        for (const type2 &element : elements_to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 0u);
        }

        hud::vector<type1, hud_test::allocator_watcher<alignof(type1)>> assigned(elements_in_assigned, extra);
        hud::vector<type2, hud_test::allocator_watcher_2<alignof(type2)>> to_assign(elements_to_assign, extra_to_assign);

        // Ensure assigned copy constructor is set to 1
        for (const type1 &element : assigned) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }
        for (const type2 &element : to_assign) {
            hud_assert_eq(element.move_assign_count(), 0u);
            hud_assert_eq(element.move_constructor_count(), 0u);
            hud_assert_eq(element.copy_constructor_count(), 1u);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;

        // if we have elements to copy, then 1 allocation should be dones
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }
        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));
        const uptr assigned_buffer_address = reinterpret_cast<const uptr>(assigned.data());
        const uptr to_assigned_buffer_address = reinterpret_cast<const uptr>(to_assign.data());
        if (assigned_buffer_address != 0 && to_assigned_buffer_address != 0) {
            hud_assert_ne(assigned_buffer_address, to_assigned_buffer_address);
        }

        /****** void assign(vector &&other, const usize min_slack = 0) */
        assigned.assign(hud::move(to_assign), min_slack);

        // We should have the same count as element to assign
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));
        // We should have at least the min_slack as slack
        hud_assert_ge(assigned.slack(), min_slack);
        hud_assert_ge(assigned.max_count(), elements_to_assign.size() + min_slack);

        // Ensure elements are all copied correctly
        // If the a reallocation was done ( We must an vector with a bigger capacity )
        // All elements are destroyed and the move constructor of all elements is called instead of move assignement
        bool should_reallocate = elements_to_assign.size() + min_slack > elements_in_assigned.size() + extra;
        if (should_reallocate) {
            for (usize index = 0; index < assigned.count(); index++) {
                const type1 &element = assigned[index];
                hud_assert_eq(element.id(), (elements_to_assign.begin() + index)->id());
                hud_assert_eq(element.move_assign_count(), 0u);
                hud_assert_eq(element.move_constructor_count(), 1u);
                hud_assert_eq(element.copy_constructor_count(), 1u);
            }
        }
        // If not reallocation was done, all element in current count should call move assign,
        // else all elements assign after the count of elements before the assignement should be move construct
        else {
            for (usize index = 0; index < assigned.count(); index++) {
                const type2 &element = assigned[index];
                if (index < elements_in_assigned.size()) {
                    hud_assert_eq(element.move_assign_count(), 1u);
                    hud_assert_eq(element.move_constructor_count(), 0u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
                else {
                    hud_assert_eq(element.move_assign_count(), 0u);
                    hud_assert_eq(element.move_constructor_count(), 1u);
                    hud_assert_eq(element.copy_constructor_count(), 1u);
                }
            }
        }

        // As we move bitwise movable type we can just stole the pointer buffer
        // A reallocation is done only if the allocation can't be stolen ( Don't meet the min_slack requirements )
        // and if the size of the element in assigned buffer with the extra slack is not big enough to contains the element to assign + min_slack
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1u);
        }
        else {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count);
        }

        // We free the buffer in assigned element if we have elements and if we reallocate the buffer or we stole it
        if (elements_in_assigned.size() + extra > 0 && should_reallocate) {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
        }
        else {
            hud_assert_eq(assigned.allocator().free_count(), previous_free_count);
        }

        // Ensure that the moved vector is set to empty
        hud_assert_eq(to_assign.count(), 0u);
        hud_assert_eq(to_assign.max_count(), 0u);
        hud_assert_eq(to_assign.data(), nullptr);
        // Ensure we don't do extra allocation in the assigned vector
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().allocation_count(), 1u);
        }
        else {
            hud_assert_eq(to_assign.allocator().allocation_count(), 0u);
        }
        // Ensure we freed the buffer in the assigned vector except if we stole the buffer
        if ((elements_to_assign.size() + extra_to_assign) > 0) {
            hud_assert_eq(to_assign.allocator().free_count(), 1u);
        }
        else {

            hud_assert_eq(to_assign.allocator().free_count(), 0u);
        }
    };

    for (usize extra = 0; extra < 5; extra++) {
        for (usize extra2 = 0; extra2 < 5; extra2++) {
            test_assign({}, extra, {}, extra2);
            test_assign({}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}, type2 {2}, type2 {3}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {type2 {0}, type2 {1}}, extra2);
            test_assign({type1 {10}, type1 {20}, type1 {30}, type1 {40}, type1 {50}}, extra, {}, extra2);
        }
    }
}

// vector &operator=(vector &&other)
GTEST_TEST(vector, move_assign_array_same_allocator_call_destructor_of_elements_no_min_slack)
{

    auto test_assign = [](std::initializer_list<i32 *> &&elements_in_assigned, usize extra, std::initializer_list<i32 *> elements_to_assign) {
        // Ensure that all destructors is not called
        for (i32 *destructor_counter : elements_in_assigned) {
            hud_assert_ne(destructor_counter, nullptr);
            *destructor_counter = 0;
        }
        for (i32 *destructor_counter : elements_to_assign) {
            hud_assert_ne(destructor_counter, nullptr);
            *destructor_counter = 0;
        }

        hud::vector<hud_test::SetBoolToTrueWhenDestroyed, hud_test::allocator_watcher<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> assigned(elements_in_assigned, extra);
        hud::vector<hud_test::SetBoolToTrueWhenDestroyed, hud_test::allocator_watcher<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> to_assign(elements_to_assign);

        // Ensure assigned copy constructor is set to 1
        for (const hud_test::SetBoolToTrueWhenDestroyed &element : assigned) {
            hud_assert_ne(element.ptr(), nullptr);
            hud_assert_eq(*element.ptr(), 0);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // If we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }

        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));

        assigned.assign(hud::move(to_assign));

        // Allocation should never be released if already done
        // The only case we have no allocation at all is when assigning empty std::initializer_list to empty vector
        if ((elements_in_assigned.size() + extra) == 0 && elements_to_assign.size() == 0) {
            hud_assert_eq(assigned.data(), nullptr);
        }
        else {
            hud_assert_ne(assigned.data(), nullptr);
        }

        // Number of element should be equal to the number of element in the std::initializer_list
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // Ensure we do no reallocate for less memory
        if (elements_in_assigned.size() + extra >= elements_to_assign.size()) {
            hud_assert_eq(assigned.max_count(), elements_in_assigned.size() + extra);
        }
        else {
            hud_assert_eq(assigned.max_count(), elements_to_assign.size());
        }

        // Allocation is supposed to reallocate if we assign more elements than was allocated
        const bool should_reallocate = elements_to_assign.size() > (elements_in_assigned.size() + extra);
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1);
            if (previous_allocation_count > 0) {
                hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
            }
        }

        // Ensure we called destructors
        // If the a reallocation was done,
        // All elements are destroyed
        if (should_reallocate) {
            for (const i32 *element : elements_in_assigned) {
                hud_assert_eq(*element, 1);
            }
        }
        // If no reallocation was done, all elements in current count should not be destroyed
        else {
            for (usize index = 0; index < elements_in_assigned.size(); index++) {
                if (index < elements_to_assign.size()) {
                    hud_assert_eq(*(*(elements_in_assigned.begin() + index)), 0);
                }
                else {
                    hud_assert_eq(*(*(elements_in_assigned.begin() + index)), 1);
                }
            }
        }

        // Ensure the copied elements are not destroyed
        for (usize index = 0; index < elements_to_assign.size(); index++) {
            hud_assert_eq(*(*(elements_to_assign.begin() + index)), 0);
        }
    };

    i32 assigned[5];
    i32 to_assign[5];

    for (usize extra = 0; extra < 5; extra++) {
        test_assign({}, extra, {});
        test_assign({}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {&to_assign[0], &to_assign[1]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {});
    }
}

// vector &operator=(vector &&other)
GTEST_TEST(vector, move_assign_array_same_allocator_call_destructor_of_elements_with_min_slack)
{

    auto test_assign = [](std::initializer_list<i32 *> &&elements_in_assigned, usize extra, std::initializer_list<i32 *> elements_to_assign) {
        constexpr const usize min_slack = 1;
        // Ensure that all destructors is not called
        for (i32 *destructor_counter : elements_in_assigned) {
            hud_assert_ne(destructor_counter, nullptr);
            *destructor_counter = 0;
        }
        for (i32 *destructor_counter : elements_to_assign) {
            hud_assert_ne(destructor_counter, nullptr);
            *destructor_counter = 0;
        }

        hud::vector<hud_test::SetBoolToTrueWhenDestroyed, hud_test::allocator_watcher<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> assigned(elements_in_assigned, extra);
        hud::vector<hud_test::SetBoolToTrueWhenDestroyed, hud_test::allocator_watcher<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> to_assign(elements_to_assign);

        // Ensure assigned copy constructor is set to 1
        for (const hud_test::SetBoolToTrueWhenDestroyed &element : assigned) {
            hud_assert_ne(element.ptr(), nullptr);
            hud_assert_eq(*element.ptr(), 0);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // If we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }

        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));

        assigned.assign(hud::move(to_assign), min_slack);

        // Allocation should never be released if already done
        // The only case we have no allocation at all is when assigning empty std::initializer_list to empty vector
        if ((elements_in_assigned.size() + extra) == 0 && (elements_to_assign.size() + min_slack) == 0) {
            hud_assert_eq(assigned.data(), nullptr);
        }
        else {
            hud_assert_ne(assigned.data(), nullptr);
        }

        // Number of element should be equal to the number of element in the std::initializer_list
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // Ensure we do no reallocate for less memory
        if (elements_in_assigned.size() + extra >= elements_to_assign.size() + min_slack) {
            hud_assert_eq(assigned.max_count(), elements_in_assigned.size() + extra);
        }
        else {
            hud_assert_eq(assigned.max_count(), elements_to_assign.size() + min_slack);
        }

        // Allocation is supposed to reallocate if we assign more elements than was allocated
        const bool should_reallocate = (elements_to_assign.size() + min_slack) > (elements_in_assigned.size() + extra);
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1);
            if (previous_allocation_count > 0) {
                hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
            }
        }

        // Ensure we called destructors
        // If the a reallocation was done,
        // All elements are destroyed
        if (should_reallocate) {
            for (const i32 *element : elements_in_assigned) {
                hud_assert_eq(*element, 1);
            }
        }
        // If no reallocation was done, all elements in current count should not be destroyed
        else {
            for (usize index = 0; index < elements_in_assigned.size(); index++) {
                if (index < elements_to_assign.size()) {
                    hud_assert_eq(*(*(elements_in_assigned.begin() + index)), 0);
                }
                else {
                    hud_assert_eq(*(*(elements_in_assigned.begin() + index)), 1);
                }
            }
        }

        // Ensure the copied elements are not destroyed
        for (usize index = 0; index < elements_to_assign.size(); index++) {
            hud_assert_eq(*(*(elements_to_assign.begin() + index)), 0);
        }
    };

    i32 assigned[5];
    i32 to_assign[5];

    for (usize extra = 0; extra < 5; extra++) {
        test_assign({}, extra, {});
        test_assign({}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {&to_assign[0], &to_assign[1]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {});
    }
}

// vector &operator=(vector<u_type_t, u_allocator_t> &&other)
GTEST_TEST(vector, move_assign_array_different_allocator_call_destructor_of_elements_no_min_slack)
{

    auto test_assign = [](std::initializer_list<i32 *> &&elements_in_assigned, usize extra, std::initializer_list<i32 *> elements_to_assign) {
        // Ensure that all destructors is not called
        for (i32 *destructor_counter : elements_in_assigned) {
            hud_assert_ne(destructor_counter, nullptr);
            *destructor_counter = 0;
        }
        for (i32 *destructor_counter : elements_to_assign) {
            hud_assert_ne(destructor_counter, nullptr);
            *destructor_counter = 0;
        }

        hud::vector<hud_test::SetBoolToTrueWhenDestroyed, hud_test::allocator_watcher<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> assigned(elements_in_assigned, extra);
        hud::vector<hud_test::SetBoolToTrueWhenDestroyed, hud_test::allocator_watcher_2<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> to_assign(elements_to_assign);

        // Ensure assigned copy constructor is set to 1
        for (const hud_test::SetBoolToTrueWhenDestroyed &element : assigned) {
            hud_assert_ne(element.ptr(), nullptr);
            hud_assert_eq(*element.ptr(), 0);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }

        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));

        assigned.assign(hud::move(to_assign));

        // Allocation should never be released if already done
        // The only case we have no allocation at all is when assigning empty std::initializer_list to empty vector
        if ((elements_in_assigned.size() + extra) == 0 && elements_to_assign.size() == 0) {
            hud_assert_eq(assigned.data(), nullptr);
        }
        else {
            hud_assert_ne(assigned.data(), nullptr);
        }

        // Number of element should be equal to the number of element in the std::initializer_list
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // Ensure we do no reallocate for less memory
        if (elements_in_assigned.size() + extra >= elements_to_assign.size()) {
            hud_assert_eq(assigned.max_count(), elements_in_assigned.size() + extra);
        }
        else {
            hud_assert_eq(assigned.max_count(), elements_to_assign.size());
        }

        // Allocation is supposed to reallocate if we assign more elements than was allocated
        const bool should_reallocate = elements_to_assign.size() > (elements_in_assigned.size() + extra);
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1);
            if (previous_allocation_count > 0) {
                hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
            }
        }

        // Ensure we called destructors
        // If the a reallocation was done,
        // All elements are destroyed
        if (should_reallocate) {
            for (const i32 *element : elements_in_assigned) {
                hud_assert_eq(*element, 1);
            }
        }
        // If no reallocation was done, all elements in current count should not be destroyed
        else {
            for (usize index = 0; index < elements_in_assigned.size(); index++) {
                if (index < elements_to_assign.size()) {
                    hud_assert_eq(*(*(elements_in_assigned.begin() + index)), 0);
                }
                else {
                    hud_assert_eq(*(*(elements_in_assigned.begin() + index)), 1);
                }
            }
        }

        // Ensure the copied elements are not destroyed
        for (usize index = 0; index < elements_to_assign.size(); index++) {
            hud_assert_eq(*(*(elements_to_assign.begin() + index)), 0);
        }
    };

    i32 assigned[5];
    i32 to_assign[5];

    for (usize extra = 0; extra < 5; extra++) {
        test_assign({}, extra, {});
        test_assign({}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {&to_assign[0], &to_assign[1]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {});
    }
}

// vector &operator=(vector<u_type_t, u_allocator_t> &&other)
GTEST_TEST(vector, move_assign_array_different_allocator_call_destructor_of_elements_with_min_slack)
{

    auto test_assign = [](std::initializer_list<i32 *> &&elements_in_assigned, usize extra, std::initializer_list<i32 *> elements_to_assign) {
        constexpr const usize min_slack = 1;
        // Ensure that all destructors is not called
        for (i32 *destructor_counter : elements_in_assigned) {
            hud_assert_ne(destructor_counter, nullptr);
            *destructor_counter = 0;
        }
        for (i32 *destructor_counter : elements_to_assign) {
            hud_assert_ne(destructor_counter, nullptr);
            *destructor_counter = 0;
        }

        hud::vector<hud_test::SetBoolToTrueWhenDestroyed, hud_test::allocator_watcher<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> assigned(elements_in_assigned, extra);
        hud::vector<hud_test::SetBoolToTrueWhenDestroyed, hud_test::allocator_watcher_2<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> to_assign(elements_to_assign);

        // Ensure assigned copy constructor is set to 1
        for (const hud_test::SetBoolToTrueWhenDestroyed &element : assigned) {
            hud_assert_ne(element.ptr(), nullptr);
            hud_assert_eq(*element.ptr(), 0);
        }

        u32 previous_allocation_count = 0;
        u32 previous_free_count = 0;
        // if we have elements to copy, then 1 allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            // Ensure we are allocating only one time
            hud_assert_eq(assigned.allocator().allocation_count(), 1u);
            hud_assert_eq(assigned.allocator().free_count(), 0u);
            previous_allocation_count = assigned.allocator().allocation_count();
            previous_free_count = assigned.allocator().free_count();
        }

        // If we have elements inside allocation should be done
        if ((elements_in_assigned.size() + extra) > 0) {
            hud_assert_ne(assigned.data(), nullptr);
        }
        else {
            hud_assert_eq(assigned.data(), nullptr);
        }
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_in_assigned.size()));
        hud_assert_eq(assigned.max_count(), static_cast<usize>(elements_in_assigned.size() + extra));

        assigned.assign(hud::move(to_assign), min_slack);

        // Allocation should never be released if already done
        // The only case we have no allocation at all is when assigning empty std::initializer_list to empty vector
        if ((elements_in_assigned.size() + extra) == 0 && (elements_to_assign.size() + min_slack) == 0) {
            hud_assert_eq(assigned.data(), nullptr);
        }
        else {
            hud_assert_ne(assigned.data(), nullptr);
        }

        // Number of element should be equal to the number of element in the std::initializer_list
        hud_assert_eq(assigned.count(), static_cast<usize>(elements_to_assign.size()));

        // Ensure we do no reallocate for less memory
        if (elements_in_assigned.size() + extra >= elements_to_assign.size() + min_slack) {
            hud_assert_eq(assigned.max_count(), elements_in_assigned.size() + extra);
        }
        else {
            hud_assert_eq(assigned.max_count(), elements_to_assign.size() + min_slack);
        }

        // Allocation is supposed to reallocate if we assign more elements than was allocated
        const bool should_reallocate = elements_to_assign.size() + min_slack > (elements_in_assigned.size() + extra);
        if (should_reallocate) {
            hud_assert_eq(assigned.allocator().allocation_count(), previous_allocation_count + 1);
            if (previous_allocation_count > 0) {
                hud_assert_eq(assigned.allocator().free_count(), previous_free_count + 1);
            }
        }

        // Ensure we called destructors
        // If the a reallocation was done,
        // All elements are destroyed
        if (should_reallocate) {
            for (const i32 *element : elements_in_assigned) {
                hud_assert_eq(*element, 1);
            }
        }
        // If no reallocation was done, all elements in current count should not be destroyed
        else {
            for (usize index = 0; index < elements_in_assigned.size(); index++) {
                if (index < elements_to_assign.size()) {
                    hud_assert_eq(*(*(elements_in_assigned.begin() + index)), 0);
                }
                else {
                    hud_assert_eq(*(*(elements_in_assigned.begin() + index)), 1);
                }
            }
        }

        // Ensure the copied elements are not destroyed
        for (usize index = 0; index < elements_to_assign.size(); index++) {
            hud_assert_eq(*(*(elements_to_assign.begin() + index)), 0);
        }
    };

    i32 assigned[5];
    i32 to_assign[5];

    for (usize extra = 0; extra < 5; extra++) {
        test_assign({}, extra, {});
        test_assign({}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {&to_assign[0], &to_assign[1], &to_assign[2], &to_assign[3]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {&to_assign[0], &to_assign[1]});
        test_assign({&assigned[0], &assigned[1], &assigned[2], &assigned[3], &assigned[4]}, extra, {});
    }
}

// vector &operator=(vector &&other)
GTEST_TEST(vector, move_assign_array_of_bitwise_copy_assignable_to_self_no_min_slack)
{

    using type = i32;

    static_assert(hud::is_bitwise_copy_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> arr(elements_in_assigned);

            const type *data_before = arr.data();
            arr.assign(hud::move(arr));

            return std::tuple {
                arr.data() == data_before,
                // Number of element should be equal to the number of element in the std::initializer_list
                arr.count(),
                // Ensure we do no reallocate for less memory
                arr.max_count(),
                arr.allocator().allocation_count(),
                arr.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, const usize extra_in_assigned) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> arr(elements_in_assigned, extra_in_assigned);

            const type *data_before = arr.data();
            arr.assign(hud::move(arr));

            return std::tuple {
                arr.data() == data_before,
                // Number of element should be equal to the number of element in the std::initializer_list
                arr.count(),
                // Ensure we do no reallocate for less memory
                arr.max_count(),
                arr.allocator().allocation_count(),
                arr.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
        }
    }
}

// vector &operator=(vector &&other)
GTEST_TEST(vector, move_assign_array_of_bitwise_copy_assignable_to_self_with_min_slack)
{

    using type = i32;

    static_assert(hud::is_bitwise_copy_assignable_v<type>);
    constexpr const usize min_slack = 1;
    // Test without extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> arr(elements_in_assigned);

            const type *data_before = arr.data();
            arr.assign(hud::move(arr), min_slack);

            return std::tuple {
                arr.data() == data_before,
                // Number of element should be equal to the number of element in the std::initializer_list
                arr.count(),
                // Ensure we do no reallocate for less memory
                arr.max_count(),
                arr.allocator().allocation_count(),
                arr.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<type> elements_in_assigned, const usize extra_in_assigned) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> arr(elements_in_assigned, extra_in_assigned);

            const type *data_before = arr.data();
            arr.assign(hud::move(arr), min_slack);

            return std::tuple {
                arr.data() == data_before,
                // Number of element should be equal to the number of element in the std::initializer_list
                arr.count(),
                // Ensure we do no reallocate for less memory
                arr.max_count(),
                arr.allocator().allocation_count(),
                arr.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_eq(std::get<3>(result), 0u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_eq(std::get<3>(result), 1u);
                hud_assert_eq(std::get<4>(result), 0u);
            }
        }
    }
}

// vector &operator=(vector &&other)
GTEST_TEST(vector, move_assign_array_of_non_bitwise_copy_assignable_to_self_no_min_slack)
{

    using type = hud_test::non_bitwise_move_assignable_type;
    static_assert(!hud::is_bitwise_move_assignable_v<type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<i32> &&elements_in_assigned) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> arr(elements_in_assigned);
            const type *data_before = arr.data();
            arr.assign(hud::move(arr));

            // Ensures we copy all values correctly
            bool no_move_or_assign_are_called = true;
            for (usize index = 0; index < arr.count(); index++) {

                // Ensure we correctly call copy constructors
                // We do not reallocate
                // For element that are already here, we call operator=
                // LCOV_EXCL_START
                if (arr[index].copy_assign_count() != 0u && arr[index].move_assign_count() != 0u) {
                    no_move_or_assign_are_called = false;
                    break;
                }
                // LCOV_EXCL_STOP
            }

            return std::tuple {
                arr.data() == data_before,
                // Number of element should be equal to the number of element in the std::initializer_list
                arr.count(),
                // Ensure we do no reallocate for less memory
                arr.max_count(),
                no_move_or_assign_are_called,
                arr.allocator().allocation_count(),
                arr.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                const auto result = test({1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<i32> &&elements_in_assigned, const usize extra_in_assigned) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> arr(elements_in_assigned, extra_in_assigned);
            const type *data_before = arr.data();
            arr.assign(hud::move(arr));

            // Ensures we copy all values correctly
            bool no_move_or_assign_are_called = true;
            for (usize index = 0; index < arr.count(); index++) {
                // Ensure we correctly call copy constructors
                // We do not reallocate
                // For element that are already here, we call operator=
                // LCOV_EXCL_START
                if (arr[index].copy_assign_count() != 0u && arr[index].move_assign_count() != 0u) {
                    no_move_or_assign_are_called = false;
                    break;
                }
                // LCOV_EXCL_STOP
            }

            return std::tuple {
                arr.data() == data_before,
                // Number of element should be equal to the number of element in the std::initializer_list
                arr.count(),
                // Ensure we do no reallocate for less memory
                arr.max_count(),
                no_move_or_assign_are_called,
                arr.allocator().allocation_count(),
                arr.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                const auto result = test({}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
        }
    }
}

// vector &operator=(vector &&other)
GTEST_TEST(vector, move_assign_array_of_non_bitwise_copy_assignable_to_self_with_min_slack)
{

    using type = hud_test::non_bitwise_move_assignable_type;
    static_assert(!hud::is_bitwise_move_assignable_v<type>);
    constexpr const usize min_slack = 1;
    // Test without extra
    {
        const auto test = [](std::initializer_list<i32> &&elements_in_assigned) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> arr(elements_in_assigned);
            const type *data_before = arr.data();
            arr.assign(hud::move(arr), min_slack);

            // Ensures we copy all values correctly
            bool no_move_or_assign_are_called = true;
            for (usize index = 0; index < arr.count(); index++) {

                // Ensure we correctly call copy constructors
                // We do not reallocate
                // For element that are already here, we call operator=
                // LCOV_EXCL_START
                if (arr[index].copy_assign_count() != 0u && arr[index].move_assign_count() != 0u) {
                    no_move_or_assign_are_called = false;
                    break;
                }
                // LCOV_EXCL_STOP
            }

            return std::tuple {
                arr.data() == data_before,
                // Number of element should be equal to the number of element in the std::initializer_list
                arr.count(),
                // Ensure we do no reallocate for less memory
                arr.max_count(),
                no_move_or_assign_are_called,
                arr.allocator().allocation_count(),
                arr.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                const auto result = test({1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2});
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<i32> &&elements_in_assigned, const usize extra_in_assigned) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> arr(elements_in_assigned, extra_in_assigned);
            const type *data_before = arr.data();
            arr.assign(hud::move(arr), min_slack);

            // Ensures we copy all values correctly
            bool no_move_or_assign_are_called = true;
            for (usize index = 0; index < arr.count(); index++) {
                // Ensure we correctly call copy constructors
                // We do not reallocate
                // For element that are already here, we call operator=
                // LCOV_EXCL_START
                if (arr[index].copy_assign_count() != 0u && arr[index].move_assign_count() != 0u) {
                    no_move_or_assign_are_called = false;
                    break;
                }
                // LCOV_EXCL_STOP
            }

            return std::tuple {
                arr.data() == data_before,
                // Number of element should be equal to the number of element in the std::initializer_list
                arr.count(),
                // Ensure we do no reallocate for less memory
                arr.max_count(),
                no_move_or_assign_are_called,
                arr.allocator().allocation_count(),
                arr.allocator().free_count()
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                const auto result = test({}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                const auto result = test({1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 0u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 0u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 0u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 2u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                constexpr auto result = test({}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 0u);
                hud_assert_eq(std::get<2>(result), 1u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
            {
                constexpr auto result = test({1, 2}, 1u);
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), 2u);
                hud_assert_eq(std::get<2>(result), 3u);
                hud_assert_true(std::get<3>(result));
                hud_assert_eq(std::get<4>(result), 1u);
                hud_assert_eq(std::get<5>(result), 0u);
            }
        }
    }
}