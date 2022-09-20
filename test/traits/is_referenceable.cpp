#include <core/traits/is_referenceable.h>

TEST(traits, is_referenceable) {
    ASSERT_TRUE((hud::is_referenceable_v<i32>));
    ASSERT_TRUE((hud::is_referenceable_v<i32&>));
    ASSERT_TRUE((hud::is_referenceable_v<i32&&>));
    ASSERT_TRUE((hud::is_referenceable_v<i32*>));
    ASSERT_TRUE((hud::is_referenceable_v<i32(*)(i32)>));
    ASSERT_FALSE((hud::is_referenceable_v<int()const >));
}