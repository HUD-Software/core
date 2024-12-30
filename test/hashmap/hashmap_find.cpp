#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, find)
{
    hud::hashmap<usize, usize> map;
    map.add(1, 11);
    map.add(2, 22);
    map.add(3, 33);

    auto it = map.find(1);
    hud_assert_ne(it, map.end());
}