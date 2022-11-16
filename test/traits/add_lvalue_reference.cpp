#include <core/traits/add_lvalue_reference.h>

GTEST_TEST(traits, add_lvvalue_reference) {
    ASSERT_TRUE((hud::is_same_v<hud::add_lvalue_reference_t<i32>, i32&>));
    ASSERT_TRUE((hud::is_same_v<hud::add_lvalue_reference_t<i32&>, i32&>));
    ASSERT_TRUE((hud::is_same_v<hud::add_lvalue_reference_t<i32&&>, i32&>));
    ASSERT_TRUE((hud::is_same_v<hud::add_lvalue_reference_t<i32*>, i32*&>));
}