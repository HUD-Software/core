#include <core/traits/add_lvalue_reference.h>

GTEST_TEST(traits, add_lvvalue_reference)
{
    hud_assert_true((hud::is_same_v<hud::add_lvalue_reference_t<i32>, i32 &>));
    hud_assert_true((hud::is_same_v<hud::add_lvalue_reference_t<i32 &>, i32 &>));
    hud_assert_true((hud::is_same_v<hud::add_lvalue_reference_t<i32 &&>, i32 &>));
    hud_assert_true((hud::is_same_v<hud::add_lvalue_reference_t<i32 *>, i32 *&>));
}