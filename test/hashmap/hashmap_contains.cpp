#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, contains_in_empty_hashmap)
{
    hud::hashmap<usize, usize> map;
    hud_assert_false(map.contains(1));
}

GTEST_TEST(hashmap, contains_in_1_element_hashmap)
{
    hud::hashmap<usize, usize> map {
        {1, 11}
    };

    // Find elements that exists
    hud_assert_true(map.contains(1));

    // Find elements that not exists
    hud_assert_false(map.contains(11));
}

GTEST_TEST(hashmap, contains_in_more_than_1_element_hashmap)
{

    for (u32 max = 0; max < 1000; max++)
    {
        hud::hashmap<usize, usize> map;

        for (u32 index = 0; index < max; index++)
        {
            map.add(index, index * 10);
        }

        // Find elements that exists
        for (u32 index = 0; index < max; index++)
        {
            hud_assert_true(map.contains(index));
        }

        // Find elements that not exists
        for (u32 index = max; index < max * 2; index++)
        {
            hud_assert_false(map.contains(index));
        }
    }
}