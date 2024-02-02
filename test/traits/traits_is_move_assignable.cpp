#include <core/traits/is_move_assignable.h>

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        b &operator=(b &&) = delete;
    };

    struct c
    {
        c &operator=(b &&) noexcept
        {
            return *this;
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_move_assignable)
{
    hud_assert_true((hud::is_move_assignable_v<i32>));
    hud_assert_true((hud::is_move_assignable_v<hud_test::a>));
    hud_assert_false((hud::is_move_assignable_v<hud_test::b>));
    hud_assert_true((hud::is_move_assignable_v<hud_test::c, hud_test::b>));
    hud_assert_false((hud::is_move_assignable_v<hud_test::b, hud_test::c>));
}