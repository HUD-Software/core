#include <core/traits/is_const.h>

GTEST_TEST(traits, is_const) {
    ASSERT_FALSE(hud::is_const_v<int>);
    ASSERT_FALSE(hud::is_const_v<volatile int>);
    ASSERT_TRUE(hud::is_const_v<const int>);
    ASSERT_TRUE(hud::is_const_v<const volatile int>);
    ASSERT_FALSE(hud::is_const_v<const int*>);
    ASSERT_TRUE(hud::is_const_v<const int* const>);
    ASSERT_FALSE(hud::is_const_v<const int&>);
    ASSERT_FALSE(hud::is_const_v<int&>);
}