#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashmap, add_by_copy_construct_non_bitwise_copy_constructible_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type, value_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            const key_type key_to_copy(1);
            const value_type value_to_copy(11);
            const auto it_1st = map.add(key_to_copy, value_to_copy);

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const key_type key_to_copy_1(1);
            const value_type value_to_copy_1(00);
            const auto it_same = map.add(key_to_copy_1, value_to_copy_1);
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const key_type key_to_copy_2(2);
            const value_type value_to_copy_2(22);
            const auto it_2nd = map.add(key_to_copy_2, value_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 1u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 2u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 2u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 4u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(2);
            const key_type key_to_copy(1);
            const value_type value_to_copy(11);

            const auto it_1st = map.add(key_to_copy, value_to_copy);

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const key_type key_to_copy_1(1);
            const value_type value_to_copy_1(00);
            const auto it_same = map.add(key_to_copy_1, value_to_copy_1);
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const key_type key_to_copy_2(2);
            const value_type value_to_copy_2(22);
            const auto it_2nd = map.add(key_to_copy_2, value_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 1u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 2u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 2u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(hashmap, add_by_copy_construct_bitwise_copy_constructible_type)
{
    using key_type = usize;
    using value_type = usize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, value_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            const key_type key_to_copy(1);
            const value_type value_to_copy(11);
            const auto it_1st = map.add(key_to_copy, value_to_copy);

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const key_type key_to_copy_1(1);
            const value_type value_to_copy_1(00);
            const auto it_same = map.add(key_to_copy_1, value_to_copy_1);
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const key_type key_to_copy_2(2);
            const value_type value_to_copy_2(22);
            const auto it_2nd = map.add(key_to_copy_2, value_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 2u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 4u);
            hud_assert_eq(std::get<4>(second_element_result), 2u);
        }
    }
    // Without reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(3);
            const key_type key_to_copy(1);
            const value_type value_to_copy(11);
            const auto it_1st = map.add(key_to_copy, value_to_copy);

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const key_type key_to_copy_1(1);
            const value_type value_to_copy_1(00);
            const auto it_same = map.add(key_to_copy_1, value_to_copy_1);
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const key_type key_to_copy_2(2);
            const value_type value_to_copy_2(22);
            const auto it_2nd = map.add(key_to_copy_2, value_to_copy_2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 0u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 2u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(hashmap, add_inplace_by_copy_construct_non_bitwise_copy_constructible_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type, value_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            const auto it_1st = map.add(1, 11);

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const auto it_same = map.add(1, 00);
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const auto it_2nd = map.add(2, 22);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 1u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 2u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 2u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 4u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(2);

            const auto it_1st = map.add(1, 11);

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const auto it_same = map.add(1, 00);
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const auto it_2nd = map.add(2, 22);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 1u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 2u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 2u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(hashmap, add_by_move_construct_non_bitwise_copy_constructible_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, key_type>);
    static_assert(hud::is_copy_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type, value_type>);
    static_assert(hud::is_copy_constructible_v<value_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            key_type key_to_copy(1);
            value_type value_to_copy(11);
            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            key_type key_to_copy_1(1);
            value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            key_type key_to_copy_2(2);
            value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 1u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 2u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 2u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 4u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(2);
            key_type key_to_copy(1);
            value_type value_to_copy(11);

            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            key_type key_to_copy_1(1);
            value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            key_type key_to_copy_2(2);
            value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 1u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 2u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 2u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(hashmap, add_by_move_construct_bitwise_copy_constructible_type)
{
    using key_type = usize;
    using value_type = usize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, value_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            key_type key_to_copy(1);
            value_type value_to_copy(11);
            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            key_type key_to_copy_1(1);
            value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            key_type key_to_copy_2(2);
            value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 2u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 4u);
            hud_assert_eq(std::get<4>(second_element_result), 2u);
        }
    }
    // Without reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(3);
            key_type key_to_copy(1);
            value_type value_to_copy(11);
            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            key_type key_to_copy_1(1);
            value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            key_type key_to_copy_2(2);
            value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 0u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 2u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(hashmap, add_by_move_construct_non_bitwise_move_constructible_type)
{
    using key_type = hud_test::non_bitwise_move_constructible_type;
    using value_type = hud_test::non_bitwise_move_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type, key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type, value_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            key_type key_to_copy(1);
            value_type value_to_copy(11);
            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->key().move_constructor_count(),
                it_1st->value().copy_constructor_count(),
                it_1st->value().move_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            key_type key_to_copy_1(1);
            value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->key().move_constructor_count(),
                it_same->value().copy_constructor_count(),
                it_same->value().move_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            key_type key_to_copy_2(2);
            value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->key().move_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                it_2nd->value().move_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 0u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 0u);
            hud_assert_eq(std::get<6>(first_element_result), 1u);
            hud_assert_eq(std::get<7>(first_element_result), 1u);
            hud_assert_eq(std::get<8>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 0u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 0u);
            hud_assert_eq(std::get<6>(same_element_result), 1u);
            hud_assert_eq(std::get<7>(same_element_result), 1u);
            hud_assert_eq(std::get<8>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 0u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 0u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 2u);
            hud_assert_eq(std::get<8>(second_element_result), 1u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 0u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 0u);
            hud_assert_eq(std::get<6>(first_element_result), 1u);
            hud_assert_eq(std::get<7>(first_element_result), 2u);
            hud_assert_eq(std::get<8>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 0u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 0u);
            hud_assert_eq(std::get<6>(same_element_result), 1u);
            hud_assert_eq(std::get<7>(same_element_result), 2u);
            hud_assert_eq(std::get<8>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 0u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 0u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 4u);
            hud_assert_eq(std::get<8>(second_element_result), 2u);
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(2);
            key_type key_to_copy(1);
            value_type value_to_copy(11);

            const auto it_1st = map.add(hud::move(key_to_copy), hud::move(value_to_copy));

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->key().move_constructor_count(),
                it_1st->value().copy_constructor_count(),
                it_1st->value().move_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            key_type key_to_copy_1(1);
            value_type value_to_copy_1(00);
            const auto it_same = map.add(hud::move(key_to_copy_1), hud::move(value_to_copy_1));
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->key().move_constructor_count(),
                it_same->value().copy_constructor_count(),
                it_same->value().move_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            key_type key_to_copy_2(2);
            value_type value_to_copy_2(22);
            const auto it_2nd = map.add(hud::move(key_to_copy_2), hud::move(value_to_copy_2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->key().move_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                it_2nd->value().move_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 0u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 0u);
            hud_assert_eq(std::get<6>(first_element_result), 1u);
            hud_assert_eq(std::get<7>(first_element_result), 1u);
            hud_assert_eq(std::get<8>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 0u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 0u);
            hud_assert_eq(std::get<6>(same_element_result), 1u);
            hud_assert_eq(std::get<7>(same_element_result), 1u);
            hud_assert_eq(std::get<8>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 0u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 0u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 1u);
            hud_assert_eq(std::get<8>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 0u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 0u);
            hud_assert_eq(std::get<6>(first_element_result), 1u);
            hud_assert_eq(std::get<7>(first_element_result), 2u);
            hud_assert_eq(std::get<8>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 0u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 0u);
            hud_assert_eq(std::get<6>(same_element_result), 1u);
            hud_assert_eq(std::get<7>(same_element_result), 2u);
            hud_assert_eq(std::get<8>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 0u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 0u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 2u);
            hud_assert_eq(std::get<8>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(hashmap, add_pair_by_copy_construct_non_bitwise_copy_constructible_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type, value_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type map;

            const hud::pair<key_type, value_type> pair {1, 11};
            const auto it_1st = map.add(pair);
            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const hud::pair<key_type, value_type> pair1 {1, 00};
            const auto it_same = map.add(pair1);
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const hud::pair<key_type, value_type> pair2 {2, 22};
            const auto it_2nd = map.add(pair2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 1u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 2u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 2u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 4u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(2);
            const hud::pair<key_type, value_type> pair {1, 11};
            const auto it_1st = map.add(pair);
            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const hud::pair<key_type, value_type> pair1 {1, 00};
            const auto it_same = map.add(pair1);
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const hud::pair<key_type, value_type> pair2 {2, 22};
            const auto it_2nd = map.add(pair2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 1u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 2u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 2u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(hashmap, add_pair_by_copy_construct_bitwise_copy_constructible_type)
{
    using key_type = usize;
    using value_type = usize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, value_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            const hud::pair<key_type, value_type> pair {1, 11};
            const auto it_1st = map.add(pair);

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const hud::pair<key_type, value_type> pair1 {1, 00};
            const auto it_same = map.add(pair1);
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const hud::pair<key_type, value_type> pair2 {2, 22};
            const auto it_2nd = map.add(pair2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 2u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 4u);
            hud_assert_eq(std::get<4>(second_element_result), 2u);
        }
    }
    // Without reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(3);
            const hud::pair<key_type, value_type> pair {1, 11};
            const auto it_1st = map.add(pair);

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const hud::pair<key_type, value_type> pair1 {1, 00};
            const auto it_same = map.add(pair1);
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const hud::pair<key_type, value_type> pair2 {2, 22};
            const auto it_2nd = map.add(pair2);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 0u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 2u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(hashmap, add_pair_inplace_by_copy_construct_non_bitwise_copy_constructible_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type, value_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            const auto it_1st = map.add({1, 11});

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const auto it_same = map.add({1, 00});
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const auto it_2nd = map.add({2, 22});
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 1u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 2u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 2u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 4u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(2);

            const auto it_1st = map.add({1, 11});

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const auto it_same = map.add({1, 00});
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            const auto it_2nd = map.add({2, 22});
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 1u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 2u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 2u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(hashmap, add_pair_by_move_construct_non_bitwise_copy_constructible_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, key_type>);
    static_assert(hud::is_copy_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type, value_type>);
    static_assert(hud::is_copy_constructible_v<value_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            hud::pair<key_type, value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            hud::pair<key_type, value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            hud::pair<key_type, value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 1u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 2u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 2u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 4u);
            hud_assert_eq(std::get<6>(second_element_result), 2u);
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(2);
            hud::pair<key_type, value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            hud::pair<key_type, value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            hud::pair<key_type, value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 1u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 1u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 1u);
            hud_assert_eq(std::get<6>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 2u);
            hud_assert_eq(std::get<6>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 2u);
            hud_assert_eq(std::get<6>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 2u);
            hud_assert_eq(std::get<6>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(hashmap, add_pair_by_move_construct_bitwise_copy_constructible_type)
{
    using key_type = usize;
    using value_type = usize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, value_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            hud::pair<key_type, value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            hud::pair<key_type, value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            hud::pair<key_type, value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 2u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 4u);
            hud_assert_eq(std::get<4>(second_element_result), 2u);
        }
    }
    // Without reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(3);
            hud::pair<key_type, value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            hud::pair<key_type, value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            hud::pair<key_type, value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 1u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 1u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 1u);
            hud_assert_eq(std::get<4>(second_element_result), 0u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 2u);
            hud_assert_eq(std::get<4>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 2u);
            hud_assert_eq(std::get<4>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 2u);
            hud_assert_eq(std::get<4>(second_element_result), 0u);
        }
    }
}

GTEST_TEST(hashmap, add_pair_by_move_construct_non_bitwise_move_constructible_type)
{
    using key_type = hud_test::non_bitwise_move_constructible_type;
    using value_type = hud_test::non_bitwise_move_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(!hud::is_bitwise_move_constructible_v<key_type, key_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<value_type, value_type>);

    // With reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            hud::pair<key_type, value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->key().move_constructor_count(),
                it_1st->value().copy_constructor_count(),
                it_1st->value().move_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            hud::pair<key_type, value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->key().move_constructor_count(),
                it_same->value().copy_constructor_count(),
                it_same->value().move_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            hud::pair<key_type, value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->key().move_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                it_2nd->value().move_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 0u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 0u);
            hud_assert_eq(std::get<6>(first_element_result), 1u);
            hud_assert_eq(std::get<7>(first_element_result), 1u);
            hud_assert_eq(std::get<8>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 0u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 0u);
            hud_assert_eq(std::get<6>(same_element_result), 1u);
            hud_assert_eq(std::get<7>(same_element_result), 1u);
            hud_assert_eq(std::get<8>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 0u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 0u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 2u);
            hud_assert_eq(std::get<8>(second_element_result), 1u);
        }
        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 1u);
            hud_assert_eq(std::get<3>(first_element_result), 0u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 0u);
            hud_assert_eq(std::get<6>(first_element_result), 1u);
            hud_assert_eq(std::get<7>(first_element_result), 2u);
            hud_assert_eq(std::get<8>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 1u);
            hud_assert_eq(std::get<3>(same_element_result), 0u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 0u);
            hud_assert_eq(std::get<6>(same_element_result), 1u);
            hud_assert_eq(std::get<7>(same_element_result), 2u);
            hud_assert_eq(std::get<8>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 0u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 0u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 4u);
            hud_assert_eq(std::get<8>(second_element_result), 2u);
        }
    }

    // Without reallocation
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(2);
            hud::pair<key_type, value_type> pair(1, 11);
            const auto it_1st = map.add(hud::move(pair));

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->key().copy_constructor_count(),
                it_1st->key().move_constructor_count(),
                it_1st->value().copy_constructor_count(),
                it_1st->value().move_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            hud::pair<key_type, value_type> pair1(1, 00);
            const auto it_same = map.add(hud::move(pair1));
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->key().copy_constructor_count(),
                it_same->key().move_constructor_count(),
                it_same->value().copy_constructor_count(),
                it_same->value().move_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };
            hud::pair<key_type, value_type> pair2(2, 22);
            const auto it_2nd = map.add(hud::move(pair2));
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->key().copy_constructor_count(),
                it_2nd->key().move_constructor_count(),
                it_2nd->value().copy_constructor_count(),
                it_2nd->value().move_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 0u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 0u);
            hud_assert_eq(std::get<6>(first_element_result), 1u);
            hud_assert_eq(std::get<7>(first_element_result), 1u);
            hud_assert_eq(std::get<8>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 0u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 0u);
            hud_assert_eq(std::get<6>(same_element_result), 1u);
            hud_assert_eq(std::get<7>(same_element_result), 1u);
            hud_assert_eq(std::get<8>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 0u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 0u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 1u);
            hud_assert_eq(std::get<8>(second_element_result), 0u);
        }

        // Constant
        {
            constexpr auto result = test();

            // First element is correctly added
            const auto first_element_result = std::get<0>(result);
            hud_assert_eq(std::get<0>(first_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(first_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(first_element_result), 1u);
            hud_assert_eq(std::get<2>(first_element_result), 3u);
            hud_assert_eq(std::get<3>(first_element_result), 0u);
            hud_assert_eq(std::get<4>(first_element_result), 1u);
            hud_assert_eq(std::get<5>(first_element_result), 0u);
            hud_assert_eq(std::get<6>(first_element_result), 1u);
            hud_assert_eq(std::get<7>(first_element_result), 2u);
            hud_assert_eq(std::get<8>(first_element_result), 0u);

            // Same element
            const auto same_element_result = std::get<1>(result);
            hud_assert_eq(std::get<0>(same_element_result).key(), 1u);
            hud_assert_eq(std::get<0>(same_element_result).value(), 11u);
            hud_assert_eq(std::get<1>(same_element_result), 1u);
            hud_assert_eq(std::get<2>(same_element_result), 3u);
            hud_assert_eq(std::get<3>(same_element_result), 0u);
            hud_assert_eq(std::get<4>(same_element_result), 1u);
            hud_assert_eq(std::get<5>(same_element_result), 0u);
            hud_assert_eq(std::get<6>(same_element_result), 1u);
            hud_assert_eq(std::get<7>(same_element_result), 2u);
            hud_assert_eq(std::get<8>(same_element_result), 0u);

            // 2nd element
            const auto second_element_result = std::get<2>(result);
            hud_assert_eq(std::get<0>(second_element_result).key(), 2u);
            hud_assert_eq(std::get<0>(second_element_result).value(), 22u);
            hud_assert_eq(std::get<1>(second_element_result), 2u);
            hud_assert_eq(std::get<2>(second_element_result), 3u);
            hud_assert_eq(std::get<3>(second_element_result), 0u);
            hud_assert_eq(std::get<4>(second_element_result), 1u);
            hud_assert_eq(std::get<5>(second_element_result), 0u);
            hud_assert_eq(std::get<6>(second_element_result), 1u);
            hud_assert_eq(std::get<7>(second_element_result), 2u);
            hud_assert_eq(std::get<8>(second_element_result), 0u);
        }
    }
}
