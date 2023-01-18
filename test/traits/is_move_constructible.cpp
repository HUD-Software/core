#include <core/traits/is_move_constructible.h>

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        b(b &&) = delete;
    };

    struct c
    {
        c(b &&) noexcept
        {
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_move_constructible)
{
    GTEST_ASSERT_TRUE((hud::is_move_constructible_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_move_constructible_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_move_constructible_v<hud_test::b>));
    GTEST_ASSERT_TRUE((hud::is_move_constructible_v<hud_test::c, hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_move_constructible_v<hud_test::b, hud_test::c>));
}