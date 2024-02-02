#include <core/traits/is_default_constructible.h>

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        b()
        {
        }
    };

    struct c
    {
        c(int)
        {
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_default_constructible)
{
    hud_assert_true((hud::is_default_constructible_v<i32>));
    hud_assert_false((hud::is_default_constructible_v<i32 &>));
    hud_assert_true((hud::is_default_constructible_v<volatile i32>));
    hud_assert_true((hud::is_default_constructible_v<const i32>));
    hud_assert_true((hud::is_default_constructible_v<const volatile i32>));
    hud_assert_true((hud::is_default_constructible_v<hud_test::a>));
    hud_assert_true((hud::is_default_constructible_v<hud_test::b>));
    hud_assert_false((hud::is_default_constructible_v<hud_test::c>));
}