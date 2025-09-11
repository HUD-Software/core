#include <core/containers/hashset.h>
#include "../misc/allocator_watcher.h"

#define TEST_VALUES { \
    0,                \
    1,                \
    2,                \
    3,                \
    4,                \
    5,                \
    6,                \
    7                 \
}

GTEST_TEST(hashset, copy_construct_bitwise_copy_constructible_same_type_same_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using key_type = i32;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    using CopiedType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    static_assert(hud::is_bitwise_copy_constructible_v<CopiedType::storage_type>);

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 4 : 2) : 0), // 3
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 3
                copy.allocator().free_count() == 0                                                                            // 4
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

            const auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            constexpr auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to copy
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the copy, if we have element to copy or if we have extra
            if (copy_allocate)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
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
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

GTEST_TEST(hashset, copy_construct_bitwise_copy_constructible_same_type_different_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using key_type = i32;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    using CopiedType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType2>;
    static_assert(hud::is_bitwise_copy_constructible_v<CopiedType::storage_type>);

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 4 : 2) : 0), // 3
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 3
                copy.allocator().free_count() == 0                                                                            // 4
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

            const auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            constexpr auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to copy
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the copy, if we have element to copy or if we have extra
            if (copy_allocate)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
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
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

GTEST_TEST(hashset, copy_construct_bitwise_copy_constructible_different_type_same_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using key_type = i32;
    using other_key_type = u32;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashset<other_key_type, hud::hash_64<other_key_type>, hud::equal<other_key_type>, AllocatorType>;
    using CopiedType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    static_assert(hud::is_bitwise_copy_constructible_v<CopiedType::storage_type>);

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 4 : 2) : 0), // 3
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 3
                copy.allocator().free_count() == 0                                                                            // 4
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

            const auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            constexpr auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to copy
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the copy, if we have element to copy or if we have extra
            if (copy_allocate)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
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
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

GTEST_TEST(hashset, copy_construct_bitwise_copy_constructible_different_type_different_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using key_type = i32;
    using other_key_type = u32;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashset<other_key_type, hud::hash_64<other_key_type>, hud::equal<other_key_type>, AllocatorType>;
    using CopiedType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType2>;
    static_assert(hud::is_bitwise_copy_constructible_v<CopiedType::storage_type>);

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 4 : 2) : 0), // 3
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 3
                copy.allocator().free_count() == 0                                                                            // 4
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

            const auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            constexpr auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to copy
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the copy, if we have element to copy or if we have extra
            if (copy_allocate)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem)
                {
                    all_keys_copied = false;
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
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

GTEST_TEST(hashset, copy_construct_non_bitwise_copy_constructible_same_type_same_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_copy_constructible_type;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    using CopiedType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    static_assert(!hud::is_bitwise_copy_constructible_v<CopiedType::storage_type>);

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 4 : 2) : 0), // 3
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 3
                copy.allocator().free_count() == 0                                                                            // 4
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

            const auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            constexpr auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to copy
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the copy, if we have element to copy or if we have extra
            if (copy_allocate)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
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
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

GTEST_TEST(hashset, copy_construct_non_bitwise_copy_constructible_same_type_different_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_copy_constructible_type;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    using CopiedType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType2>;
    static_assert(!hud::is_bitwise_copy_constructible_v<CopiedType::storage_type>);

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 4 : 2) : 0), // 3
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 3
                copy.allocator().free_count() == 0                                                                            // 4
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

            const auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            constexpr auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to copy
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the copy, if we have element to copy or if we have extra
            if (copy_allocate)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
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
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

GTEST_TEST(hashset, copy_construct_non_bitwise_copy_constructible_different_type_same_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using other_key_type = hud_test::non_bitwise_copy_constructible_type2;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashset<other_key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    using CopiedType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    static_assert(!hud::is_bitwise_copy_constructible_v<CopiedType::storage_type>);

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 4 : 2) : 0), // 3
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 3
                copy.allocator().free_count() == 0                                                                            // 4
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

            const auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            constexpr auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to copy
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the copy, if we have element to copy or if we have extra
            if (copy_allocate)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
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
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

