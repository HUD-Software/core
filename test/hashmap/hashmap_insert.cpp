#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

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

    auto result = test();

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
    }
    // hud::hashmap<i32, u64> map;
    // hud_assert_eq(map.insert_to_ref(1, 11), 11);
    // u64 res1 = map.insert_to_ref(2, 22);
    // res = map.insert_to_ref(3, 33);
    //
}