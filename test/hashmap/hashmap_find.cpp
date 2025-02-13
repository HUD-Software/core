#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, find_in_empty_hashmap)
{
    hud::hashmap<usize, usize> map;
    hud_assert_eq(map.find(1), map.end());
}

GTEST_TEST(hashmap, find_in_1_element_hashmap)
{
    hud::hashmap<usize, usize> map {
        {1, 11}
    };

    // Find elements that exists
    const auto it = map.find(1);
    hud_assert_ne(it, map.end());
    hud_assert_eq(it->key(), 1);
    hud_assert_eq(it->value(), 11);

    // Find elements that not exists
    hud_assert_eq(map.find(11), map.end());
}

GTEST_TEST(hashmap, find_in_more_than_1_element_hashmap)
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
            const auto it = map.find(index);
            hud_assert_ne(it, map.end());
            hud_assert_eq(it->key(), index);
            hud_assert_eq(it->value(), index * 10);
        }

        // Find elements that not exists
        for (u32 index = max; index < max * 2; index++)
        {
            const auto it = map.find(index);
            hud_assert_eq(it, map.end());
        }
    }
}