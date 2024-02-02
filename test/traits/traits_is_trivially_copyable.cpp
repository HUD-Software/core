#include <core/traits/is_trivially_copyable.h>

namespace hud_test
{
    struct a
    {
        int i;
    };

    struct b
    {
        int i, j;
        b(const b &x)
            : i(x.i)
            , j(x.j) {};
    };

    struct c
    {
        int i, j;
        c(const a &x)
            : i(x.i)
            , j(1) {};
    };
} // namespace hud_test

GTEST_TEST(traits, is_trivially_copyable)
{
    hud_assert_true((hud::is_trivially_copyable_v<i32>));
    hud_assert_false((hud::is_trivially_copyable_v<i32 &>));
    hud_assert_true((hud::is_trivially_copyable_v<i32[]>));
    hud_assert_true((hud::is_trivially_copyable_v<i32[32]>));
    hud_assert_true((hud::is_trivially_copyable_v<i32 *>));

    hud_assert_true((hud::is_trivially_copyable_v<hud_test::a>));
    hud_assert_false((hud::is_trivially_copyable_v<hud_test::b>));
    hud_assert_true((hud::is_trivially_copyable_v<hud_test::c>));
}