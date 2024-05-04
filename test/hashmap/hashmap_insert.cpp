#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, insert)
{
    hud::hashmap<const char *, i64> map;
    u64 hash = hud::hash_64("key", 3);
    auto res = map.insert("key", 1);
}