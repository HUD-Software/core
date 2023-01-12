#include <core/traits/is_nothrow_assignable.h>

namespace hud_test
{
    struct a
    {
    };
    struct b
    {
        b &operator=(const a &) noexcept { return *this; }
        b &operator=(const b &) { return *this; }
        b &operator=(a &&) noexcept { return *this; }
        b &operator=(b &&) { return *this; }
    };
}

GTEST_TEST(traits, is_nothrow_assignable)
{
    GTEST_ASSERT_TRUE((hud::is_nothrow_assignable_v<hud_test::a, hud_test::a>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_assignable_v<hud_test::b, hud_test::a &>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_assignable_v<hud_test::b, hud_test::b &>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_assignable_v<hud_test::b, hud_test::a &&>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_assignable_v<hud_test::b, hud_test::b &&>));
}