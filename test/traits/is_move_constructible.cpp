#include <core/traits/is_move_constructible.h>

namespace {
    struct a { };
    struct b { b(b&&) = delete; };
    struct c { c(b&&) noexcept { } };
}

TEST(traits, is_move_constructible) {
    ASSERT_TRUE((hud::is_move_constructible_v<i32>));
    ASSERT_TRUE((hud::is_move_constructible_v<a>));
    ASSERT_FALSE((hud::is_move_constructible_v<b>));
    ASSERT_TRUE((hud::is_move_constructible_v<c, b>));
    ASSERT_FALSE((hud::is_move_constructible_v<b, c>));
}