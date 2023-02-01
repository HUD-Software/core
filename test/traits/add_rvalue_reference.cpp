#include <core/traits/add_rvalue_reference.h>

GTEST_TEST(traits, add_rvalue_reference)
{
    hud_assert_true((hud::is_same_v<hud::add_rvalue_reference_t<i32>, i32 &&>));
    hud_assert_true((hud::is_same_v<hud::add_rvalue_reference_t<i32 &>, i32 &>));
    hud_assert_true((hud::is_same_v<hud::add_rvalue_reference_t<i32 &&>, i32 &&>));
    hud_assert_true((hud::is_same_v<hud::add_rvalue_reference_t<i32 *>, i32 *&&>));
}