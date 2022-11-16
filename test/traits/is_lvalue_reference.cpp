#include <core/traits/is_lvalue_reference.h>


GTEST_TEST(traits, is_lvalue_reference) {
    ASSERT_FALSE(hud::is_lvalue_reference_v<i32>);
    ASSERT_TRUE(hud::is_lvalue_reference_v<i32&>);
    ASSERT_FALSE(hud::is_lvalue_reference_v<i32&&>);
}