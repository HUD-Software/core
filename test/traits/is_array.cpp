#include <core/traits/is_array.h>

namespace hud_test
{
    class a
    {
    };
} // namespace hud_test

GTEST_TEST(traits, is_array)
{
    hud_assert_false(hud::is_array_v<hud_test::a>);
    hud_assert_false(hud::is_array_v<hud_test::a *>);
    hud_assert_true(hud::is_array_v<hud_test::a[]>);
    hud_assert_true(hud::is_array_v<hud_test::a[][2]>);
    hud_assert_true(hud::is_array_v<hud_test::a[2]>);
    hud_assert_true(hud::is_array_v<hud_test::a[2][2]>);
}