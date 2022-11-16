#include <core/traits/is_array.h>

namespace hud_test {
    class a {};
}

GTEST_TEST(traits, is_array) {
    ASSERT_FALSE(hud::is_array_v<hud_test::a>);
    ASSERT_FALSE(hud::is_array_v<hud_test::a*>);
    ASSERT_TRUE(hud::is_array_v<hud_test::a[]>);
    ASSERT_TRUE(hud::is_array_v<hud_test::a[][2]>);
    ASSERT_TRUE(hud::is_array_v<hud_test::a[2]>);
    ASSERT_TRUE(hud::is_array_v<hud_test::a[2][2]>);
}