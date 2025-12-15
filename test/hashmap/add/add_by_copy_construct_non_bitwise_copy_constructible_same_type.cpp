#include <core/containers/hashmap.h>
#include "../../misc/allocator_watcher.h"

GTEST_TEST(hashmap, add_by_copy_construct_non_bitwise_copy_constructible_same_type)
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
            const key_type key_to_copy(1);
            const value_type value_to_copy(11);
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
            const key_type key_to_copy_1(1);
            const value_type value_to_copy_1(00);
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
            const key_type key_to_copy_2(2);
            const value_type value_to_copy_2(22);
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
            map.reserve(reserved_size);
            const key_type key_to_copy(1);
            const value_type value_to_copy(11);
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
            const key_type key_to_copy_1(1);
            const value_type value_to_copy_1(00);
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
            const key_type key_to_copy_2(2);
            const value_type value_to_copy_2(22);
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
