#include <core/traits/is_nothrow_constructible.h>

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        b()
        {
        }

        b(const a &) noexcept
        {
        }

        b(a &&) noexcept
        {
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_nothrow_constructible)
{
    GTEST_ASSERT_TRUE((hud::is_nothrow_constructible_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_constructible_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_constructible_v<hud_test::b>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_constructible_v<hud_test::b, hud_test::a &>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_constructible_v<hud_test::b, hud_test::a &&>));
}