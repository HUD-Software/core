#include <core/traits/is_rvalue_reference.h>

TEST(traits, is_rvalue_reference) {
    ASSERT_FALSE((hud::is_rvalue_reference_v<i32>));
    ASSERT_FALSE((hud::is_rvalue_reference_v<i32&>));
    ASSERT_TRUE((hud::is_rvalue_reference_v<i32&&>));
}