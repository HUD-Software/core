#include <core/traits/is_trivially_assignable.h>

namespace
{
    struct a
    {
    };

    struct b : a
    {
    };

    struct c
    {
        c &operator=(const a &)
        {
            return *this;
        }
    };

    struct d
    {
        d &operator=(const d &)
        {
            return *this;
        }
    };
} // namespace

GTEST_TEST(traits, is_trivially_assignable)
{
    hud_assert_false((hud::is_trivially_assignable_v<i32, i32>));
    hud_assert_true((hud::is_trivially_assignable_v<i32 &, i32>));
    hud_assert_false((hud::is_trivially_assignable_v<i32[], i32[]>));
    hud_assert_false((hud::is_trivially_assignable_v<i32[32], i32[32]>));
    hud_assert_false((hud::is_trivially_assignable_v<i32 *, i32 *>));
    hud_assert_true((hud::is_trivially_assignable_v<a, a>));
    hud_assert_true((hud::is_trivially_assignable_v<a, b>));
    hud_assert_true((hud::is_trivially_assignable_v<a, b &>));
    hud_assert_true((hud::is_trivially_assignable_v<a, b &&>));
    hud_assert_false((hud::is_trivially_assignable_v<a, c>));
    hud_assert_false((hud::is_trivially_assignable_v<a, d>));
    hud_assert_false((hud::is_trivially_assignable_v<b, a>));
    hud_assert_true((hud::is_trivially_assignable_v<b, b>));
    hud_assert_false((hud::is_trivially_assignable_v<b, c>));
    hud_assert_false((hud::is_trivially_assignable_v<b, d>));
    hud_assert_false((hud::is_trivially_assignable_v<c, a>));
    hud_assert_false((hud::is_trivially_assignable_v<c, b>));
    hud_assert_true((hud::is_trivially_assignable_v<c, c &>));
    hud_assert_false((hud::is_trivially_assignable_v<c, d>));
    hud_assert_false((hud::is_trivially_assignable_v<d, a>));
    hud_assert_false((hud::is_trivially_assignable_v<d, b>));
    hud_assert_false((hud::is_trivially_assignable_v<d, c>));
    hud_assert_false((hud::is_trivially_assignable_v<d, d &>));
    hud_assert_false((hud::is_trivially_assignable_v<d, d &&>));
}