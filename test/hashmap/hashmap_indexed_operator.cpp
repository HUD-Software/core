#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, indexed_operator_trivial_same_type)
{
    const auto test = []()
    {
        hud::hashmap<usize, usize> map;
        map[1] = 2;
        hud::hashmap<const char *, const char *> map2;
        map2["key"] = "value";

        return std::tuple {
            map.find(1)->value() = 2,
            map2.find("key")->value() = "value",
        };
    };
}