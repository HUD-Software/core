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
    hud_assert_true((hud::is_trivially_destructible_v<i32>));
    hud_assert_true((hud::is_trivially_destructible_v<i32 &>));
    hud_assert_false((hud::is_trivially_destructible_v<i32[]>));
    hud_assert_true((hud::is_trivially_destructible_v<i32[32]>));
    hud_assert_true((hud::is_trivially_destructible_v<i32 *>));

    hud_assert_true((hud::is_trivially_destructible_v<hud_test::a>));
    hud_assert_false((hud::is_trivially_destructible_v<hud_test::b>));
}