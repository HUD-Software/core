#include <core/traits/add_rvalue_reference.h>

TEST(traits, add_rvalue_reference) {
    ASSERT_TRUE((hud::is_same_v<hud::add_rvalue_reference_t<i32>, i32&&>));
    ASSERT_TRUE((hud::is_same_v<hud::add_rvalue_reference_t<i32&>, i32&>));
    ASSERT_TRUE((hud::is_same_v<hud::add_rvalue_reference_t<i32&&>, i32&&>));
    ASSERT_TRUE((hud::is_same_v<hud::add_rvalue_reference_t<i32*>, i32*&&>));
}