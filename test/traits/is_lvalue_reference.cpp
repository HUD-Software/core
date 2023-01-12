#include <core/traits/is_lvalue_reference.h>

GTEST_TEST(traits, is_lvalue_reference)
{
    GTEST_ASSERT_FALSE(hud::is_lvalue_reference_v<i32>);
    GTEST_ASSERT_TRUE(hud::is_lvalue_reference_v<i32 &>);
    GTEST_ASSERT_FALSE(hud::is_lvalue_reference_v<i32 &&>);
}