#include <core/traits/is_default_constructible.h>

namespace {
    struct a { };
    struct b { b() {} };
    struct c { c(int) {} };
}

TEST(traits, is_default_constructible) {
    ASSERT_TRUE((hud::is_default_constructible_v<i32>));
    ASSERT_FALSE((hud::is_default_constructible_v<i32&>));
    ASSERT_TRUE((hud::is_default_constructible_v<volatile i32>));
    ASSERT_TRUE((hud::is_default_constructible_v<const i32>));
    ASSERT_TRUE((hud::is_default_constructible_v<const volatile i32>));
    ASSERT_TRUE((hud::is_default_constructible_v<a>));
    ASSERT_TRUE((hud::is_default_constructible_v<b>));
    ASSERT_FALSE((hud::is_default_constructible_v<c>));
}