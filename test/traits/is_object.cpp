#include <core/traits/is_object.h>

namespace hud_test {
    class a {};
}

GTEST_TEST(traits, is_object) {
    GTEST_ASSERT_TRUE((hud::is_object_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_object_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_object_v<hud_test::a&>));
    GTEST_ASSERT_TRUE((hud::is_object_v<hud_test::a*>));
    GTEST_ASSERT_FALSE((hud::is_object_v<i32(i32)>));
    GTEST_ASSERT_TRUE((hud::is_object_v<i32(*)(i32)>));
}