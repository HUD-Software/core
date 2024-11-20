#include <core/containers/hashmap.h>
#include <core/cstring.h>

GTEST_TEST(hashmap, insert)
{
    hud::hashmap<const char *, const char *> map;
    auto res = map.insert("key", "value");
    res;
}