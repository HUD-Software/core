#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashmap, add_by_copy_construct_non_bitwise_copy_constructible_different_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type2;
    using other_key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type2;
    using other_value_type = hud_test::non_bitwise_copy_constructible_type;

    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type, other_value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            const other_key_type key_to_copy(1);
            const other_value_type value_to_copy(11);
            const auto it_1st = map.add(key_to_copy, value_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            const other_key_type key_to_copy_1(1);
            const other_value_type value_to_copy_1(00);
            const auto it_same = map.add(key_to_copy_1, value_to_copy_1);

            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            const other_key_type key_to_copy_2(2);
            const other_value_type value_to_copy_2(22);
            const auto it_2nd = map.add(key_to_copy_2, value_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 5
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 2;
            hashmap_type map;
            map.reserve(2);
            const other_key_type key_to_copy(1);
            const other_value_type value_to_copy(11);

            const auto it_1st = map.add(key_to_copy, value_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            const other_key_type key_to_copy_1(1);
            const other_value_type value_to_copy_1(00);
            const auto it_same = map.add(key_to_copy_1, value_to_copy_1);

            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            const other_key_type key_to_copy_2(2);
            const other_value_type value_to_copy_2(22);
            const auto it_2nd = map.add(key_to_copy_2, value_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_by_copy_construct_bitwise_copy_constructible_same_type)
{
    using key_type = usize;
    using value_type = usize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, key_type>);
    static_assert(hud::is_bitwise_copy_constructible_v<value_type, value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            const key_type key_to_copy(1);
            const value_type value_to_copy(11);
            const auto it_1st = map.add(key_to_copy, value_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const key_type key_to_copy_1(1);
            const value_type value_to_copy_1(00);
            const auto it_same = map.add(key_to_copy_1, value_to_copy_1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const key_type key_to_copy_2(2);
            const value_type value_to_copy_2(22);
            const auto it_2nd = map.add(key_to_copy_2, value_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 3
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 2;
            hashmap_type map;
            map.reserve(reserved_size);
            const key_type key_to_copy(1);
            const value_type value_to_copy(11);
            const auto it_1st = map.add(key_to_copy, value_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const key_type key_to_copy_1(1);
            const value_type value_to_copy_1(00);
            const auto it_same = map.add(key_to_copy_1, value_to_copy_1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const key_type key_to_copy_2(2);
            const value_type value_to_copy_2(22);
            const auto it_2nd = map.add(key_to_copy_2, value_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_by_copy_construct_bitwise_copy_constructible_different_type)
{
    using key_type = usize;
    using other_key_type = isize;
    using value_type = usize;
    using other_value_type = isize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);
    static_assert(hud::is_bitwise_copy_constructible_v<value_type, other_value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            const other_key_type key_to_copy(1);
            const other_value_type value_to_copy(11);
            const auto it_1st = map.add(key_to_copy, value_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const other_key_type key_to_copy_1(1);
            const other_value_type value_to_copy_1(00);
            const auto it_same = map.add(key_to_copy_1, value_to_copy_1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const other_key_type key_to_copy_2(2);
            const other_value_type value_to_copy_2(22);
            const auto it_2nd = map.add(key_to_copy_2, value_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 3
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 4
            };
            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 2;
            hashmap_type map;
            map.reserve(reserved_size);
            const other_key_type key_to_copy(1);
            const other_value_type value_to_copy(11);
            const auto it_1st = map.add(key_to_copy, value_to_copy);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const other_key_type key_to_copy_1(1);
            const other_value_type value_to_copy_1(00);
            const auto it_same = map.add(key_to_copy_1, value_to_copy_1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const other_key_type key_to_copy_2(2);
            const other_value_type value_to_copy_2(22);
            const auto it_2nd = map.add(key_to_copy_2, value_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_by_move_construct_non_bitwise_copy_constructible_same_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            key_type key_to_copy(1);
            value_type value_to_copy(11);
            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            key_type key_to_copy_1(1);
            value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            key_type key_to_copy_2(2);
            value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 5
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 2;
            hashmap_type map;
            map.reserve(reserved_size);
            key_type key_to_copy(1);
            value_type value_to_copy(11);

            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            key_type key_to_copy_1(1);
            value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            key_type key_to_copy_2(2);
            value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_by_move_construct_non_bitwise_copy_constructible_different_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type2;
    using other_key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type2;
    using other_value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type, other_value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            other_key_type key_to_copy(1);
            other_value_type value_to_copy(11);
            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            other_key_type key_to_copy_1(1);
            other_value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            other_key_type key_to_copy_2(2);
            other_value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 5
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 2;
            hashmap_type map;
            map.reserve(reserved_size);
            other_key_type key_to_copy(1);
            other_value_type value_to_copy(11);

            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            other_key_type key_to_copy_1(1);
            other_value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            other_key_type key_to_copy_2(2);
            other_value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_by_move_construct_bitwise_copy_constructible_same_type)
{
    using key_type = usize;
    using value_type = usize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(hud::is_bitwise_copy_constructible_v<value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            key_type key_to_copy(1);
            value_type value_to_copy(11);
            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            key_type key_to_copy_1(1);
            value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            key_type key_to_copy_2(2);
            value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 3
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 3;
            hashmap_type map;
            map.reserve(reserved_size);
            key_type key_to_copy(1);
            value_type value_to_copy(11);
            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            key_type key_to_copy_1(1);
            value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            key_type key_to_copy_2(2);
            value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_by_move_construct_bitwise_copy_constructible_different_type)
{
    using key_type = usize;
    using other_key_type = isize;
    using value_type = usize;
    using other_value_type = isize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);
    static_assert(hud::is_bitwise_copy_constructible_v<value_type, other_value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            other_key_type key_to_copy(1);
            other_value_type value_to_copy(11);
            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            other_key_type key_to_copy_1(1);
            other_value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            other_key_type key_to_copy_2(2);
            other_value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 3
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 3;
            hashmap_type map;
            map.reserve(reserved_size);
            other_key_type key_to_copy(1);
            other_value_type value_to_copy(11);
            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            other_key_type key_to_copy_1(1);
            other_value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            other_key_type key_to_copy_2(2);
            other_value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_by_move_construct_non_bitwise_move_constructible_same_type)
{
    using key_type = hud_test::non_bitwise_move_constructible_type;
    using value_type = hud_test::non_bitwise_move_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            key_type key_to_copy(1);
            value_type value_to_copy(11);
            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 0,                                  // 3
                it_1st->key().move_constructor_count() == 1,                                  // 4
                it_1st->value().copy_constructor_count() == 0,                                // 5
                it_1st->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            key_type key_to_copy_1(1);
            value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_same->key().copy_constructor_count() == 0,                                 // 3
                it_same->key().move_constructor_count() == 1,                                 // 4
                it_same->value().copy_constructor_count() == 0,                               // 5
                it_same->value().move_constructor_count() == 1,                               // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            key_type key_to_copy_2(2);
            value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 0,                                  // 3
                it_2nd->key().move_constructor_count() == 1,                                  // 4
                it_2nd->value().copy_constructor_count() == 0,                                // 5
                it_2nd->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 7
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 8
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
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
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
        }
    }

    // Without reallocation
    {
        static const auto test = []() {
            constexpr usize reserved_size = 2;
            hashmap_type map;
            map.reserve(reserved_size);
            key_type key_to_copy(1);
            value_type value_to_copy(11);

            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 0,                                  // 3
                it_1st->key().move_constructor_count() == 1,                                  // 4
                it_1st->value().copy_constructor_count() == 0,                                // 5
                it_1st->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            key_type key_to_copy_1(1);
            value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 0,                                 // 3
                it_same->key().move_constructor_count() == 1,                                 // 4
                it_same->value().copy_constructor_count() == 0,                               // 5
                it_same->value().move_constructor_count() == 1,                               // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            key_type key_to_copy_2(2);
            value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 0,                                  // 3
                it_2nd->key().move_constructor_count() == 1,                                  // 4
                it_2nd->value().copy_constructor_count() == 0,                                // 5
                it_2nd->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
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
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
        }
    }
}

GTEST_TEST(hashmap, add_by_move_construct_non_bitwise_move_constructible_different_type)
{
    using key_type = hud_test::non_bitwise_move_constructible_type2;
    using other_key_type = hud_test::non_bitwise_move_constructible_type;
    using value_type = hud_test::non_bitwise_move_constructible_type2;
    using other_value_type = hud_test::non_bitwise_move_constructible_type;

    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type, other_value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            other_key_type key_to_copy(1);
            other_value_type value_to_copy(11);
            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 0,                                  // 3
                it_1st->key().move_constructor_count() == 1,                                  // 4
                it_1st->value().copy_constructor_count() == 0,                                // 5
                it_1st->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            other_key_type key_to_copy_1(1);
            other_value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_same->key().copy_constructor_count() == 0,                                 // 3
                it_same->key().move_constructor_count() == 1,                                 // 4
                it_same->value().copy_constructor_count() == 0,                               // 5
                it_same->value().move_constructor_count() == 1,                               // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            other_key_type key_to_copy_2(2);
            other_value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 0,                                  // 3
                it_2nd->key().move_constructor_count() == 1,                                  // 4
                it_2nd->value().copy_constructor_count() == 0,                                // 5
                it_2nd->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 7
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 8
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
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
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
        }
    }

    // Without reallocation
    {
        static const auto test = []() {
            constexpr usize reserved_size = 2;
            hashmap_type map;
            map.reserve(reserved_size);
            other_key_type key_to_copy(1);
            other_value_type value_to_copy(11);

            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 0,                                  // 3
                it_1st->key().move_constructor_count() == 1,                                  // 4
                it_1st->value().copy_constructor_count() == 0,                                // 5
                it_1st->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            other_key_type key_to_copy_1(1);
            other_value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 0,                                 // 3
                it_same->key().move_constructor_count() == 1,                                 // 4
                it_same->value().copy_constructor_count() == 0,                               // 5
                it_same->value().move_constructor_count() == 1,                               // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            other_key_type key_to_copy_2(2);
            other_value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 0,                                  // 3
                it_2nd->key().move_constructor_count() == 1,                                  // 4
                it_2nd->value().copy_constructor_count() == 0,                                // 5
                it_2nd->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
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
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
        }
    }
}

GTEST_TEST(hashmap, add_pair_by_copy_construct_non_bitwise_copy_constructible_same_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;

            const hud::pair<key_type, value_type> pair {1, 11};
            const auto it_1st = map.add(pair);
            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            const hud::pair<key_type, value_type> pair1 {1, 00};
            const auto it_same = map.add(pair1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            const hud::pair<key_type, value_type> pair2 {2, 22};
            const auto it_2nd = map.add(pair2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 5
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 2;
            hashmap_type map;
            map.reserve(reserved_size);
            const hud::pair<key_type, value_type> pair {1, 11};
            const auto it_1st = map.add(pair);
            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            const hud::pair<key_type, value_type> pair1 {1, 00};
            const auto it_same = map.add(pair1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            const hud::pair<key_type, value_type> pair2 {2, 22};
            const auto it_2nd = map.add(pair2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_pair_by_copy_construct_non_bitwise_copy_constructible_different_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type2;
    using other_key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type2;
    using other_value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type, other_value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;

            const hud::pair<other_key_type, other_value_type> pair {1, 11};
            const auto it_1st = map.add(pair);
            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            const hud::pair<other_key_type, other_value_type> pair1 {1, 00};
            const auto it_same = map.add(pair1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            const hud::pair<other_key_type, other_value_type> pair2 {2, 22};
            const auto it_2nd = map.add(pair2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 5
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 2;
            hashmap_type map;
            map.reserve(reserved_size);
            const hud::pair<other_key_type, other_value_type> pair {1, 11};
            const auto it_1st = map.add(pair);
            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            const hud::pair<other_key_type, other_value_type> pair1 {1, 00};
            const auto it_same = map.add(pair1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            const hud::pair<other_key_type, other_value_type> pair2 {2, 22};
            const auto it_2nd = map.add(pair2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_pair_by_copy_construct_bitwise_copy_constructible_same_type)
{
    using key_type = usize;
    using value_type = usize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(hud::is_bitwise_copy_constructible_v<value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            const hud::pair<key_type, value_type> pair {1, 11};
            const auto it_1st = map.add(pair);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const hud::pair<key_type, value_type> pair1 {1, 00};
            const auto it_same = map.add(pair1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const hud::pair<key_type, value_type> pair2 {2, 22};
            const auto it_2nd = map.add(pair2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 3
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 3;
            hashmap_type map;
            map.reserve(reserved_size);
            const hud::pair<key_type, value_type> pair {1, 11};
            const auto it_1st = map.add(pair);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const hud::pair<key_type, value_type> pair1 {1, 00};
            const auto it_same = map.add(pair1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const hud::pair<key_type, value_type> pair2 {2, 22};
            const auto it_2nd = map.add(pair2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };
            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_pair_by_copy_construct_bitwise_copy_constructible_different_type)
{
    using key_type = usize;
    using other_key_type = isize;
    using value_type = usize;
    using other_value_type = isize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);
    static_assert(hud::is_bitwise_copy_constructible_v<value_type, other_value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            const hud::pair<other_key_type, other_value_type> pair {1, 11};
            const auto it_1st = map.add(pair);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const hud::pair<other_key_type, other_value_type> pair1 {1, 00};
            const auto it_same = map.add(pair1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const hud::pair<other_key_type, other_value_type> pair2 {2, 22};
            const auto it_2nd = map.add(pair2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 3
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 3;
            hashmap_type map;
            map.reserve(reserved_size);
            const hud::pair<other_key_type, other_value_type> pair {1, 11};
            const auto it_1st = map.add(pair);

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const hud::pair<other_key_type, other_value_type> pair1 {1, 00};
            const auto it_same = map.add(pair1);
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            const hud::pair<other_key_type, other_value_type> pair2 {2, 22};
            const auto it_2nd = map.add(pair2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };
            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_pair_by_move_construct_non_bitwise_copy_constructible_same_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            hud::pair<key_type, value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.max_count(),                                           // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            hud::pair<key_type, value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.max_count(),                                           // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            hud::pair<key_type, value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.max_count(),                                           // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 5
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 2;
            hashmap_type map;
            map.reserve(reserved_size);
            hud::pair<key_type, value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            hud::pair<key_type, value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            hud::pair<key_type, value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_pair_by_move_construct_non_bitwise_copy_constructible_different_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type2;
    using other_key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type2;
    using other_value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type, other_value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            hud::pair<other_key_type, other_value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.max_count(),                                           // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            hud::pair<other_key_type, other_value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.max_count(),                                           // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            hud::pair<other_key_type, other_value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.max_count(),                                           // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 5
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 2;
            hashmap_type map;
            map.reserve(reserved_size);
            hud::pair<other_key_type, other_value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 1,                                  // 3
                it_1st->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            hud::pair<other_key_type, other_value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 1,                                 // 3
                it_same->value().copy_constructor_count() == 1,                               // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };
            hud::pair<other_key_type, other_value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 1,                                  // 3
                it_2nd->value().copy_constructor_count() == 1,                                // 4
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 5
                map.allocator().free_count() == 0,                                            // 6
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_pair_by_move_construct_bitwise_copy_constructible_same_type)
{
    using key_type = usize;
    using value_type = usize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(hud::is_bitwise_copy_constructible_v<value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            hud::pair<key_type, value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            hud::pair<key_type, value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            hud::pair<key_type, value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 3
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 3;
            hashmap_type map;
            map.reserve(reserved_size);
            hud::pair<key_type, value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            hud::pair<key_type, value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            hud::pair<key_type, value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_pair_by_move_construct_bitwise_copy_constructible_different_type)
{
    using key_type = usize;
    using other_key_type = isize;
    using value_type = usize;
    using other_value_type = isize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);
    static_assert(hud::is_bitwise_copy_constructible_v<value_type, other_value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            hud::pair<other_key_type, other_value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            hud::pair<other_key_type, other_value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            hud::pair<other_key_type, other_value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 3
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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
        static const auto test = []() {
            constexpr usize reserved_size = 3;
            hashmap_type map;
            map.reserve(reserved_size);
            hud::pair<other_key_type, other_value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            hud::pair<other_key_type, other_value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            hud::pair<other_key_type, other_value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 3
                map.allocator().free_count() == 0,                                            // 4
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

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

GTEST_TEST(hashmap, add_pair_by_move_construct_non_bitwise_move_constructible_same_type)
{
    using key_type = hud_test::non_bitwise_move_constructible_type;
    using value_type = hud_test::non_bitwise_move_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            hud::pair<key_type, value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 0,                                  // 3
                it_1st->key().move_constructor_count() == 1,                                  // 4
                it_1st->value().copy_constructor_count() == 0,                                // 5
                it_1st->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            hud::pair<key_type, value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_same->key().copy_constructor_count() == 0,                                 // 3
                it_same->key().move_constructor_count() == 1,                                 // 4
                it_same->value().copy_constructor_count() == 0,                               // 5
                it_same->value().move_constructor_count() == 1,                               // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            hud::pair<key_type, value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 0,                                  // 3
                it_2nd->key().move_constructor_count() == 1,                                  // 4
                it_2nd->value().copy_constructor_count() == 0,                                // 5
                it_2nd->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 7
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 8
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
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
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
        }
    }

    // Without reallocation
    {
        static const auto test = []() {
            constexpr usize reserved_size = 2;
            hashmap_type map;
            map.reserve(reserved_size);
            hud::pair<key_type, value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 0,                                  // 3
                it_1st->key().move_constructor_count() == 1,                                  // 4
                it_1st->value().copy_constructor_count() == 0,                                // 5
                it_1st->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            hud::pair<key_type, value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 0,                                 // 3
                it_same->key().move_constructor_count() == 1,                                 // 4
                it_same->value().copy_constructor_count() == 0,                               // 5
                it_same->value().move_constructor_count() == 1,                               // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            hud::pair<key_type, value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 0,                                  // 3
                it_2nd->key().move_constructor_count() == 1,                                  // 4
                it_2nd->value().copy_constructor_count() == 0,                                // 5
                it_2nd->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
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
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
        }
    }
}

GTEST_TEST(hashmap, add_pair_by_move_construct_non_bitwise_move_constructible_different_type)
{
    using key_type = hud_test::non_bitwise_move_constructible_type2;
    using other_key_type = hud_test::non_bitwise_move_constructible_type;
    using value_type = hud_test::non_bitwise_move_constructible_type2;
    using other_value_type = hud_test::non_bitwise_move_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type, other_key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type, other_value_type>);

    // With reallocation
    {
        static const auto test = []() {
            hashmap_type map;
            hud::pair<other_key_type, other_value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_1st->key().copy_constructor_count() == 0,                                  // 3
                it_1st->key().move_constructor_count() == 1,                                  // 4
                it_1st->value().copy_constructor_count() == 0,                                // 5
                it_1st->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            hud::pair<other_key_type, other_value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_same->key().copy_constructor_count() == 0,                                 // 3
                it_same->key().move_constructor_count() == 1,                                 // 4
                it_same->value().copy_constructor_count() == 0,                               // 5
                it_same->value().move_constructor_count() == 1,                               // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            hud::pair<other_key_type, other_value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= map.count(),                                               // 2
                it_2nd->key().copy_constructor_count() == 0,                                  // 3
                it_2nd->key().move_constructor_count() == 1,                                  // 4
                it_2nd->value().copy_constructor_count() == 0,                                // 5
                it_2nd->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2), // 7
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 8
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
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
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
        }
    }

    // Without reallocation
    {
        static const auto test = []() {
            constexpr usize reserved_size = 2;
            hashmap_type map;
            map.reserve(reserved_size);
            hud::pair<other_key_type, other_value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                it_1st->key() == 1 && it_1st->value() == 11,                                  // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_1st->key().copy_constructor_count() == 0,                                  // 3
                it_1st->key().move_constructor_count() == 1,                                  // 4
                it_1st->value().copy_constructor_count() == 0,                                // 5
                it_1st->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            hud::pair<other_key_type, other_value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                it_same->key() == 1 && it_same->value() == 11,                                // 0
                map.count() == 1,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_same->key().copy_constructor_count() == 0,                                 // 3
                it_same->key().move_constructor_count() == 1,                                 // 4
                it_same->value().copy_constructor_count() == 0,                               // 5
                it_same->value().move_constructor_count() == 1,                               // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };
            hud::pair<other_key_type, other_value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                it_2nd->key() == 2 && it_2nd->value() == 22,                                  // 0
                map.count() == 2,                                                             // 1
                map.max_count() >= reserved_size,                                             // 2
                it_2nd->key().copy_constructor_count() == 0,                                  // 3
                it_2nd->key().move_constructor_count() == 1,                                  // 4
                it_2nd->value().copy_constructor_count() == 0,                                // 5
                it_2nd->value().move_constructor_count() == 1,                                // 6
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 7
                map.allocator().free_count() == 0,                                            // 8
            };

            return std::tuple {
                first_element_result,
                same_element_result,
                second_element_result
            };
        };

        // Non Constant
        {
            const auto result = runtime_test(test);

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_true(std::get<0>(first_element_result));
            hud_assert_true(std::get<1>(first_element_result));
            hud_assert_true(std::get<2>(first_element_result));
            hud_assert_true(std::get<3>(first_element_result));
            hud_assert_true(std::get<4>(first_element_result));
            hud_assert_true(std::get<5>(first_element_result));
            hud_assert_true(std::get<6>(first_element_result));
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
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
            hud_assert_true(std::get<7>(first_element_result));
            hud_assert_true(std::get<8>(first_element_result));

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_true(std::get<0>(same_element_result));
            hud_assert_true(std::get<1>(same_element_result));
            hud_assert_true(std::get<2>(same_element_result));
            hud_assert_true(std::get<3>(same_element_result));
            hud_assert_true(std::get<4>(same_element_result));
            hud_assert_true(std::get<5>(same_element_result));
            hud_assert_true(std::get<6>(same_element_result));
            hud_assert_true(std::get<7>(same_element_result));
            hud_assert_true(std::get<8>(same_element_result));

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_true(std::get<0>(second_element_result));
            hud_assert_true(std::get<1>(second_element_result));
            hud_assert_true(std::get<2>(second_element_result));
            hud_assert_true(std::get<3>(second_element_result));
            hud_assert_true(std::get<4>(second_element_result));
            hud_assert_true(std::get<5>(second_element_result));
            hud_assert_true(std::get<6>(second_element_result));
            hud_assert_true(std::get<7>(second_element_result));
            hud_assert_true(std::get<8>(second_element_result));
        }
    }
}

GTEST_TEST(hashmap, add_by_piecewise_construct_bitwise_same_type)
{
    using key_type = i32;
    using value_type = i64;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static const auto test = []() -> decltype(auto) {
        constexpr usize reserved_size = 2;
        hashmap_type map;
        map.reserve(reserved_size);
        const auto it = map.add(hud::tag_piecewise_construct, hud::forward_as_tuple(1), hud::forward_as_tuple(2));
        return std::tuple {
            map.count() == 1, // 0
            it->key() == 1,   // 1
            it->value() == 2  // 2
        };
    };

    // Non constant
    {
        const auto result = runtime_test(test);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }
}

GTEST_TEST(hashmap, add_by_piecewise_construct_bitwise_different_type)
{
    using key_type = i32;
    using value_type = i64;
    using other_key_type = u32;
    using other_value_type = u64;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static const auto test = []() {
        constexpr usize reserved_size = 2;
        hashmap_type map;
        map.reserve(reserved_size);
        const auto it = map.add(hud::tag_piecewise_construct, hud::forward_as_tuple(other_key_type {1}), hud::forward_as_tuple(other_value_type {2}));

        return std::tuple {
            map.count() == 1, // 0
            it->key() == 1,   // 1
            it->value() == 2  // 2
        };
    };

    // Non constant
    {
        const auto result = runtime_test(test);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }
}

GTEST_TEST(hashmap, add_by_piecewise_construct_non_bitwise_same_type)
{
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static const auto test = []() {
        constexpr usize reserved_size = 2;
        i32 ptr[2];
        hashmap_type map;
        map.reserve(reserved_size);

        static_assert(hud::is_hashable_64_v<key_type, decltype(hud::forward_as_tuple(1, ptr))>);
        static_assert(hud::is_comparable_with_equal_v<key_type, decltype(hud::forward_as_tuple(1, ptr))>);

        const auto it = map.add(hud::tag_piecewise_construct, hud::forward_as_tuple(1, ptr), hud::forward_as_tuple(2, ptr + 1));

        return std::tuple {
            map.count() == 1,                           // 0
            it->key().constructor_count() == 1,         // 1
            it->key().move_constructor_count() == 0,    // 2
            it->key().copy_constructor_count() == 0,    // 3
            it->key().move_assign_count() == 0,         // 4
            it->key().copy_assign_count() == 0,         // 5
            it->key().id() == 1,                        // 6
            it->key().destructor_counter() == ptr,      // 7
            it->value().constructor_count() == 1,       // 8
            it->value().move_constructor_count() == 0,  // 9
            it->value().copy_constructor_count() == 0,  // 10
            it->value().move_assign_count() == 0,       // 11
            it->value().copy_assign_count() == 0,       // 12
            it->value().id() == 2,                      // 13
            it->value().destructor_counter() == ptr + 1 // 14
        };
    };

    // Non constant
    {
        const auto result = runtime_test(test);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
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
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
    }
}

GTEST_TEST(hashmap, add_by_piecewise_construct_non_bitwise_different_type)
{
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static const auto test = []() {
        constexpr usize reserved_size = 2;
        i32 ptr[2];
        hashmap_type map;
        map.reserve(reserved_size);

        static_assert(hud::is_hashable_64_v<key_type, decltype(hud::forward_as_tuple(i64 {1}, ptr))>);
        static_assert(hud::is_comparable_with_equal_v<key_type, decltype(hud::forward_as_tuple(i64 {1}, ptr))>);

        const auto it = map.add(hud::tag_piecewise_construct, hud::forward_as_tuple(i64 {1}, ptr), hud::forward_as_tuple(u64 {2}, ptr + 1));

        return std::tuple {
            map.count() == 1,                           // 0
            it->key().constructor_count() == 1,         // 1
            it->key().move_constructor_count() == 0,    // 2
            it->key().copy_constructor_count() == 0,    // 3
            it->key().move_assign_count() == 0,         // 4
            it->key().copy_assign_count() == 0,         // 5
            it->key().id() == 1,                        // 6
            it->key().destructor_counter() == ptr,      // 7
            it->value().constructor_count() == 1,       // 8
            it->value().move_constructor_count() == 0,  // 9
            it->value().copy_constructor_count() == 0,  // 10
            it->value().move_assign_count() == 0,       // 11
            it->value().copy_assign_count() == 0,       // 12
            it->value().id() == 2,                      // 13
            it->value().destructor_counter() == ptr + 1 // 14
        };
    };

    // Non constant
    {
        const auto result = runtime_test(test);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
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
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
    }
}

GTEST_TEST(hashmap, add_by_piecewise_construct_tuples)
{
    using key_type = hud::tuple<i32, u64, hud_test::non_bitwise_type>;
    using value_type = hud::tuple<i32, u64, hud_test::non_bitwise_type>;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, hud_test::allocator_watcher<1>>;

    static const auto test = []() {
        constexpr usize reserved_size = 2;
        i32 ptr[2];
        hashmap_type map;
        map.reserve(reserved_size);

        static_assert(hud::is_hashable_64_v<key_type, decltype(hud::forward_as_tuple(1, 2, hud_test::non_bitwise_type {3, ptr}))>);
        static_assert(hud::is_comparable_with_equal_v<key_type, decltype(hud::forward_as_tuple(1, 2, hud_test::non_bitwise_type {3, ptr}))>);

        const auto it = map.add(hud::tag_piecewise_construct, hud::forward_as_tuple(1, 2, hud_test::non_bitwise_type {3, ptr}), hud::forward_as_tuple(4, 5, hud_test::non_bitwise_type {6, ptr + 1}));

        const key_type expected_key {
            1,
            2,
            hud_test::non_bitwise_type {3, ptr}
        };
        const value_type expected_value {
            4,
            5,
            hud_test::non_bitwise_type {6, ptr + 1}
        };
        return std::tuple {
            map.count() == 1,              // 0
            it->key() == expected_key,     // 1
            it->value() == expected_value, // 2
        };
    };

    // Non constant
    {
        const auto result = runtime_test(test);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }
}
