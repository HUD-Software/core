#include <core/traits/is_nothrow_destructible.h>

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

    struct c
    {
        ~c() noexcept(false)
        {
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_nothrow_destructible)
{
    hud_assert_true((hud::is_nothrow_destructible_v<i32>));
    hud_assert_true((hud::is_nothrow_destructible_v<hud_test::a>));
    hud_assert_true((hud::is_nothrow_destructible_v<hud_test::b>));
    hud_assert_false((hud::is_nothrow_destructible_v<hud_test::c>));
}