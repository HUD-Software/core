#include <core/traits/is_unbounded_array.h>

TEST(traits, is_unbounded_array) {
    ASSERT_FALSE((hud::is_unbounded_array_v<i32>));
    ASSERT_FALSE((hud::is_unbounded_array_v<i32&>));
    ASSERT_TRUE((hud::is_unbounded_array_v<i32[]>));
    ASSERT_FALSE((hud::is_unbounded_array_v<i32[32]>));
    ASSERT_FALSE((hud::is_unbounded_array_v<i32*>));
}
