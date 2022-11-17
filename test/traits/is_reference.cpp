#include <core/traits/is_reference.h>

GTEST_TEST(traits, is_reference) {
    GTEST_ASSERT_FALSE((hud::is_reference_v<i32>));
    GTEST_ASSERT_FALSE((hud::is_reference_v<i32*>));
    GTEST_ASSERT_TRUE((hud::is_reference_v<i32&>));
    GTEST_ASSERT_TRUE((hud::is_reference_v<i32&&>));
}