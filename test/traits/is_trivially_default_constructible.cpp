#include <core/traits/is_trivially_default_constructible.h>

namespace {
    struct a { };
    struct b { b() {} };
    struct c { virtual void fn() {} };
}

TEST(traits, is_trivially_default_constructible) {
    ASSERT_TRUE((hud::is_trivially_default_constructible_v<i32>));
    ASSERT_FALSE((hud::is_trivially_default_constructible_v<i32&>));
    ASSERT_FALSE((hud::is_trivially_default_constructible_v<i32[]>));
    ASSERT_TRUE((hud::is_trivially_default_constructible_v<i32[32]>));
    ASSERT_TRUE((hud::is_trivially_default_constructible_v<i32*>));

    ASSERT_TRUE((hud::is_trivially_default_constructible_v<a>));
    ASSERT_FALSE((hud::is_trivially_default_constructible_v<b>));
    ASSERT_FALSE((hud::is_trivially_default_constructible_v<c>));
}