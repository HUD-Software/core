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

GTEST_TEST(hashmap, move_construct_bitwise_copy_constructible_same_type_same_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = i32;
    using value_type = i64;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(hud::is_bitwise_copy_constructible_v<value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    static_assert(hud::is_bitwise_copy_constructible_v<MovedType::slot_type>);

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer)
        {
            MovedType moved(initializer);

            // Move the map
            NewType move(hud::move(moved));

            // Ensure we move all elements
            bool all_keys_and_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto &init_elem = (initializer.begin() + index);
                const auto it = move.find(init_elem->first);
                if (it == move.end())
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                move.count() == initializer.size(),                                                                           // 0
                move.max_count() >= initializer.size(),                                                                       // 1
                moved.count() == 0,                                                                                           // 2
                moved.max_count() == 0,                                                                                       // 3
                all_keys_and_values_moved,                                                                                    // 4
                move.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 4 : 2) : 0), // 5
                move.allocator().free_count() == 0                                                                            // 6
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
            hud_assert_true(std::get<5>(result_empty));
            hud_assert_true(std::get<6>(result_empty));

            const auto result = test_default_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
        }

        // Constant
        {
            constexpr auto result_empty = test_default_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));
            hud_assert_true(std::get<5>(result_empty));
            hud_assert_true(std::get<6>(result_empty));

            constexpr auto result = test_default_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer)
        {
            MovedType moved(initializer);

            // Move the map
            NewType move(hud::move(moved), AllocatorType {});

            // Ensure we move all elements
            bool all_keys_and_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto &init_elem = (initializer.begin() + index);
                const auto it = move.find(init_elem->first);
                if (it == move.end())
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                move.count() == initializer.size(),                                                                           // 0
                move.max_count() >= initializer.size(),                                                                       // 1
                moved.count() == 0,                                                                                           // 2
                moved.max_count() == 0,                                                                                       // 3
                all_keys_and_values_moved,                                                                                    // 4
                move.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 5
                move.allocator().free_count() == 0                                                                            // 6
            };
        };

        // Non constant
        {
            const auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));
            hud_assert_true(std::get<5>(result_empty));
            hud_assert_true(std::get<6>(result_empty));

            const auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));
            hud_assert_true(std::get<5>(result_empty));
            hud_assert_true(std::get<6>(result_empty));

            constexpr auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize extra)
        {
            MovedType moved(initializer);

            // Move the map
            NewType move(hud::move(moved), extra);

            // Ensure we move all elements
            bool all_keys_and_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto &init_elem = (initializer.begin() + index);
                const auto it = move.find(init_elem->first);
                if (it == move.end())
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            // Allocation of the object to move
            if (initializer.size() > 0)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (initializer.size() > 0 || extra > 0)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }
            return std::tuple {
                move.count() == initializer.size(),                               // 0
                move.max_count() >= initializer.size(),                           // 1
                moved.count() == 0,                                               // 2
                moved.max_count() == 0,                                           // 3
                all_keys_and_values_moved,                                        // 4
                move.allocator().allocation_count() == expected_allocation_count, // 5
                move.allocator().free_count() == 0                                // 6
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
            hud_assert_true(std::get<5>(result_empty_no_extra));
            hud_assert_true(std::get<6>(result_empty_no_extra));

            const auto result_empty_extra = test_default_allocator({}, 10u);
            hud_assert_true(std::get<0>(result_empty_extra));
            hud_assert_true(std::get<1>(result_empty_extra));
            hud_assert_true(std::get<2>(result_empty_extra));
            hud_assert_true(std::get<3>(result_empty_extra));
            hud_assert_true(std::get<4>(result_empty_extra));
            hud_assert_true(std::get<5>(result_empty_extra));
            hud_assert_true(std::get<6>(result_empty_extra));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));
            hud_assert_true(std::get<5>(result_no_extra));
            hud_assert_true(std::get<6>(result_no_extra));

            const auto result_extra = test_default_allocator(TEST_VALUES, 10u);
            hud_assert_true(std::get<0>(result_extra));
            hud_assert_true(std::get<1>(result_extra));
            hud_assert_true(std::get<2>(result_extra));
            hud_assert_true(std::get<3>(result_extra));
            hud_assert_true(std::get<4>(result_extra));
            hud_assert_true(std::get<5>(result_extra));
            hud_assert_true(std::get<6>(result_extra));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));
            hud_assert_true(std::get<5>(result_empty_no_extra));
            hud_assert_true(std::get<6>(result_empty_no_extra));

            constexpr auto result_empty_extra = test_default_allocator({}, 10u);
            hud_assert_true(std::get<0>(result_empty_extra));
            hud_assert_true(std::get<1>(result_empty_extra));
            hud_assert_true(std::get<2>(result_empty_extra));
            hud_assert_true(std::get<3>(result_empty_extra));
            hud_assert_true(std::get<4>(result_empty_extra));
            hud_assert_true(std::get<5>(result_empty_extra));
            hud_assert_true(std::get<6>(result_empty_extra));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));
            hud_assert_true(std::get<5>(result_no_extra));
            hud_assert_true(std::get<6>(result_no_extra));

            constexpr auto result_extra = test_default_allocator(TEST_VALUES, 10u);
            hud_assert_true(std::get<0>(result_extra));
            hud_assert_true(std::get<1>(result_extra));
            hud_assert_true(std::get<2>(result_extra));
            hud_assert_true(std::get<3>(result_extra));
            hud_assert_true(std::get<4>(result_extra));
            hud_assert_true(std::get<5>(result_extra));
            hud_assert_true(std::get<6>(result_extra));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize extra)
        {
            MovedType moved(initializer);

            // Move the map
            NewType move(hud::move(moved), extra, AllocatorType {});

            // Ensure we move all elements
            bool all_keys_and_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto &init_elem = (initializer.begin() + index);
                const auto it = move.find(init_elem->first);
                if (it == move.end())
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || extra > 0)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                move.count() == initializer.size(),                               // 0
                move.max_count() >= initializer.size(),                           // 1
                moved.count() == 0,                                               // 2
                moved.max_count() == 0,                                           // 3
                all_keys_and_values_moved,                                        // 4
                move.allocator().allocation_count() == expected_allocation_count, // 5
                move.allocator().free_count() == 0                                // 6
            };
        };

        // Non constant
        {
            const auto result_empty_no_extra = test_with_allocator({}, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));
            hud_assert_true(std::get<5>(result_empty_no_extra));
            hud_assert_true(std::get<6>(result_empty_no_extra));

            const auto result_empty_extra = test_with_allocator({}, 10u);
            hud_assert_true(std::get<0>(result_empty_extra));
            hud_assert_true(std::get<1>(result_empty_extra));
            hud_assert_true(std::get<2>(result_empty_extra));
            hud_assert_true(std::get<3>(result_empty_extra));
            hud_assert_true(std::get<4>(result_empty_extra));
            hud_assert_true(std::get<5>(result_empty_extra));
            hud_assert_true(std::get<6>(result_empty_extra));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));
            hud_assert_true(std::get<5>(result_no_extra));
            hud_assert_true(std::get<6>(result_no_extra));

            const auto result_extra = test_with_allocator(TEST_VALUES, 10u);
            hud_assert_true(std::get<0>(result_extra));
            hud_assert_true(std::get<1>(result_extra));
            hud_assert_true(std::get<2>(result_extra));
            hud_assert_true(std::get<3>(result_extra));
            hud_assert_true(std::get<4>(result_extra));
            hud_assert_true(std::get<5>(result_extra));
            hud_assert_true(std::get<6>(result_extra));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));
            hud_assert_true(std::get<5>(result_empty_no_extra));
            hud_assert_true(std::get<6>(result_empty_no_extra));

            constexpr auto result_empty_extra = test_with_allocator({}, 10u);
            hud_assert_true(std::get<0>(result_empty_extra));
            hud_assert_true(std::get<1>(result_empty_extra));
            hud_assert_true(std::get<2>(result_empty_extra));
            hud_assert_true(std::get<3>(result_empty_extra));
            hud_assert_true(std::get<4>(result_empty_extra));
            hud_assert_true(std::get<5>(result_empty_extra));
            hud_assert_true(std::get<6>(result_empty_extra));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));
            hud_assert_true(std::get<5>(result_no_extra));
            hud_assert_true(std::get<6>(result_no_extra));

            constexpr auto result_extra = test_with_allocator(TEST_VALUES, 10u);
            hud_assert_true(std::get<0>(result_extra));
            hud_assert_true(std::get<1>(result_extra));
            hud_assert_true(std::get<2>(result_extra));
            hud_assert_true(std::get<3>(result_extra));
            hud_assert_true(std::get<4>(result_extra));
            hud_assert_true(std::get<5>(result_extra));
            hud_assert_true(std::get<6>(result_extra));
        }
    }
}
