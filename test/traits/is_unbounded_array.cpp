#include <core/traits/is_unbounded_array.h>

GTEST_TEST(traits, is_unbounded_array)
{
    GTEST_ASSERT_FALSE((hud::is_unbounded_array_v<i32>));
    GTEST_ASSERT_FALSE((hud::is_unbounded_array_v<i32 &>));
    GTEST_ASSERT_TRUE((hud::is_unbounded_array_v<i32[]>));
    GTEST_ASSERT_FALSE((hud::is_unbounded_array_v<i32[32]>));
    GTEST_ASSERT_FALSE((hud::is_unbounded_array_v<i32 *>));
}
