#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"
#include <core/memory.h>

GTEST_TEST(vector, add_no_construct_do_not_call_constructor)
{
    using type = hud_test::NonDefaultConstructibleType;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;
    static_assert(!hud::is_constructible_v<type>);

    // test with reallocation
    {
        const auto test = []() {
            array_type vector;

            // Insert one element without constructing it
            const usize index_0 = vector.add_no_construct(2);
            const auto result_0 = std::tuple {
                index_0,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count()
            };

            // Construct only in constant_evaluated
            // Constant evaluation do not allowed to access non initialized memory
            if consteval {
                hud::memory::construct_array_at(vector.data(), vector.data() + 2, hud::i32_max);
            }

            // Insert one element without constructing it
            const usize index_1 = vector.add_no_construct(3);
            const auto result_1 = std::tuple {
                index_1,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count()
            };

            return std::tuple {
                result_0,
                result_1
            };
        };

        // Non constant
        {
            const auto result = test();
            const auto result_0 = std::get<0>(result);
            hud_assert_eq(std::get<0>(result_0), 0u);
            hud_assert_true(std::get<1>(result_0));
            hud_assert_eq(std::get<2>(result_0), 2u);
            hud_assert_eq(std::get<3>(result_0), 2u);
            hud_assert_eq(std::get<4>(result_0), 1u);
            hud_assert_eq(std::get<5>(result_0), 0u);

            const auto result_1 = std::get<1>(result);
            hud_assert_eq(std::get<0>(result_1), 2u);
            hud_assert_true(std::get<1>(result_1));
            hud_assert_eq(std::get<2>(result_1), 5u);
            hud_assert_eq(std::get<3>(result_1), 5u);
            hud_assert_eq(std::get<4>(result_1), 2u);
            hud_assert_eq(std::get<5>(result_1), 1u);
        }

        // Constant
        {
            constexpr auto result = test();
            const auto result_0 = std::get<0>(result);
            hud_assert_eq(std::get<0>(result_0), 0u);
            hud_assert_true(std::get<1>(result_0));
            hud_assert_eq(std::get<2>(result_0), 2u);
            hud_assert_eq(std::get<3>(result_0), 2u);
            hud_assert_eq(std::get<4>(result_0), 1u);
            hud_assert_eq(std::get<5>(result_0), 0u);

            const auto result_1 = std::get<1>(result);
            hud_assert_eq(std::get<0>(result_1), 2u);
            hud_assert_true(std::get<1>(result_1));
            hud_assert_eq(std::get<2>(result_1), 5u);
            hud_assert_eq(std::get<3>(result_1), 5u);
            hud_assert_eq(std::get<4>(result_1), 2u);
            hud_assert_eq(std::get<5>(result_1), 1u);
        }
    }

    // test without reallocation
    {
        const auto test = []() {
            array_type vector;
            vector.reserve(5);

            // Insert one element without constructing it
            const usize index_0 = vector.add_no_construct(2);
            const auto result_0 = std::tuple {
                index_0,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count()
            };

            // Construct only in constant_evaluated
            // Constant evaluation do not allowed to access non initialized memory
            if consteval {
                hud::memory::construct_array_at(vector.data(), vector.data() + 2, hud::i32_max);
            }

            // Insert one element without constructing it
            const usize index_1 = vector.add_no_construct(3);
            const auto result_1 = std::tuple {
                index_1,
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),
                vector.allocator().allocation_count(),
                vector.allocator().free_count()
            };

            return std::tuple {
                result_0,
                result_1
            };
        };

        // Non constant
        {
            const auto result = test();
            const auto result_0 = std::get<0>(result);
            hud_assert_eq(std::get<0>(result_0), 0u);
            hud_assert_true(std::get<1>(result_0));
            hud_assert_eq(std::get<2>(result_0), 2u);
            hud_assert_eq(std::get<3>(result_0), 5u);
            hud_assert_eq(std::get<4>(result_0), 1u);
            hud_assert_eq(std::get<5>(result_0), 0u);

            const auto result_1 = std::get<1>(result);
            hud_assert_eq(std::get<0>(result_1), 2u);
            hud_assert_true(std::get<1>(result_1));
            hud_assert_eq(std::get<2>(result_1), 5u);
            hud_assert_eq(std::get<3>(result_1), 5u);
            hud_assert_eq(std::get<4>(result_1), 1u);
            hud_assert_eq(std::get<5>(result_1), 0u);
        }

        // Constant
        {
            constexpr auto result = test();
            const auto result_0 = std::get<0>(result);
            hud_assert_eq(std::get<0>(result_0), 0u);
            hud_assert_true(std::get<1>(result_0));
            hud_assert_eq(std::get<2>(result_0), 2u);
            hud_assert_eq(std::get<3>(result_0), 5u);
            hud_assert_eq(std::get<4>(result_0), 1u);
            hud_assert_eq(std::get<5>(result_0), 0u);

            const auto result_1 = std::get<1>(result);
            hud_assert_eq(std::get<0>(result_1), 2u);
            hud_assert_true(std::get<1>(result_1));
            hud_assert_eq(std::get<2>(result_1), 5u);
            hud_assert_eq(std::get<3>(result_1), 5u);
            hud_assert_eq(std::get<4>(result_1), 1u);
            hud_assert_eq(std::get<5>(result_1), 0u);
        }
    }
}