#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, insert)
{
    hud::hashmap<i32, u64> map;
    u64 &res = map.insert_to_ref(1, 2);
    res = map.insert_to_ref(1, 3);
    res = map.insert_to_ref(2, 3);
}