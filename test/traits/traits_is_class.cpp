#include <core/traits/is_class.h>

GTEST_TEST(traits, is_class)
{
    hud_assert_false(hud::is_class_v<i32>);
    hud_assert_false(hud::is_class_v<i32 *>);
    hud_assert_false(hud::is_class_v<i32 &>);
    hud_assert_true(hud::is_class_v<class a>);
    hud_assert_true(hud::is_class_v<struct S>);
}