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
    GTEST_ASSERT_TRUE((hud::is_nothrow_default_constructible_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_default_constructible_v<hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_default_constructible_v<hud_test::c>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_default_constructible_v<hud_test::d>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_default_constructible_v<hud_test::e>));
}