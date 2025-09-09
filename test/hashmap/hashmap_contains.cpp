#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, contains_in_empty_hashmap)
{
    hud::hashmap<usize, usize> map;
    hud_assert_false(map.contains(1));
}

GTEST_TEST(hashmap, contains_in_hashmap_trivial_type)
{

    hud_test::for_each_value(std::make_integer_sequence<usize, 128>(), []<usize maxx>()
                             {
            const auto test = [](usize max)
            {
                using KeyType = usize;
                using ValueType = usize;
                hud::hashmap<KeyType, ValueType> map;

                for (u32 index = 0; index < max; index++)
                {
                    map.add(index, index * index);
                }

                bool is_find_ok = true;
                // Find elements that exists
                for (u32 index = 0; index < max; index++)
                {
                    is_find_ok &= map.contains(index);
                }

                // Find elements that not exists
                for (u32 index = max; index < max * 2; index++)
                {
                    is_find_ok &= !map.contains(index);
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

GTEST_TEST(hashmap, contains_in_hashmap_non_trivial_type)
{
    hud_test::for_each_value(std::make_integer_sequence<usize, 128>(), []<usize maxx>()
                             {
        const auto test_1 = [](usize max)
        {
            using KeyType = hud_test::non_bitwise_type;
            using ValueType = hud_test::non_bitwise_type;
            hud::hashmap<KeyType, ValueType> map;
            for (u32 index = 0; index < max; index++)
            {
                map.add(index, index * index);
            }

            bool is_find_ok = true;
            // Find elements that exists
            for (u32 index = 0; index < max; index++)
            {
                is_find_ok &= map.contains(index);
            }

            // Find elements that not exists
            for (u32 index = max; index < max * 2; index++)
            {
                is_find_ok &= !map.contains(index);
            }
            return is_find_ok;
        };

        const auto test_2 = [](usize max)
        {
            using KeyType = hud_test::non_bitwise_type;
            using ValueType = hud_test::non_bitwise_type;
            hud::hashmap<KeyType, ValueType> map;
            for (u32 index = 0; index < max; index++)
            {
                map.add(index, index * index);
            }

            bool is_find_ok = true;
            // Find elements that exists
            for (u32 index = 0; index < max; index++)
            {
                is_find_ok &= map.contains(KeyType(index));
            }

            // Find elements that not exists
            for (u32 index = max; index < max * 2; index++)
            {
                is_find_ok &= !map.contains(KeyType(index));
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