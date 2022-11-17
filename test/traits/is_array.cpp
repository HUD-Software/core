#include <core/traits/is_array.h>

namespace hud_test {
    class a {};
}

GTEST_TEST(traits, is_array) {
    GTEST_ASSERT_FALSE(hud::is_array_v<hud_test::a>);
    GTEST_ASSERT_FALSE(hud::is_array_v<hud_test::a*>);
    GTEST_ASSERT_TRUE(hud::is_array_v<hud_test::a[]>);
    GTEST_ASSERT_TRUE(hud::is_array_v<hud_test::a[][2]>);
    GTEST_ASSERT_TRUE(hud::is_array_v<hud_test::a[2]>);
    GTEST_ASSERT_TRUE(hud::is_array_v<hud_test::a[2][2]>);
}