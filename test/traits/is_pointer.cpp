#include <core/traits/is_pointer.h>

TEST(traits, is_pointer) {
    ASSERT_FALSE((hud::is_pointer_v<i32>));
    ASSERT_TRUE((hud::is_pointer_v<i32*>));
    ASSERT_TRUE((hud::is_pointer_v<i32**>));
    ASSERT_TRUE((hud::is_pointer_v<i32(*)(i32)>));
}