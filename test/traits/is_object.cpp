#include <core/traits/is_object.h>

namespace {
    class a {};
}

TEST(traits, is_object) {
    ASSERT_TRUE((hud::is_object_v<i32>));
    ASSERT_TRUE((hud::is_object_v<a>));
    ASSERT_FALSE((hud::is_object_v<a&>));
    ASSERT_TRUE((hud::is_object_v<a*>));
    ASSERT_FALSE((hud::is_object_v<i32(i32)>));
    ASSERT_TRUE((hud::is_object_v<i32(*)(i32)>));
}