#include <core/traits/is_bounded_array.h>

GTEST_TEST(traits, is_bounded_array)
{
    GTEST_ASSERT_FALSE(hud::is_bounded_array_v<i32>);
    GTEST_ASSERT_FALSE(hud::is_bounded_array_v<i32 *>);
    GTEST_ASSERT_FALSE(hud::is_bounded_array_v<i32 &>);
    GTEST_ASSERT_FALSE(hud::is_bounded_array_v<i32[][2]>);
    GTEST_ASSERT_TRUE(hud::is_bounded_array_v<i32[2]>);
    GTEST_ASSERT_TRUE(hud::is_bounded_array_v<i32[2][2]>);
}