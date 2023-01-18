#include <core/traits/is_trivially_default_constructible.h>

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
        virtual void fn()
        {
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_trivially_default_constructible)
{
    GTEST_ASSERT_TRUE((hud::is_trivially_default_constructible_v<i32>));
    GTEST_ASSERT_FALSE((hud::is_trivially_default_constructible_v<i32 &>));
    GTEST_ASSERT_FALSE((hud::is_trivially_default_constructible_v<i32[]>));
    GTEST_ASSERT_TRUE((hud::is_trivially_default_constructible_v<i32[32]>));
    GTEST_ASSERT_TRUE((hud::is_trivially_default_constructible_v<i32 *>));

    GTEST_ASSERT_TRUE((hud::is_trivially_default_constructible_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_trivially_default_constructible_v<hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_trivially_default_constructible_v<hud_test::c>));
}