GTEST_TEST(hashset, copy_construct_non_bitwise_copy_constructible_different_type_different_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using other_key_type = hud_test::non_bitwise_copy_constructible_type2;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashset<other_key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    using CopiedType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType2>;
    static_assert(!hud::is_bitwise_copy_constructible_v<CopiedType::storage_type>);

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 4 : 2) : 0), // 3
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 3
                copy.allocator().free_count() == 0                                                                            // 4
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

            const auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            constexpr auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to copy
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the copy, if we have element to copy or if we have extra
            if (copy_allocate)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
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
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

GTEST_TEST(hashset, copy_construct_non_bitwise_same_type_same_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_type;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    using CopiedType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    static_assert(!hud::is_bitwise_copy_constructible_v<CopiedType::storage_type>);

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 4 : 2) : 0), // 3
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 3
                copy.allocator().free_count() == 0                                                                            // 4
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

            const auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            constexpr auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to copy
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the copy, if we have element to copy or if we have extra
            if (copy_allocate)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval
            {
                expected_allocation_count *= 2;
            }
            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
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
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

GTEST_TEST(hashset, copy_construct_non_bitwise_same_type_different_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_type;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    using CopiedType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType2>;
    static_assert(!hud::is_bitwise_copy_constructible_v<CopiedType::storage_type>);

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 4 : 2) : 0), // 3
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 3
                copy.allocator().free_count() == 0                                                                            // 4
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

            const auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            constexpr auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to copy
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the copy, if we have element to copy or if we have extra
            if (copy_allocate)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(init_elem);
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
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
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

GTEST_TEST(hashset, copy_construct_non_bitwise_different_type_same_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_type;
    using other_key_type = hud_test::non_bitwise_type2;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using NewType = hud::hashset<other_key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    using CopiedType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    static_assert(!hud::is_bitwise_copy_constructible_v<CopiedType::storage_type>);

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(key_type {init_elem});
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 4 : 2) : 0), // 3
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(key_type {init_elem});
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 3
                copy.allocator().free_count() == 0                                                                            // 4
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

            const auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            constexpr auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, copied_extra);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(key_type {init_elem});
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            // Allocation of the object to copy
            if (initializer.size() > 0)
            {
                expected_allocation_count++;
            }
            // Allocation of the copy, if we have element to copy or if we have extra
            if (initializer.size() > 0 || copied_extra > 0)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copied_extra,            // 1
                all_keys_copied,                                                  // 2
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

            constexpr auto result_empty_extra = test_default_allocator({}, 10u);
            hud_assert_true(std::get<0>(result_empty_extra));
            hud_assert_true(std::get<1>(result_empty_extra));
            hud_assert_true(std::get<2>(result_empty_extra));
            hud_assert_true(std::get<3>(result_empty_extra));
            hud_assert_true(std::get<4>(result_empty_extra));

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

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, copied_extra, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(key_type {init_elem});
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
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
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copied_extra,            // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

            const auto result_empty_extra = test_with_allocator({}, 10u);
            hud_assert_true(std::get<0>(result_empty_extra));
            hud_assert_true(std::get<1>(result_empty_extra));
            hud_assert_true(std::get<2>(result_empty_extra));
            hud_assert_true(std::get<3>(result_empty_extra));
            hud_assert_true(std::get<4>(result_empty_extra));

            const auto result_no_extra = test_with_allocator(TEST_VALUES, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            const auto result_extra = test_with_allocator(TEST_VALUES, 10u);
            hud_assert_true(std::get<0>(result_extra));
            hud_assert_true(std::get<1>(result_extra));
            hud_assert_true(std::get<2>(result_extra));
            hud_assert_true(std::get<3>(result_extra));
            hud_assert_true(std::get<4>(result_extra));
        }

        // Constant
        {
            constexpr auto result_empty_no_extra = test_with_allocator({}, 0u);
            hud_assert_true(std::get<0>(result_empty_no_extra));
            hud_assert_true(std::get<1>(result_empty_no_extra));
            hud_assert_true(std::get<2>(result_empty_no_extra));
            hud_assert_true(std::get<3>(result_empty_no_extra));
            hud_assert_true(std::get<4>(result_empty_no_extra));

            constexpr auto result_empty_extra = test_with_allocator({}, 10u);
            hud_assert_true(std::get<0>(result_empty_extra));
            hud_assert_true(std::get<1>(result_empty_extra));
            hud_assert_true(std::get<2>(result_empty_extra));
            hud_assert_true(std::get<3>(result_empty_extra));
            hud_assert_true(std::get<4>(result_empty_extra));

            constexpr auto result_no_extra = test_with_allocator(TEST_VALUES, 0u);
            hud_assert_true(std::get<0>(result_no_extra));
            hud_assert_true(std::get<1>(result_no_extra));
            hud_assert_true(std::get<2>(result_no_extra));
            hud_assert_true(std::get<3>(result_no_extra));
            hud_assert_true(std::get<4>(result_no_extra));

            constexpr auto result_extra = test_with_allocator(TEST_VALUES, 10u);
            hud_assert_true(std::get<0>(result_extra));
            hud_assert_true(std::get<1>(result_extra));
            hud_assert_true(std::get<2>(result_extra));
            hud_assert_true(std::get<3>(result_extra));
            hud_assert_true(std::get<4>(result_extra));
        }
    }
}

