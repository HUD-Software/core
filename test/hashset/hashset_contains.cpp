#include <core/containers/hashset.h>

GTEST_TEST(hashset, contains_in_empty_hashset)
{
    hud::hashset<usize> set;
    hud_assert_false(set.contains(1));
}

GTEST_TEST(hashset, contains_in_hashset_trivial_type)
{
    hud_test::for_each_value(std::make_integer_sequence<usize, 64>(), []<usize maxx>()
                             {
                                 const auto test = [](usize max)
                                 {
                                     using KeyType = usize;
                                     hud::hashset<KeyType> set;

                                     for (u32 index = 0; index < max; index++)
                                     {
                                         set.add(index);
                                     }

                                     bool is_find_ok = true;
                                     // Find elements that exists
                                     for (u32 index = 0; index < max; index++)
                                     {
                                         is_find_ok &= set.contains(index);
                                     }

                                     // Find elements that not exists
                                     for (u32 index = max; index < max * 2; index++)
                                     {
                                         is_find_ok &= !set.contains(index);
                                     }
                                     return is_find_ok;
                                 };

                                 // Non constant
                                 {
                                     const auto result = test(maxx);
                                     hud_assert_true(result);
                                 }

                                 // Constant
            {
                constexpr auto result = test(maxx);
                hud_assert_true(result);
            }
        }
    );
}

GTEST_TEST(hashset, contains_in_hashset_non_trivial_type)
{
    hud_test::for_each_value(std::make_integer_sequence<usize, 64>(), []<usize maxx>()
                             {
                                 const auto test_1 = [](usize max)
                                 {
                                     using KeyType = hud_test::non_bitwise_type;
                                     hud::hashset<KeyType> set;
                                     for (u32 index = 0; index < max; index++)
                                     {
                                         set.add(index);
                                     }

                                     bool is_find_ok = true;
                                     // Find elements that exists
                                     for (u32 index = 0; index < max; index++)
                                     {
                                         is_find_ok &= set.contains(index);
                                     }

                                     // Find elements that not exists
                                     for (u32 index = max; index < max * 2; index++)
                                     {
                                         is_find_ok &= !set.contains(index);
                                     }
                                     return is_find_ok;
                                 };

                                 const auto test_2 = [](usize max)
                                 {
                                     using KeyType = hud_test::non_bitwise_type;
                                     hud::hashset<KeyType> set;
                                     for (u32 index = 0; index < max; index++)
                                     {
                                         set.add(index);
                                     }

                                     bool is_find_ok = true;
                                     // Find elements that exists
                                     for (u32 index = 0; index < max; index++)
                                     {
                                         is_find_ok &= set.contains(KeyType(index));
                                     }

                                     // Find elements that not exists
                                     for (u32 index = max; index < max * 2; index++)
                                     {
                                         is_find_ok &= !set.contains(KeyType(index));
                                     }
                                     return is_find_ok;
                                 };

                                 // Non constant
                                 {
                                     const auto result = test_1(maxx);
                                     hud_assert_true(result);
                                     auto result_2 = test_2(maxx);
                                     hud_assert_true(result_2);
                                 }

                                 // Constant
            {
                constexpr auto result = test_1(maxx);
                hud_assert_true(result);
                constexpr auto result_2 = test_2(maxx);
                hud_assert_true(result_2);
            }
        }
    );
}