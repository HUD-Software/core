#include <core/templates/declval.h>

GTEST_TEST(templates, declval)
{
    i32 integer = 22;
    decltype(hud::declval<i32>()) n = hud::move(integer);
    GTEST_ASSERT_EQ(n, integer);
}