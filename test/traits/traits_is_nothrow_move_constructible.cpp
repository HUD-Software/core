#include <core/traits/is_nothrow_move_constructible.h>

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        b(b &&)
        {
        }
    };

    struct c
    {
        c(c &&) noexcept
        {
        }

        c(a &&) noexcept
        {
        }

        c(b &&)
        {
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_nothrow_move_constructible)
{
    hud_assert_true((hud::is_nothrow_move_constructible_v<i32>));
    hud_assert_true((hud::is_nothrow_move_constructible_v<hud_test::a>));
    hud_assert_false((hud::is_nothrow_move_constructible_v<hud_test::b>));
    hud_assert_true((hud::is_nothrow_move_constructible_v<hud_test::c>));
    hud_assert_true((hud::is_nothrow_move_constructible_v<hud_test::c, hud_test::a>));
    hud_assert_false((hud::is_nothrow_move_constructible_v<hud_test::c, hud_test::b>));
}