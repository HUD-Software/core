#include <core/traits/is_bounded_array.h>

GTEST_TEST(traits, is_bounded_array)
{
    hud_assert_false(hud::is_bounded_array_v<i32>);
    hud_assert_false(hud::is_bounded_array_v<i32 *>);
    hud_assert_false(hud::is_bounded_array_v<i32 &>);
    hud_assert_false(hud::is_bounded_array_v<i32[][2]>);
    hud_assert_true(hud::is_bounded_array_v<i32[2]>);
    hud_assert_true(hud::is_bounded_array_v<i32[2][2]>);
}