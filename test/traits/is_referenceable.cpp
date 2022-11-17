#include <core/traits/is_referenceable.h>

GTEST_TEST(traits, is_referenceable) {
    GTEST_ASSERT_TRUE((hud::is_referenceable_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_referenceable_v<i32&>));
    GTEST_ASSERT_TRUE((hud::is_referenceable_v<i32&&>));
    GTEST_ASSERT_TRUE((hud::is_referenceable_v<i32*>));
    GTEST_ASSERT_TRUE((hud::is_referenceable_v<i32(*)(i32)>));
    GTEST_ASSERT_FALSE((hud::is_referenceable_v<int()const >));
}