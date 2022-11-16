#include <core/traits/is_object.h>

namespace hud_test {
    class a {};
}

GTEST_TEST(traits, is_object) {
    ASSERT_TRUE((hud::is_object_v<i32>));
    ASSERT_TRUE((hud::is_object_v<hud_test::a>));
    ASSERT_FALSE((hud::is_object_v<hud_test::a&>));
    ASSERT_TRUE((hud::is_object_v<hud_test::a*>));
    ASSERT_FALSE((hud::is_object_v<i32(i32)>));
    ASSERT_TRUE((hud::is_object_v<i32(*)(i32)>));
}