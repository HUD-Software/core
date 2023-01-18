#include <core/traits/is_nothrow_move_assignable.h>

namespace
{
    struct a
    {
    };

    struct b
    {
        b &operator=(b &&)
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

        c &operator=(a &&) noexcept
        {
            return *this;
        }

        c &operator=(b &&)
        {
            return *this;
        }
    };
} // namespace

GTEST_TEST(traits, is_nothrow_move_assignable)
{
    GTEST_ASSERT_TRUE((hud::is_nothrow_move_assignable_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_move_assignable_v<a>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_move_assignable_v<b>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_move_assignable_v<c>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_move_assignable_v<c, a>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_move_assignable_v<c, b>));
}
