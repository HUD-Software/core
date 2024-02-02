#include <core/traits/is_copy_constructible.h>

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        b(b &&) noexcept
        {
        }
    };

    struct c
    {
        c(const c &) noexcept
        {
        }
    };

    struct d
    {
        d(const c &) noexcept
        {
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_copy_constructible)
{
    hud_assert_true((hud::is_copy_constructible_v<i32>));
    hud_assert_true((hud::is_copy_constructible_v<i32 &>));
    hud_assert_true((hud::is_copy_constructible_v<volatile i32>));
    hud_assert_true((hud::is_copy_constructible_v<const i32>));
    hud_assert_true((hud::is_copy_constructible_v<const volatile i32>));
    hud_assert_true((hud::is_copy_constructible_v<hud_test::a>));
    hud_assert_false((hud::is_copy_constructible_v<hud_test::b>));
    hud_assert_true((hud::is_copy_constructible_v<hud_test::c>));
    hud_assert_true((hud::is_copy_constructible_v<hud_test::d, hud_test::c>));
    hud_assert_true((hud::is_copy_constructible_v<hud_test::d, hud_test::d>));
    hud_assert_false((hud::is_copy_constructible_v<hud_test::d, hud_test::b>));
}