#include <core/traits/is_nothrow_default_constructible.h>

namespace hud_test
{
    struct a
    {
        a() noexcept
        {
        }
    };

    struct b
    {
        b() noexcept(false)
        {
        }
    };

    struct c
    {
        c()
        {
            throw 20;
        }
    };

    struct d
    {
        d() noexcept
            : c()
        {
        }

        c c;
    };

    struct e
    {
        e()
            : c()
        {
        }

        c c;
    };
} // namespace hud_test

GTEST_TEST(traits, is_nothrow_default_constructible)
{
    hud_assert_true((hud::is_nothrow_default_constructible_v<hud_test::a>));
    hud_assert_false((hud::is_nothrow_default_constructible_v<hud_test::b>));
    hud_assert_false((hud::is_nothrow_default_constructible_v<hud_test::c>));
    hud_assert_true((hud::is_nothrow_default_constructible_v<hud_test::d>));
    hud_assert_false((hud::is_nothrow_default_constructible_v<hud_test::e>));
}