#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, default_constructor_should_allocate_no_memory)
{
    hud::hashmap<i32, i64> map;
}