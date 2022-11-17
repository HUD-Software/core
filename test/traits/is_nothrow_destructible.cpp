#include <core/traits/is_nothrow_destructible.h>

namespace hud_test {
    struct a { };
    struct b { ~b() {} };
    struct c { ~c() noexcept(false) {} };
}

GTEST_TEST(traits, is_nothrow_destructible) {
    GTEST_ASSERT_TRUE((hud::is_nothrow_destructible_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_destructible_v<hud_test::a>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_destructible_v<hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_destructible_v<hud_test::c>));
}