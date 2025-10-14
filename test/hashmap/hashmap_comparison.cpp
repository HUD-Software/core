#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, equal_operator)
{
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type>;

    static const auto test = []() {
        hashmap_type map;
        hashmap_type map_2;
        bool empty_empty_same = (map == map_2);

        map.add(1, 11);
        map.add(2, 22);
        map.add(3, 33);
        bool non_empty_empty_not_same = !(map == map_2);

        map_2.add(1, 11);
        bool non_empty_non_empty_smaller_not_same = !(map == map_2);

        map_2.add(2, 22);
        map_2.add(3, 33);
        bool non_empty_non_empty_same = (map == map_2);

        map_2.add(4, 44);
        bool non_empty_non_empty_greater_not_same = !(map == map_2);

        // Reserve to match the max_count of map_2
        map.reserve(5);
        non_empty_non_empty_greater_not_same &= !(map == map_2);

        // Reserve to have more max_count than map_2
        map.add(4, 44);
        non_empty_non_empty_same &= (map == map_2);

        // Reserve more but same elements
        map.reserve(10);
        non_empty_non_empty_same &= (map == map_2);

        return std::tuple {
            empty_empty_same,                     // 0
            non_empty_empty_not_same,             // 1
            non_empty_non_empty_smaller_not_same, // 2
            non_empty_non_empty_same,             // 3
            non_empty_non_empty_greater_not_same, // 4

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
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
    }
}

GTEST_TEST(hashmap, not_equal_operator)
{
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;
    using hashmap_type = hud::hashmap<key_type, value_type>;

    static const auto test = []() {
        hashmap_type map;
        hashmap_type map_2;
        bool empty_empty_same = !(map != map_2);

        map.add(1, 11);
        map.add(2, 22);
        map.add(3, 33);
        bool non_empty_empty_not_same = (map != map_2);

        map_2.add(1, 11);
        bool non_empty_non_empty_smaller_not_same = (map != map_2);

        map_2.add(2, 22);
        map_2.add(3, 33);
        bool non_empty_non_empty_same = !(map != map_2);

        map_2.add(4, 44);
        bool non_empty_non_empty_greater_not_same = (map != map_2);

        // Reserve to match the max_count of map_2
        map.reserve(5);
        non_empty_non_empty_greater_not_same &= (map != map_2);

        // Reserve to have more max_count than map_2
        map.add(4, 44);
        non_empty_non_empty_same &= !(map != map_2);

        // Reserve more but same elements
        map.reserve(10);
        non_empty_non_empty_same &= !(map != map_2);

        return std::tuple {
            empty_empty_same,                     // 0
            non_empty_empty_not_same,             // 1
            non_empty_non_empty_smaller_not_same, // 2
            non_empty_non_empty_same,             // 3
            non_empty_non_empty_greater_not_same, // 4

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
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
    }
}