#include <core/traits/is_trivially_move_assignable.h>

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        b &operator=(a &&) noexcept
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
    };
} // namespace hud_test

GTEST_TEST(traits, is_trivially_move_assignable)
{
    hud_assert_true((hud::is_trivially_move_assignable_v<i32>));
    hud_assert_true((hud::is_trivially_move_assignable_v<i32 &>));
    hud_assert_false((hud::is_trivially_move_assignable_v<i32[]>));
    hud_assert_false((hud::is_trivially_move_assignable_v<i32[32]>));
    hud_assert_true((hud::is_trivially_move_assignable_v<i32 *>));
    hud_assert_true((hud::is_trivially_move_assignable_v<i32, f32>));

    hud_assert_true((hud::is_trivially_move_assignable_v<hud_test::a>));
    hud_assert_true((hud::is_trivially_move_assignable_v<hud_test::a, hud_test::a>));
    hud_assert_true((hud::is_trivially_move_assignable_v<hud_test::b>));
    hud_assert_true((hud::is_trivially_move_assignable_v<hud_test::b, hud_test::b>));
    hud_assert_false((hud::is_trivially_move_assignable_v<hud_test::c>));
}