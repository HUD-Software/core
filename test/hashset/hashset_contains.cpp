#include <core/containers/hashset.h>

GTEST_TEST(hashset, contains_in_empty_hashset)
{
    hud::hashset<usize> set;
    hud_assert_false(set.contains(1));
}

GTEST_TEST(hashset, contains_in_hashset_trivial_type)
{
    const auto test = [](usize max) {
        using KeyType = usize;
        hud::hashset<KeyType> set;

        for (u32 index = 0; index < max; index++) {
            set.add(index);
        }

        bool is_find_ok = true;
        // Find elements that exists
        for (u32 index = 0; index < max; index++) {
            is_find_ok &= set.contains(index);
        }

        // Find elements that not exists
        for (u32 index = max; index < max * 2; index++) {
            is_find_ok &= !set.contains(index);
        }
        return is_find_ok;
    };

    // Non constant
    {
        const auto result = runtime_test(test, 32768);
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result1 = test(1);
        hud_assert_true(result1);
        constexpr auto result2 = test(2);
        hud_assert_true(result2);
        constexpr auto result3 = test(3);
        hud_assert_true(result3);
        constexpr auto result4 = test(4);
        hud_assert_true(result4);
        constexpr auto result5 = test(5);
        hud_assert_true(result5);
        constexpr auto result6 = test(6);
        hud_assert_true(result6);
        constexpr auto result7 = test(7);
        hud_assert_true(result7);
        constexpr auto result8 = test(8);
        hud_assert_true(result8);
        constexpr auto result9 = test(9);
        hud_assert_true(result9);
        constexpr auto result10 = test(10);
        hud_assert_true(result10);
        constexpr auto result11 = test(11);
        hud_assert_true(result11);
        constexpr auto result12 = test(12);
        hud_assert_true(result12);
        constexpr auto result13 = test(13);
        hud_assert_true(result13);
        constexpr auto result14 = test(14);
        hud_assert_true(result14);
        constexpr auto result15 = test(15);
        hud_assert_true(result15);
        constexpr auto result16 = test(16);
        hud_assert_true(result16);
        constexpr auto result32 = test(32);
        hud_assert_true(result32);
        constexpr auto result64 = test(64);
        hud_assert_true(result64);
        constexpr auto result128 = test(128);
        hud_assert_true(result128);
    }
}

GTEST_TEST(hashset, contains_in_hashset_non_trivial_type)
{
    const auto test = [](usize max) {
        using KeyType = hud_test::non_bitwise_type;
        hud::hashset<KeyType> set;
        for (u32 index = 0; index < max; index++) {
            set.add(index);
        }

        bool is_find_ok = true;
        // Find elements that exists
        for (u32 index = 0; index < max; index++) {
            is_find_ok &= set.contains(index);
        }

        // Find elements that not exists
        for (u32 index = max; index < max * 2; index++) {
            is_find_ok &= !set.contains(index);
        }

        // Find elements that exists
        for (u32 index = 0; index < max; index++) {
            is_find_ok &= set.contains(KeyType(index));
        }

        // Find elements that not exists
        for (u32 index = max; index < max * 2; index++) {
            is_find_ok &= !set.contains(KeyType(index));
        }

        return is_find_ok;
    };

    // Non constant
    {
        const auto result = runtime_test(test, 32768);
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result1 = test(1);
        hud_assert_true(result1);
        constexpr auto result2 = test(2);
        hud_assert_true(result2);
        constexpr auto result3 = test(3);
        hud_assert_true(result3);
        constexpr auto result4 = test(4);
        hud_assert_true(result4);
        constexpr auto result5 = test(5);
        hud_assert_true(result5);
        constexpr auto result6 = test(6);
        hud_assert_true(result6);
        constexpr auto result7 = test(7);
        hud_assert_true(result7);
        constexpr auto result8 = test(8);
        hud_assert_true(result8);
        constexpr auto result9 = test(9);
        hud_assert_true(result9);
        constexpr auto result10 = test(10);
        hud_assert_true(result10);
        constexpr auto result11 = test(11);
        hud_assert_true(result11);
        constexpr auto result12 = test(12);
        hud_assert_true(result12);
        constexpr auto result13 = test(13);
        hud_assert_true(result13);
        constexpr auto result14 = test(14);
        hud_assert_true(result14);
        constexpr auto result15 = test(15);
        hud_assert_true(result15);
        constexpr auto result16 = test(16);
        hud_assert_true(result16);
        constexpr auto result32 = test(32);
        hud_assert_true(result32);
        constexpr auto result64 = test(64);
        hud_assert_true(result64);
        constexpr auto result128 = test(128);
        hud_assert_true(result128);
    }
}