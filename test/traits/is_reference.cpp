#include <core/traits/is_reference.h>

TEST(traits, is_reference) {
    ASSERT_FALSE((hud::is_reference_v<i32>));
    ASSERT_FALSE((hud::is_reference_v<i32*>));
    ASSERT_TRUE((hud::is_reference_v<i32&>));
    ASSERT_TRUE((hud::is_reference_v<i32&&>));
}