GTEST_TEST(hashset, copy_construct_non_bitwise_different_type_different_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using key_type = hud_test::non_bitwise_type;
    using other_key_type = hud_test::non_bitwise_type2;

    static_assert(hud::is_not_same_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);

    using AllocatorType = hud_test::allocator_watcher<1>;
    using AllocatorType2 = hud_test::allocator_watcher_2<1>;
    using NewType = hud::hashset<other_key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
    using CopiedType = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType2>;
    static_assert(!hud::is_bitwise_copy_constructible_v<CopiedType::storage_type>);

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(key_type {init_elem});
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 4 : 2) : 0), // 3
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer)
        {
            const CopiedType copied(initializer);

            // Copy the set
            NewType copy(copied, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(key_type {init_elem});
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }

            return std::tuple {
                copy.count() == copied.count(),                                                                               // 0
                copy.max_count() == copied.max_count(),                                                                       // 1
                all_keys_copied,                                                                                              // 2
                copy.allocator().allocation_count() == (initializer.size() > 0 ? (hud::is_constant_evaluated() ? 2 : 1) : 0), // 3
                copy.allocator().free_count() == 0                                                                            // 4
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

            const auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {

            constexpr auto result_empty = test_with_allocator({});
            hud_assert_true(std::get<0>(result_empty));
            hud_assert_true(std::get<1>(result_empty));
            hud_assert_true(std::get<2>(result_empty));
            hud_assert_true(std::get<3>(result_empty));
            hud_assert_true(std::get<4>(result_empty));

            constexpr auto result = test_with_allocator(TEST_VALUES);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
        }
    }

    // With extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra);

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(key_type {init_elem});
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool copied_allocate = initializer.size() > 0 || copied_extra > 0;
            bool copy_allocate = copied_allocate || copy_extra > 0;

            // Allocation of the object to copy
            if (copied_allocate)
            {
                expected_allocation_count++;
            }
            // Allocation of the copy, if we have element to copy or if we have extra
            if (copy_allocate)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval
            {
                expected_allocation_count *= 2;
            }
            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra, usize copy_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the set
            NewType copy(copied, copy_extra, AllocatorType {});

            // Ensure we copy all elements
            bool all_keys_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                const auto init_elem = initializer.begin()[index];
                const auto it = copy.find(key_type {init_elem});
                if (it == copy.end())
                {
                    all_keys_copied = false;
                    break;
                }
                if (it->key() != init_elem && it->key().copy_constructor_count() != 2)
                {
                    all_keys_copied = false;
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
            if consteval
            {
                expected_allocation_count *= 2;
            }

            return std::tuple {
                copy.count() == copied.count(),                                   // 0
                copy.max_count() >= copied.max_count() + copy_extra,              // 1
                all_keys_copied,                                                  // 2
                copy.allocator().allocation_count() == expected_allocation_count, // 3
                copy.allocator().free_count() == 0                                // 4
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
