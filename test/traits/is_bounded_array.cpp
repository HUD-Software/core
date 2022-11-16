#include <core/traits/is_bounded_array.h>


GTEST_TEST(traits, is_bounded_array) {
    ASSERT_FALSE(hud::is_bounded_array_v<i32>);
    ASSERT_FALSE(hud::is_bounded_array_v<i32*>);
    ASSERT_FALSE(hud::is_bounded_array_v<i32&>);
    ASSERT_FALSE(hud::is_bounded_array_v<i32[][2]>);
    ASSERT_TRUE(hud::is_bounded_array_v<i32[2]>);
    ASSERT_TRUE(hud::is_bounded_array_v<i32[2][2]>);
}