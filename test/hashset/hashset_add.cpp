#include <core/containers/hashset.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashset, add_by_copy_construct_non_bitwise_copy_constructible_same_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type set;
            const key_type key_to_copy(1);
            const auto it_1st = set.add(key_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            const key_type key_to_copy_1(1);
            const auto it_same = set.add(key_to_copy_1);

            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            const key_type key_to_copy_2(2);
            const auto it_2nd = set.add(key_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 4
                set.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 5
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            constexpr usize reserved_size = 2;
            hashmap_type set;
            set.reserve(reserved_size);
            const key_type key_to_copy(1);
            const auto it_1st = set.add(key_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            const key_type key_to_copy_1(1);
            const auto it_same = set.add(key_to_copy_1);

            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            const key_type key_to_copy_2(2);
            const auto it_2nd = set.add(key_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }
    }
}

GTEST_TEST(hashset, add_by_copy_construct_non_bitwise_copy_constructible_different_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type2;
    using other_key_type = hud_test::non_bitwise_copy_constructible_type;

    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type set;
            const other_key_type key_to_copy(1);
            const auto it_1st = set.add(key_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            const other_key_type key_to_copy_1(1);
            const auto it_same = set.add(key_to_copy_1);

            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            const other_key_type key_to_copy_2(2);
            const auto it_2nd = set.add(key_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 4
                set.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 5
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            constexpr usize reserved_size = 2;
            hashmap_type set;
            set.reserve(2);
            const other_key_type key_to_copy(1);

            const auto it_1st = set.add(key_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            const other_key_type key_to_copy_1(1);
            const auto it_same = set.add(key_to_copy_1);

            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            const other_key_type key_to_copy_2(2);
            const auto it_2nd = set.add(key_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }
    }
}

GTEST_TEST(hashset, add_by_copy_construct_bitwise_copy_constructible_same_type)
{
    using key_type = usize;
    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, key_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type set;
            const key_type key_to_copy(1);
            const auto it_1st = set.add(key_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            const key_type key_to_copy_1(1);
            const auto it_same = set.add(key_to_copy_1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            const key_type key_to_copy_2(2);
            const auto it_2nd = set.add(key_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 3
                set.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
    }
    // Without reallocation
    {
        const auto test = []()
        {
            constexpr usize reserved_size = 2;
            hashmap_type set;
            set.reserve(reserved_size);
            const key_type key_to_copy(1);
            const auto it_1st = set.add(key_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            const key_type key_to_copy_1(1);
            const auto it_same = set.add(key_to_copy_1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            const key_type key_to_copy_2(2);
            const auto it_2nd = set.add(key_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
    }
}

GTEST_TEST(hashset, add_by_copy_construct_bitwise_copy_constructible_different_type)
{
    using key_type = usize;
    using other_key_type = isize;
    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type set;
            const other_key_type key_to_copy(1);
            const auto it_1st = set.add(key_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            const other_key_type key_to_copy_1(1);
            const auto it_same = set.add(key_to_copy_1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            const other_key_type key_to_copy_2(2);
            const auto it_2nd = set.add(key_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 3
                set.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 4
            };
            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
    }
    // Without reallocation
    {
        const auto test = []()
        {
            constexpr usize reserved_size = 2;
            hashmap_type set;
            set.reserve(reserved_size);
            const other_key_type key_to_copy(1);
            const auto it_1st = set.add(key_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            const other_key_type key_to_copy_1(1);
            const auto it_same = set.add(key_to_copy_1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            const other_key_type key_to_copy_2(2);
            const auto it_2nd = set.add(key_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
    }
}

GTEST_TEST(hashset, add_by_move_construct_non_bitwise_copy_constructible_same_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type set;
            key_type key_to_copy(1);
            const auto it_1st = set.add(hud::move(key_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            key_type key_to_copy_1(1);
            const auto it_same = set.add(hud::move(key_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            key_type key_to_copy_2(2);
            const auto it_2nd = set.add(hud::move(key_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 4
                set.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 5
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            constexpr usize reserved_size = 2;
            hashmap_type set;
            set.reserve(reserved_size);
            key_type key_to_copy(1);

            const auto it_1st = set.add(hud::move(key_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            key_type key_to_copy_1(1);
            const auto it_same = set.add(hud::move(key_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            key_type key_to_copy_2(2);
            const auto it_2nd = set.add(hud::move(key_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }
    }
}

GTEST_TEST(hashset, add_by_move_construct_non_bitwise_copy_constructible_different_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type2;
    using other_key_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type set;
            other_key_type key_to_copy(1);
            const auto it_1st = set.add(hud::move(key_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            other_key_type key_to_copy_1(1);
            const auto it_same = set.add(hud::move(key_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            other_key_type key_to_copy_2(2);
            const auto it_2nd = set.add(hud::move(key_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 4
                set.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 5
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            constexpr usize reserved_size = 2;
            hashmap_type set;
            set.reserve(reserved_size);
            other_key_type key_to_copy(1);

            const auto it_1st = set.add(hud::move(key_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            other_key_type key_to_copy_1(1);
            const auto it_same = set.add(hud::move(key_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };
            other_key_type key_to_copy_2(2);
            const auto it_2nd = set.add(hud::move(key_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 4
                set.allocator().free_count() == 0,                                            // 5
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
        }
    }
}

GTEST_TEST(hashset, add_by_move_construct_bitwise_copy_constructible_same_type)
{
    using key_type = usize;
    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type set;
            key_type key_to_copy(1);
            const auto it_1st = set.add(hud::move(key_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            key_type key_to_copy_1(1);
            const auto it_same = set.add(hud::move(key_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            key_type key_to_copy_2(2);
            const auto it_2nd = set.add(hud::move(key_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 3
                set.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
    }
    // Without reallocation
    {
        const auto test = []()
        {
            constexpr usize reserved_size = 3;
            hashmap_type set;
            set.reserve(reserved_size);
            key_type key_to_copy(1);
            const auto it_1st = set.add(hud::move(key_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            key_type key_to_copy_1(1);
            const auto it_same = set.add(hud::move(key_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            key_type key_to_copy_2(2);
            const auto it_2nd = set.add(hud::move(key_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
    }
}

GTEST_TEST(hashset, add_by_move_construct_bitwise_copy_constructible_different_type)
{
    using key_type = usize;
    using other_key_type = isize;
    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type set;
            other_key_type key_to_copy(1);
            const auto it_1st = set.add(hud::move(key_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            other_key_type key_to_copy_1(1);
            const auto it_same = set.add(hud::move(key_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            other_key_type key_to_copy_2(2);
            const auto it_2nd = set.add(hud::move(key_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 3
                set.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
    }
    // Without reallocation
    {
        const auto test = []()
        {
            constexpr usize reserved_size = 3;
            hashmap_type set;
            set.reserve(reserved_size);
            other_key_type key_to_copy(1);
            const auto it_1st = set.add(hud::move(key_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            other_key_type key_to_copy_1(1);
            const auto it_same = set.add(hud::move(key_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            other_key_type key_to_copy_2(2);
            const auto it_2nd = set.add(hud::move(key_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                set.allocator().free_count() == 0,                                            // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
        }
    }
}

GTEST_TEST(hashset, add_by_move_construct_non_bitwise_move_constructible_same_type)
{
    using key_type = hud_test::non_bitwise_move_constructible_type;
    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type set;
            key_type key_to_copy(1);
            const auto it_1st = set.add(hud::move(key_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 0,                                  // 3
                it_1st->key().move_constructor_count() == 1,                                  // 4
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                set.allocator().free_count() == 0,                                            // 6
            };
            key_type key_to_copy_1(1);
            const auto it_same = set.add(hud::move(key_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_same->key().copy_constructor_count() == 0,                                 // 3
                it_same->key().move_constructor_count() == 1,                                 // 4
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                set.allocator().free_count() == 0,                                            // 6
            };
            key_type key_to_copy_2(2);
            const auto it_2nd = set.add(hud::move(key_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 0,                                  // 3
                it_2nd->key().move_constructor_count() == 1,                                  // 4
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 5
                set.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            constexpr usize reserved_size = 2;
            hashmap_type set;
            set.reserve(reserved_size);
            key_type key_to_copy(1);

            const auto it_1st = set.add(hud::move(key_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 0,                                  // 3
                it_1st->key().move_constructor_count() == 1,                                  // 4
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                set.allocator().free_count() == 0,                                            // 6
            };
            key_type key_to_copy_1(1);
            const auto it_same = set.add(hud::move(key_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 0,                                 // 3
                it_same->key().move_constructor_count() == 1,                                 // 4
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                set.allocator().free_count() == 0,                                            // 6
            };
            key_type key_to_copy_2(2);
            const auto it_2nd = set.add(hud::move(key_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 0,                                  // 3
                it_2nd->key().move_constructor_count() == 1,                                  // 4
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                set.allocator().free_count() == 0,                                            // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
        }
    }
}

GTEST_TEST(hashset, add_by_move_construct_non_bitwise_move_constructible_different_type)
{
    using key_type = hud_test::non_bitwise_move_constructible_type2;
    using other_key_type = hud_test::non_bitwise_move_constructible_type;

    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type, other_key_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type set;
            other_key_type key_to_copy(1);
            const auto it_1st = set.add(hud::move(key_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 0,                                  // 3
                it_1st->key().move_constructor_count() == 1,                                  // 4
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                set.allocator().free_count() == 0,                                            // 6
            };
            other_key_type key_to_copy_1(1);
            const auto it_same = set.add(hud::move(key_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_same->key().copy_constructor_count() == 0,                                 // 3
                it_same->key().move_constructor_count() == 1,                                 // 4
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                set.allocator().free_count() == 0,                                            // 6
            };
            other_key_type key_to_copy_2(2);
            const auto it_2nd = set.add(hud::move(key_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= set.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 0,                                  // 3
                it_2nd->key().move_constructor_count() == 1,                                  // 4
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 5
                set.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            constexpr usize reserved_size = 2;
            hashmap_type set;
            set.reserve(reserved_size);
            other_key_type key_to_copy(1);

            const auto it_1st = set.add(hud::move(key_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1,                                                           // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 0,                                  // 3
                it_1st->key().move_constructor_count() == 1,                                  // 4
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                set.allocator().free_count() == 0,                                            // 6
            };
            other_key_type key_to_copy_1(1);
            const auto it_same = set.add(hud::move(key_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1,                                                          // 0
                set.count() == 1,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 0,                                 // 3
                it_same->key().move_constructor_count() == 1,                                 // 4
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                set.allocator().free_count() == 0,                                            // 6
            };
            other_key_type key_to_copy_2(2);
            const auto it_2nd = set.add(hud::move(key_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2,                                                           // 0
                set.count() == 2,                                                             // 1
                set.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 0,                                  // 3
                it_2nd->key().move_constructor_count() == 1,                                  // 4
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                set.allocator().free_count() == 0,                                            // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
        }
    }
}

GTEST_TEST(hashset, add_by_piecewise_construct_bitwise_same_type)
{
    using key_type = i32;
    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    const auto test = []() -> decltype(auto)
    {
        constexpr usize reserved_size = 2;
        hashmap_type set;
        set.reserve(reserved_size);
        const auto it = set.add(hud::tag_piecewise_construct, hud::forward_as_tuple(1));
        return std::tuple {
            set.count() == 1, // 0
            it->key() == 1,   // 1
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }
}

GTEST_TEST(hashset, add_by_piecewise_construct_bitwise_different_type)
{
    using key_type = i32;
    using other_key_type = u32;
    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    const auto test = []()
    {
        constexpr usize reserved_size = 2;
        hashmap_type set;
        set.reserve(reserved_size);
        const auto it = set.add(hud::tag_piecewise_construct, hud::forward_as_tuple(other_key_type {1}));

        return std::tuple {
            set.count() == 1, // 0
            it->key() == 1,   // 1
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }
}

GTEST_TEST(hashset, add_by_piecewise_construct_non_bitwise_same_type)
{
    using key_type = hud_test::non_bitwise_type;
    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    const auto test = []()
    {
        constexpr usize reserved_size = 2;
        i32 ptr[2];
        hashmap_type set;
        set.reserve(reserved_size);

        static_assert(hud::is_hashable_64_v<key_type, decltype(hud::forward_as_tuple(1, ptr))>);
        static_assert(hud::is_comparable_with_equal_v<key_type, decltype(hud::forward_as_tuple(1, ptr))>);

        const auto it = set.add(hud::tag_piecewise_construct, hud::forward_as_tuple(1, ptr));

        return std::tuple {
            set.count() == 1,                        // 0
            it->key().constructor_count() == 1,      // 1
            it->key().move_constructor_count() == 0, // 2
            it->key().copy_constructor_count() == 0, // 3
            it->key().move_assign_count() == 0,      // 4
            it->key().copy_assign_count() == 0,      // 5
            it->key().id() == 1,                     // 6
            it->key().destructor_counter() == ptr,   // 7
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
    }
}

GTEST_TEST(hashset, add_by_piecewise_construct_non_bitwise_different_type)
{
    using key_type = hud_test::non_bitwise_type;
    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    const auto test = []()
    {
        constexpr usize reserved_size = 2;
        i32 ptr[2];
        hashmap_type set;
        set.reserve(reserved_size);

        static_assert(hud::is_hashable_64_v<key_type, decltype(hud::forward_as_tuple(i64 {1}, ptr))>);
        static_assert(hud::is_comparable_with_equal_v<key_type, decltype(hud::forward_as_tuple(i64 {1}, ptr))>);

        const auto it = set.add(hud::tag_piecewise_construct, hud::forward_as_tuple(i64 {1}, ptr));

        return std::tuple {
            set.count() == 1,                        // 0
            it->key().constructor_count() == 1,      // 1
            it->key().move_constructor_count() == 0, // 2
            it->key().copy_constructor_count() == 0, // 3
            it->key().move_assign_count() == 0,      // 4
            it->key().copy_assign_count() == 0,      // 5
            it->key().id() == 1,                     // 6
            it->key().destructor_counter() == ptr,   // 7
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
    }
}

GTEST_TEST(hashset, add_by_piecewise_construct_tuples)
{
    using key_type = hud::tuple<i32, u64, hud_test::non_bitwise_type>;
    using hashmap_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    const auto test = []()
    {
        constexpr usize reserved_size = 2;
        i32 ptr[2];
        hashmap_type set;
        set.reserve(reserved_size);

        static_assert(hud::is_hashable_64_v<key_type, decltype(hud::forward_as_tuple(1, 2, hud_test::non_bitwise_type {3, ptr}))>);
        static_assert(hud::is_comparable_with_equal_v<key_type, decltype(hud::forward_as_tuple(1, 2, hud_test::non_bitwise_type {3, ptr}))>);

        const auto it = set.add(hud::tag_piecewise_construct, hud::forward_as_tuple(1, 2, hud_test::non_bitwise_type {3, ptr}));

        const key_type expected_key {
            1,
            2,
            hud_test::non_bitwise_type {3, ptr}
        };
        return std::tuple {
            set.count() == 1,          // 0
            it->key() == expected_key, // 1
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }
}
