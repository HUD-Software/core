#include <core/traits/is_const.h>

GTEST_TEST(traits, is_const)
{
    hud_assert_false(hud::is_const_v<int>);
    hud_assert_false(hud::is_const_v<volatile int>);
    hud_assert_true(hud::is_const_v<const int>);
    hud_assert_true(hud::is_const_v<const volatile int>);
    hud_assert_false(hud::is_const_v<const int *>);
    hud_assert_true(hud::is_const_v<const int *const>);
    hud_assert_false(hud::is_const_v<const int &>);
    hud_assert_false(hud::is_const_v<int &>);
}