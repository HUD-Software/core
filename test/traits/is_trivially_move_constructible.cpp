#include <core/traits/is_trivially_move_constructible.h>

namespace hud_test
{
    struct a
    {
    };
    struct b
    {
        b(b &&) noexcept {}
    };
    struct c
    {
        virtual void fn() {}
    };
}

GTEST_TEST(traits, is_trivially_move_constructible)
{
    GTEST_ASSERT_TRUE((hud::is_trivially_move_constructible_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_trivially_move_constructible_v<i32 &>));
    GTEST_ASSERT_FALSE((hud::is_trivially_move_constructible_v<i32[]>));
    GTEST_ASSERT_FALSE((hud::is_trivially_move_constructible_v<i32[32]>));
    GTEST_ASSERT_TRUE((hud::is_trivially_move_constructible_v<i32 *>));
    GTEST_ASSERT_TRUE((hud::is_trivially_move_constructible_v<i32, f32>));

    GTEST_ASSERT_TRUE((hud::is_trivially_move_constructible_v<hud_test::a>));
    GTEST_ASSERT_TRUE((hud::is_trivially_move_constructible_v<hud_test::a, hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_trivially_move_constructible_v<hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_trivially_move_constructible_v<hud_test::c>));
    GTEST_ASSERT_FALSE((hud::is_trivially_move_constructible_v<hud_test::c, hud_test::c>));
}