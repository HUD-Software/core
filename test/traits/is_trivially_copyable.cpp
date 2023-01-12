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
        b(const b &x) : i(x.i), j(x.j){};
    };
    struct c
    {
        int i, j;
        c(const a &x) : i(x.i), j(1){};
    };
}

GTEST_TEST(traits, is_trivially_copyable)
{
    GTEST_ASSERT_TRUE((hud::is_trivially_copyable_v<i32>));
    GTEST_ASSERT_FALSE((hud::is_trivially_copyable_v<i32 &>));
    GTEST_ASSERT_TRUE((hud::is_trivially_copyable_v<i32[]>));
    GTEST_ASSERT_TRUE((hud::is_trivially_copyable_v<i32[32]>));
    GTEST_ASSERT_TRUE((hud::is_trivially_copyable_v<i32 *>));

    GTEST_ASSERT_TRUE((hud::is_trivially_copyable_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_trivially_copyable_v<hud_test::b>));
    GTEST_ASSERT_TRUE((hud::is_trivially_copyable_v<hud_test::c>));
}