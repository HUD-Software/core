#include <core/containers/hashset.h>
#include <core/templates/integer_sequence.h>

GTEST_TEST(hashset, find_in_empty_hashset)
{
    hud::hashset<usize> set;
    hud_assert_eq(set.find(1), set.end());
}

GTEST_TEST(hashset, find_in_hashset_trivial_type)
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
                    const auto it = set.find(index);
                    if (it == set.end())
                    {
                        is_find_ok = false;
                    }
                    if (it->key() != index)
                    {
                        is_find_ok = false;
                    }
                }

                // Find elements that not exists
                for (u32 index = max; index < max * 2; index++)
                {
                    const auto it = set.find(index);
                    if (it != set.end())
                    {
                        is_find_ok = false;
                    }
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
            } });
}

GTEST_TEST(hashset, find_in_hashset_non_trivial_type)
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
                    const auto it = set.find(index);
                    if (it == set.end())
                    {
                        is_find_ok = false;
                    }
                    if (it->key() != index)
                    {
                        is_find_ok = false;
                    }
                }

                // Find elements that not exists
                for (u32 index = max; index < max * 2; index++)
                {
                    const auto it = set.find(index);
                    if (it != set.end())
                    {
                        is_find_ok = false;
                    }
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
                    const auto it = set.find(KeyType(index));
                    if (it == set.end())
                    {
                        is_find_ok = false;
                    }
                    if (it->key() != index)
                    {
                        is_find_ok = false;
                    }
                }

                // Find elements that not exists
                for (u32 index = max; index < max * 2; index++)
                {
                    const auto it = set.find(KeyType(index));
                    if (it != set.end())
                    {
                        is_find_ok = false;
                    }
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
            } });
}
