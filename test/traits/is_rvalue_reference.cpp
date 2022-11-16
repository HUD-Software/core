#include <core/traits/is_rvalue_reference.h>

GTEST_TEST(traits, is_rvalue_reference) {
    ASSERT_FALSE((hud::is_rvalue_reference_v<i32>));
    ASSERT_FALSE((hud::is_rvalue_reference_v<i32&>));
    ASSERT_TRUE((hud::is_rvalue_reference_v<i32&&>));
}