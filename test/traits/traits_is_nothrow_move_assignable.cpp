#include <core/traits/is_nothrow_move_assignable.h>

namespace
{
    struct a
    {
    };

    struct b
    {
        b &operator=(b &&)
        {
            return *this;
        }
    };

    struct c
    {
        c &operator=(c &&) noexcept
        {
            return *this;
        }

        c &operator=(a &&) noexcept
        {
            return *this;
        }

        c &operator=(b &&)
        {
            return *this;
        }
    };
} // namespace

GTEST_TEST(traits, is_nothrow_move_assignable)
{
    hud_assert_true((hud::is_nothrow_move_assignable_v<i32>));
    hud_assert_true((hud::is_nothrow_move_assignable_v<a>));
    hud_assert_false((hud::is_nothrow_move_assignable_v<b>));
    hud_assert_true((hud::is_nothrow_move_assignable_v<c>));
    hud_assert_true((hud::is_nothrow_move_assignable_v<c, a>));
    hud_assert_false((hud::is_nothrow_move_assignable_v<c, b>));
}
