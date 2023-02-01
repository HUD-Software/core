#include <core/traits/is_lvalue_reference.h>

GTEST_TEST(traits, is_lvalue_reference)
{
    hud_assert_false(hud::is_lvalue_reference_v<i32>);
    hud_assert_true(hud::is_lvalue_reference_v<i32 &>);
    hud_assert_false(hud::is_lvalue_reference_v<i32 &&>);
}