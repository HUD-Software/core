#include <core/traits/is_trivially_destructible.h>

namespace {
    struct a { };
    struct b { ~b() {} };
}

TEST(traits, is_trivially_destructible) {
    ASSERT_TRUE((hud::is_trivially_destructible_v<i32>));
    ASSERT_TRUE((hud::is_trivially_destructible_v<i32&>));
    ASSERT_FALSE((hud::is_trivially_destructible_v<i32[]>));
    ASSERT_TRUE((hud::is_trivially_destructible_v<i32[32]>));
    ASSERT_TRUE((hud::is_trivially_destructible_v<i32*>));

    ASSERT_TRUE((hud::is_trivially_destructible_v<a>));
    ASSERT_FALSE((hud::is_trivially_destructible_v<b>));
}