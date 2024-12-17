#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashmap, add_by_copy_construct_non_bitwise_copy_constructible_type)
{

    using key_type = hud_test::non_bitwise_copy_assignable_type;
    using value_type = hud_test::non_bitwise_copy_assignable_type;
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
                it_1st->first.copy_assign_count(),
                it_1st->first.copy_constructor_count(),
                it_1st->second.copy_assign_count(),
                it_1st->first.copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const auto it_same = map.add(1, 00);
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->first.copy_assign_count(),
                it_same->first.copy_constructor_count(),
                it_same->second.copy_assign_count(),
                it_same->first.copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const auto it_2nd = map.add(2, 22);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->first.copy_assign_count(),
                it_2nd->first.copy_constructor_count(),
                it_2nd->second.copy_assign_count(),
                it_2nd->first.copy_constructor_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).first, 1u);
            hud_assert_eq(std::get<0>(first_element_result).second, 11u);
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
            hud_assert_eq(std::get<0>(same_element_result).first, 1u);
            hud_assert_eq(std::get<0>(same_element_result).second, 11u);
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
            hud_assert_eq(std::get<0>(second_element_result).first, 2u);
            hud_assert_eq(std::get<0>(second_element_result).second, 22u);
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
            hud_assert_eq(std::get<0>(first_element_result).first, 1u);
            hud_assert_eq(std::get<0>(first_element_result).second, 11u);
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
            hud_assert_eq(std::get<0>(same_element_result).first, 1u);
            hud_assert_eq(std::get<0>(same_element_result).second, 11u);
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
            hud_assert_eq(std::get<0>(second_element_result).first, 2u);
            hud_assert_eq(std::get<0>(second_element_result).second, 22u);
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
            const auto it_1st = map.add(1, 11);

            const auto first_element_result = std::tuple {
                *it_1st,
                map.count(),
                map.max_count(),
                it_1st->first.copy_assign_count(),
                it_1st->first.copy_constructor_count(),
                it_1st->second.copy_assign_count(),
                it_1st->first.copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const auto it_same = map.add(1, 00);
            // Add same element
            const auto same_element_result = std::tuple {
                *it_same,
                map.count(),
                map.max_count(),
                it_same->first.copy_assign_count(),
                it_same->first.copy_constructor_count(),
                it_same->second.copy_assign_count(),
                it_same->first.copy_constructor_count(),
                map.allocator().allocation_count(),
                map.allocator().free_count(),
            };

            const auto it_2nd = map.add(2, 22);
            // Add 2nd element
            const auto second_element_result = std::tuple {
                *it_2nd,
                map.count(),
                map.max_count(),
                it_2nd->first.copy_assign_count(),
                it_2nd->first.copy_constructor_count(),
                it_2nd->second.copy_assign_count(),
                it_2nd->first.copy_constructor_count(),
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
            hud_assert_eq(std::get<0>(first_element_result).first, 1u);
            hud_assert_eq(std::get<0>(first_element_result).second, 11u);
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
            hud_assert_eq(std::get<0>(same_element_result).first, 1u);
            hud_assert_eq(std::get<0>(same_element_result).second, 11u);
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
            hud_assert_eq(std::get<0>(second_element_result).first, 2u);
            hud_assert_eq(std::get<0>(second_element_result).second, 22u);
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
            hud_assert_eq(std::get<0>(first_element_result).first, 1u);
            hud_assert_eq(std::get<0>(first_element_result).second, 11u);
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
            hud_assert_eq(std::get<0>(same_element_result).first, 1u);
            hud_assert_eq(std::get<0>(same_element_result).second, 11u);
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
            hud_assert_eq(std::get<0>(second_element_result).first, 2u);
            hud_assert_eq(std::get<0>(second_element_result).second, 22u);
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

GTEST_TEST(hashmap, add_by_copy_construct_bitwise_copy_constructible_type)
{
    using key_type = usize;
    using value_type = usize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, value_type>);

    const auto test = []()
    {
        hashmap_type map;

        const auto it_1st = map.add(1, 11);

        const auto first_element_result = std::tuple {
            *it_1st,
            map.count(),
            map.max_count(),
            map.allocator().allocation_count(),
            map.allocator().free_count(),
        };

        const auto it_same = map.add(1, 00);
        // Add same element
        const auto same_element_result = std::tuple {
            *it_same,
            map.count(),
            map.max_count(),
            map.allocator().allocation_count(),
            map.allocator().free_count(),
        };

        const auto it_2nd = map.add(2, 22);
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