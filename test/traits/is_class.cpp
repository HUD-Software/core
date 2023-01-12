#include <core/traits/is_class.h>

GTEST_TEST(traits, is_class)
{
    GTEST_ASSERT_FALSE(hud::is_class_v<i32>);
    GTEST_ASSERT_FALSE(hud::is_class_v<i32 *>);
    GTEST_ASSERT_FALSE(hud::is_class_v<i32 &>);
    GTEST_ASSERT_TRUE(hud::is_class_v<class a>);
    GTEST_ASSERT_TRUE(hud::is_class_v<struct S>);
}