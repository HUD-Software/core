#include <core/containers/hashset.h>

GTEST_TEST(hashset, equal_operator)
{
    using key_type = hud_test::non_bitwise_type;
    using hashset_type = hud::hashset<key_type>;

    const auto test = []()
    {
        hashset_type set;
        hashset_type set_2;
        bool empty_empty_same = (set == set_2);

        set.add(1);
        set.add(2);
        set.add(3);
        bool non_empty_empty_not_same = !(set == set_2);

        set_2.add(1);
        bool non_empty_non_empty_smaller_not_same = !(set == set_2);

        set_2.add(2);
        set_2.add(3);
        bool non_empty_non_empty_same = (set == set_2);

        set_2.add(4);
        bool non_empty_non_empty_greater_not_same = !(set == set_2);

        // Reserve to match the max_count of set_2
        set.reserve(5);
        non_empty_non_empty_greater_not_same &= !(set == set_2);

        // Reserve to have more max_count than set_2
        set.add(4);
        non_empty_non_empty_same &= (set == set_2);

        // Reserve more but same elements
        set.reserve(10);
        non_empty_non_empty_same &= (set == set_2);

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
        const auto result = test();
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

GTEST_TEST(hashset, not_equal_operator)
{
    using key_type = hud_test::non_bitwise_type;
    using hashset_type = hud::hashset<key_type>;

    const auto test = []()
    {
        hashset_type set;
        hashset_type set_2;
        bool empty_empty_same = !(set != set_2);

        set.add(1);
        set.add(2);
        set.add(3);
        bool non_empty_empty_not_same = (set != set_2);

        set_2.add(1);
        bool non_empty_non_empty_smaller_not_same = (set != set_2);

        set_2.add(2);
        set_2.add(3);
        bool non_empty_non_empty_same = !(set != set_2);

        set_2.add(4);
        bool non_empty_non_empty_greater_not_same = (set != set_2);

        // Reserve to match the max_count of set_2
        set.reserve(5);
        non_empty_non_empty_greater_not_same &= (set != set_2);

        // Reserve to have more max_count than set_2
        set.add(4);
        non_empty_non_empty_same &= !(set != set_2);

        // Reserve more but same elements
        set.reserve(10);
        non_empty_non_empty_same &= !(set != set_2);

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
        const auto result = test();
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