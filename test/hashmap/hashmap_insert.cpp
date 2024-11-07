#include <core/containers/hashmap.h>
#include <core/cstring.h>

GTEST_TEST(hashmap, insert)
{
    hud::hashmap<const char *, i64> map;
    constexpr const usize len = hud::cstring::length("key");

    u64 hash = hud::combine_64(hud::combine_64(0, hud::hash_64("key", len)), hud::hash_64((const char *)&len, sizeof(usize)));
    auto res = map.insert("key", 1);
}