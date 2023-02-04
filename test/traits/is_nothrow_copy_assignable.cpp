#include <core/traits/is_nothrow_copy_assignable.h>

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        b &operator=(const b &)
        {
            return *this;
        }
    };

    struct c
    {
        c &operator=(const c &) noexcept
        {
            return *this;
        }

        c &operator=(const b &) noexcept
        {
            return *this;
        }

        c &operator=(const a &)
        {
            return *this;
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_nothrow_copy_assignable)
{
    hud_assert_true((hud::is_nothrow_copy_assignable_v<i32>));
    hud_assert_true((hud::is_nothrow_copy_assignable_v<hud_test::a>));
    hud_assert_false((hud::is_nothrow_copy_assignable_v<hud_test::b>));
    hud_assert_true((hud::is_nothrow_copy_assignable_v<hud_test::c>));
    hud_assert_true((hud::is_nothrow_copy_assignable_v<hud_test::c, hud_test::b>));
    hud_assert_false((hud::is_nothrow_copy_assignable_v<hud_test::c, hud_test::a>));
    hud_assert_false((hud::is_nothrow_copy_assignable_v<hud_test::b, hud_test::c>));
}