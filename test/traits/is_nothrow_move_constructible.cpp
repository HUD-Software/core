#include <core/traits/is_nothrow_move_constructible.h>

namespace {
    struct a { };
    struct b {
        b(b&&) {}
    };
    struct c {
        c(c&&) noexcept {}
        c(a&&) noexcept {}
        c(b&&) {}
    };
}

TEST(traits, is_nothrow_move_constructible) {
    ASSERT_TRUE((hud::is_nothrow_move_constructible_v<i32>));
    ASSERT_TRUE((hud::is_nothrow_move_constructible_v<a>));
    ASSERT_FALSE((hud::is_nothrow_move_constructible_v<b>));
    ASSERT_TRUE((hud::is_nothrow_move_constructible_v<c>));
    ASSERT_TRUE((hud::is_nothrow_move_constructible_v<c, a>));
    ASSERT_FALSE((hud::is_nothrow_move_constructible_v<c, b>));
}