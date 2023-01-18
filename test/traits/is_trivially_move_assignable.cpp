#include <core/traits/is_trivially_move_assignable.h>

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        b &operator=(a &&) noexcept
        {
            return *this;
        }
    };

    struct c
    {
        c &operator=(c &&) noexcept
        {
            return *this;
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_trivially_move_assignable)
{
    GTEST_ASSERT_TRUE((hud::is_trivially_move_assignable_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_trivially_move_assignable_v<i32 &>));
    GTEST_ASSERT_FALSE((hud::is_trivially_move_assignable_v<i32[]>));
    GTEST_ASSERT_FALSE((hud::is_trivially_move_assignable_v<i32[32]>));
    GTEST_ASSERT_TRUE((hud::is_trivially_move_assignable_v<i32 *>));
    GTEST_ASSERT_TRUE((hud::is_trivially_move_assignable_v<i32, f32>));

    GTEST_ASSERT_TRUE((hud::is_trivially_move_assignable_v<hud_test::a>));
    GTEST_ASSERT_TRUE((hud::is_trivially_move_assignable_v<hud_test::a, hud_test::a>));
    GTEST_ASSERT_TRUE((hud::is_trivially_move_assignable_v<hud_test::b>));
    GTEST_ASSERT_TRUE((hud::is_trivially_move_assignable_v<hud_test::b, hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_trivially_move_assignable_v<hud_test::c>));
}