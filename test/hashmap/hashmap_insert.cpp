#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, insert)
{
    hud::hashmap<i32, u64> map;
    auto it = map.begin();
    u64 &res = map.insert_to_ref(1, 2);
    it = map.begin();
    res = map.insert_to_ref(1, 3);
    res = map.insert_to_ref(2, 3);
    // map.begin();
    it = map.find(2);
}