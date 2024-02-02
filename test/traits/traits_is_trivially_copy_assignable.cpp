#include <core/traits/is_trivially_copy_assignable.h>

namespace hud_test
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

        d &operator=(const a &)
        {
            return *this;
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_trivially_copy_assignable)
{
    hud_assert_true((hud::is_trivially_copy_assignable_v<i32>));
    hud_assert_true((hud::is_trivially_copy_assignable_v<i32 &>));
    hud_assert_false((hud::is_trivially_copy_assignable_v<i32[]>));
    hud_assert_false((hud::is_trivially_copy_assignable_v<i32[32]>));
    hud_assert_true((hud::is_trivially_copy_assignable_v<i32 *>));
    hud_assert_true((hud::is_trivially_copy_assignable_v<i32, f32>));

    hud_assert_true((hud::is_trivially_copy_assignable_v<hud_test::a>));
    hud_assert_true((hud::is_trivially_copy_assignable_v<hud_test::a, hud_test::a>));
    hud_assert_true((hud::is_trivially_copy_assignable_v<hud_test::b>));
    hud_assert_true((hud::is_trivially_copy_assignable_v<hud_test::b, hud_test::b>));
    hud_assert_true((hud::is_trivially_copy_assignable_v<hud_test::c>));
    hud_assert_true((hud::is_trivially_copy_assignable_v<hud_test::c, hud_test::c>));
    hud_assert_false((hud::is_trivially_copy_assignable_v<hud_test::d>));
    hud_assert_false((hud::is_trivially_copy_assignable_v<hud_test::d, hud_test::a>));
}
