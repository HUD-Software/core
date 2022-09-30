#include <core/traits/is_nothrow_move_constructible.h>

namespace hud_test {
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
    ASSERT_TRUE((hud::is_nothrow_move_constructible_v<hud_test::a>));
    ASSERT_FALSE((hud::is_nothrow_move_constructible_v<hud_test::b>));
    ASSERT_TRUE((hud::is_nothrow_move_constructible_v<hud_test::c>));
    ASSERT_TRUE((hud::is_nothrow_move_constructible_v<hud_test::c, hud_test::a>));
    ASSERT_FALSE((hud::is_nothrow_move_constructible_v<hud_test::c, hud_test::b>));
}