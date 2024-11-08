#include <core/containers/hashmap.h>
#include <core/cstring.h>

GTEST_TEST(hashmap, insert)
{
    hud::hashmap<const char *, i64> map;
    auto res = map.insert("key", 1);
}