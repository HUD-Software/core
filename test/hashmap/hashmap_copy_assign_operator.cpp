#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

#define TEST_VALUES { \
    {0,  0},           \
    {1, 11},          \
    {2, 22},          \
    {3, 33},          \
    {4, 44},          \
    {5, 55},          \
    {6, 66},          \
    {7, 77}           \
}

GTEST_TEST(hashmap, copy_assign_operator_bitwise_copy_constructible_same_type_same_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using key_type = i32;
    using value_type = i64;

    static_assert(hud::is_bitwise_copy_assignable_v<key_type>);
    static_assert(hud::is_bitwise_copy_assignable_v<value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using CopiedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the map
            NewType copy;
            copy = copied;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto &init_elem = (initializer.begin() + index);
                const auto it = copy.find(init_elem->first);
                if (it == copy.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_and_values_copied,                                                                                   // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 3
                copy.allocator().free_count() == 0                                                                            // 4
            };
        };

        // Non constant
        {
            const auto result_empty = test_default_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            const auto result = test_default_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_default_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            constexpr auto result = test_default_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the map
            NewType copy;
            copy = copied;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto &init_elem = (initializer.begin() + index);
                const auto it = copy.find(init_elem->first);
                if (it == copy.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() == copied.max_count(),                           // 1
                all_keys_and_values_copied,                                       // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
            };
        };

        // Non constant
        {
            const auto result_empty_no_extra = test_default_allocator({}, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra = test_default_allocator({}, 10u);
            hud_assert_true(std::get<0>(result_empty_extra));
            hud_assert_true(std::get<1>(result_empty_extra));
            hud_assert_true(std::get<2>(result_empty_extra));
            hud_assert_true(std::get<3>(result_empty_extra));
            hud_assert_true(std::get<4>(result_empty_extra));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra = test_default_allocator(TEST_VALUES, 10u);
            hud_assert_true(std::get<0>(result_extra));
            hud_assert_true(std::get<1>(result_extra));
            hud_assert_true(std::get<2>(result_extra));
            hud_assert_true(std::get<3>(result_extra));
            hud_assert_true(std::get<4>(result_extra));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            // constexpr auto result_empty_extra = test_default_allocator({}, 10u);
            // hud_assert_true(std::get<0>(result_empty_extra));
            // hud_assert_true(std::get<1>(result_empty_extra));
            // hud_assert_true(std::get<2>(result_empty_extra));
            // hud_assert_true(std::get<3>(result_empty_extra));
            // hud_assert_true(std::get<4>(result_empty_extra));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra = test_default_allocator(TEST_VALUES, 10u);
            hud_assert_true(std::get<0>(result_extra));
            hud_assert_true(std::get<1>(result_extra));
            hud_assert_true(std::get<2>(result_extra));
            hud_assert_true(std::get<3>(result_extra));
            hud_assert_true(std::get<4>(result_extra));
        }
    }
}