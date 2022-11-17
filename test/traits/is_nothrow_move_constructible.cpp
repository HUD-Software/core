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

GTEST_TEST(traits, is_nothrow_move_constructible) {
    GTEST_ASSERT_TRUE((hud::is_nothrow_move_constructible_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_move_constructible_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_move_constructible_v<hud_test::b>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_move_constructible_v<hud_test::c>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_move_constructible_v<hud_test::c, hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_move_constructible_v<hud_test::c, hud_test::b>));
}