#include <core/traits/is_pointer.h>

GTEST_TEST(traits, is_pointer) {
    GTEST_ASSERT_FALSE((hud::is_pointer_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_pointer_v<i32*>));
    GTEST_ASSERT_TRUE((hud::is_pointer_v<i32**>));
    GTEST_ASSERT_TRUE((hud::is_pointer_v<i32(*)(i32)>));
}