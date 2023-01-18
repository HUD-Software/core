#include <core/traits/is_trivially_destructible.h>

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        ~b()
        {
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_trivially_destructible)
{
    GTEST_ASSERT_TRUE((hud::is_trivially_destructible_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_trivially_destructible_v<i32 &>));
    GTEST_ASSERT_FALSE((hud::is_trivially_destructible_v<i32[]>));
    GTEST_ASSERT_TRUE((hud::is_trivially_destructible_v<i32[32]>));
    GTEST_ASSERT_TRUE((hud::is_trivially_destructible_v<i32 *>));

    GTEST_ASSERT_TRUE((hud::is_trivially_destructible_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_trivially_destructible_v<hud_test::b>));
}