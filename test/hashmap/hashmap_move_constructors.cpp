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

    static_assert(hud::is_bitwise_move_constructible_v<key_type>);
    static_assert(hud::is_bitwise_move_constructible_v<value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;

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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_bitwise_copy_constructible_same_type_different_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = i32;
    using value_type = i64;

    static_assert(hud::is_bitwise_move_constructible_v<key_type>);
    static_assert(hud::is_bitwise_move_constructible_v<value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType2>;

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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_bitwise_copy_constructible_different_type_same_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = i32;
    using value_type = i64;
    using other_key_type = u32;
    using other_value_type = u64;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(hud::is_not_same_v<value_type, other_value_type>);
    static_assert(hud::is_bitwise_move_constructible_v<key_type, other_key_type>);
    static_assert(hud::is_bitwise_move_constructible_v<value_type, other_value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashmap<other_key_type, other_value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;

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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_bitwise_copy_constructible_different_type_different_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = i32;
    using value_type = i64;
    using other_key_type = u32;
    using other_value_type = u64;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(hud::is_not_same_v<value_type, other_value_type>);
    static_assert(hud::is_bitwise_move_constructible_v<key_type, other_key_type>);
    static_assert(hud::is_bitwise_move_constructible_v<value_type, other_value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashmap<other_key_type, other_value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType2>;

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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_non_bitwise_move_constructible_same_type_same_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_move_constructible_type;
    using value_type = hud_test::non_bitwise_move_constructible_type;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    static_assert(!hud::is_bitwise_move_constructible_v<MovedType::storage_type>);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;

            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_non_bitwise_move_constructible_same_type_different_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_move_constructible_type;
    using value_type = hud_test::non_bitwise_move_constructible_type;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType2>;
    static_assert(!hud::is_bitwise_move_constructible_v<MovedType::storage_type>);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;

            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_non_bitwise_move_constructible_different_type_same_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_move_constructible_type;
    using value_type = hud_test::non_bitwise_move_constructible_type;
    using other_key_type = hud_test::non_bitwise_move_constructible_type2;
    using other_value_type = hud_test::non_bitwise_move_constructible_type2;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(hud::is_not_same_v<value_type, other_value_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type, other_value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    static_assert(!hud::is_bitwise_move_constructible_v<MovedType::storage_type>);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;

            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_non_bitwise_move_constructible_different_type_different_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_move_constructible_type;
    using value_type = hud_test::non_bitwise_move_constructible_type;
    using other_key_type = hud_test::non_bitwise_move_constructible_type2;
    using other_value_type = hud_test::non_bitwise_move_constructible_type2;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(hud::is_not_same_v<value_type, other_value_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type, other_value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType2>;
    static_assert(!hud::is_bitwise_move_constructible_v<MovedType::storage_type>);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;

            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_non_bitwise_copy_constructible_same_type_same_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    static_assert(!hud::is_bitwise_move_constructible_v<MovedType::storage_type>);

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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;

            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_non_bitwise_copy_constructible_same_type_different_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType2>;
    static_assert(!hud::is_bitwise_move_constructible_v<MovedType::storage_type>);

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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;

            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_non_bitwise_copy_constructible_different_type_same_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;
    using other_key_type = hud_test::non_bitwise_copy_constructible_type2;
    using other_value_type = hud_test::non_bitwise_copy_constructible_type2;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(hud::is_not_same_v<value_type, other_value_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type, other_value_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type, other_value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    static_assert(!hud::is_bitwise_move_constructible_v<MovedType::storage_type>);

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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;

            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_non_bitwise_copy_constructible_different_type_different_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;
    using other_key_type = hud_test::non_bitwise_copy_constructible_type2;
    using other_value_type = hud_test::non_bitwise_copy_constructible_type2;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(hud::is_not_same_v<value_type, other_value_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type, other_value_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type, other_value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType2>;
    static_assert(!hud::is_bitwise_move_constructible_v<MovedType::storage_type>);

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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
                if (it->key() != init_elem->first && it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;

            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_non_bitwise_same_type_same_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    static_assert(!hud::is_bitwise_move_constructible_v<MovedType::storage_type>);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;

            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_non_bitwise_same_type_different_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType2>;
    static_assert(!hud::is_bitwise_move_constructible_v<MovedType::storage_type>);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;

            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_non_bitwise_different_type_same_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;
    using other_key_type = hud_test::non_bitwise_type2;
    using other_value_type = hud_test::non_bitwise_type2;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(hud::is_not_same_v<value_type, other_value_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type, other_value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    static_assert(!hud::is_bitwise_move_constructible_v<MovedType::storage_type>);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;

            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}

GTEST_TEST(hashmap, move_construct_non_bitwise_different_type_different_allocator)
{
    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;
    using other_key_type = hud_test::non_bitwise_type2;
    using other_value_type = hud_test::non_bitwise_type2;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(hud::is_not_same_v<value_type, other_value_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type, other_value_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    using MovedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType2>;
    static_assert(!hud::is_bitwise_move_constructible_v<MovedType::storage_type>);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
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
        auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra);

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to move
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the move, if we have element to move or if we have extra
            if (copy_allocate)
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
            const auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_default_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_default_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_default_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_default_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_default_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_default_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_default_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_default_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        auto test_with_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer, usize copied_extra, usize copy_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Move the map
            NewType move(hud::move(moved), copy_extra, AllocatorType {});

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
                if (it->key() != init_elem->first && it->key().move_constructor_count() != 1 && it->key().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
                if (it->value() != init_elem->second && it->value().move_constructor_count() != 1 && it->value().copy_constructor_count() != 1)
                {
                    all_keys_and_values_moved = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;

            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0 || copy_extra > 0)
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
            const auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            const auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            const auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            const auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            const auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            const auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra_1 = test_with_allocator({}, 10u, 0u);
            hud_assert_true(std::get<0>(result_empty_extra_1));
            hud_assert_true(std::get<1>(result_empty_extra_1));
            hud_assert_true(std::get<2>(result_empty_extra_1));
            hud_assert_true(std::get<3>(result_empty_extra_1));
            hud_assert_true(std::get<4>(result_empty_extra_1));

            constexpr auto result_empty_extra_2 = test_with_allocator({}, 0u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_2));
            hud_assert_true(std::get<1>(result_empty_extra_2));
            hud_assert_true(std::get<2>(result_empty_extra_2));
            hud_assert_true(std::get<3>(result_empty_extra_2));
            hud_assert_true(std::get<4>(result_empty_extra_2));

            constexpr auto result_empty_extra_3 = test_with_allocator({}, 10u, 10u);
            hud_assert_true(std::get<0>(result_empty_extra_3));
            hud_assert_true(std::get<1>(result_empty_extra_3));
            hud_assert_true(std::get<2>(result_empty_extra_3));
            hud_assert_true(std::get<3>(result_empty_extra_3));
            hud_assert_true(std::get<4>(result_empty_extra_3));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra1 = test_with_allocator(TEST_VALUES, 10u, 0u);
            hud_assert_true(std::get<0>(result_extra1));
            hud_assert_true(std::get<1>(result_extra1));
            hud_assert_true(std::get<2>(result_extra1));
            hud_assert_true(std::get<3>(result_extra1));
            hud_assert_true(std::get<4>(result_extra1));

            constexpr auto result_extra2 = test_with_allocator(TEST_VALUES, 0u, 10u);
            hud_assert_true(std::get<0>(result_extra2));
            hud_assert_true(std::get<1>(result_extra2));
            hud_assert_true(std::get<2>(result_extra2));
            hud_assert_true(std::get<3>(result_extra2));
            hud_assert_true(std::get<4>(result_extra2));

            constexpr auto result_extra3 = test_with_allocator(TEST_VALUES, 10u, 10u);
            hud_assert_true(std::get<0>(result_extra3));
            hud_assert_true(std::get<1>(result_extra3));
            hud_assert_true(std::get<2>(result_extra3));
            hud_assert_true(std::get<3>(result_extra3));
            hud_assert_true(std::get<4>(result_extra3));
        }
    }
}
