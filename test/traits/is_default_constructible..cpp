#include <core/traits/is_default_constructible.h>

namespace hud_test {
    struct a { };
    struct b { b() {} };
    struct c { c(int) {} };
}

GTEST_TEST(traits, is_default_constructible) {
    ASSERT_TRUE((hud::is_default_constructible_v<i32>));
    ASSERT_FALSE((hud::is_default_constructible_v<i32&>));
    ASSERT_TRUE((hud::is_default_constructible_v<volatile i32>));
    ASSERT_TRUE((hud::is_default_constructible_v<const i32>));
    ASSERT_TRUE((hud::is_default_constructible_v<const volatile i32>));
    ASSERT_TRUE((hud::is_default_constructible_v<hud_test::a>));
    ASSERT_TRUE((hud::is_default_constructible_v<hud_test::b>));
    ASSERT_FALSE((hud::is_default_constructible_v<hud_test::c>));
}