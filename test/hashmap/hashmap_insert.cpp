#include <core/containers/hashmap.h>
#include <core/cstring.h>

GTEST_TEST(hashmap, insert)
{
    hud::hashmap<i32, u64> map;
    u64 &res = map.insert_to_ref(1, 2);
    res = map.insert_to_ref(1, 2);
}