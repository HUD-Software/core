#include <core/traits/is_trivially_destructible.h>

namespace hud_test {
    struct a { };
    struct b { ~b() {} };
}

GTEST_TEST(traits, is_trivially_destructible) {
    ASSERT_TRUE((hud::is_trivially_destructible_v<i32>));
    ASSERT_TRUE((hud::is_trivially_destructible_v<i32&>));
    ASSERT_FALSE((hud::is_trivially_destructible_v<i32[]>));
    ASSERT_TRUE((hud::is_trivially_destructible_v<i32[32]>));
    ASSERT_TRUE((hud::is_trivially_destructible_v<i32*>));

    ASSERT_TRUE((hud::is_trivially_destructible_v<hud_test::a>));
    ASSERT_FALSE((hud::is_trivially_destructible_v<hud_test::b>));
}