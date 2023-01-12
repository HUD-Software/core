#include <core/traits/is_const.h>

GTEST_TEST(traits, is_const)
{
    GTEST_ASSERT_FALSE(hud::is_const_v<int>);
    GTEST_ASSERT_FALSE(hud::is_const_v<volatile int>);
    GTEST_ASSERT_TRUE(hud::is_const_v<const int>);
    GTEST_ASSERT_TRUE(hud::is_const_v<const volatile int>);
    GTEST_ASSERT_FALSE(hud::is_const_v<const int *>);
    GTEST_ASSERT_TRUE(hud::is_const_v<const int *const>);
    GTEST_ASSERT_FALSE(hud::is_const_v<const int &>);
    GTEST_ASSERT_FALSE(hud::is_const_v<int &>);
}