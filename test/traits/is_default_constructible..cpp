#include <core/traits/is_default_constructible.h>

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
    };

    struct c
    {
        c(int)
        {
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_default_constructible)
{
    GTEST_ASSERT_TRUE((hud::is_default_constructible_v<i32>));
    GTEST_ASSERT_FALSE((hud::is_default_constructible_v<i32 &>));
    GTEST_ASSERT_TRUE((hud::is_default_constructible_v<volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_default_constructible_v<const i32>));
    GTEST_ASSERT_TRUE((hud::is_default_constructible_v<const volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_default_constructible_v<hud_test::a>));
    GTEST_ASSERT_TRUE((hud::is_default_constructible_v<hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_default_constructible_v<hud_test::c>));
}