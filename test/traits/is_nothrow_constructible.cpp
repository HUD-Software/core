#include <core/traits/is_nothrow_constructible.h>

namespace hud_test {
    struct a { };
    struct b {
        b() {}
        b(const a&) noexcept {}
        b(a&&) noexcept {}
    };
}

GTEST_TEST(traits, is_nothrow_constructible) {
    ASSERT_TRUE((hud::is_nothrow_constructible_v<i32>));
    ASSERT_TRUE((hud::is_nothrow_constructible_v<hud_test::a>));
    ASSERT_FALSE((hud::is_nothrow_constructible_v<hud_test::b>));
    ASSERT_TRUE((hud::is_nothrow_constructible_v<hud_test::b, hud_test::a&>));
    ASSERT_TRUE((hud::is_nothrow_constructible_v<hud_test::b, hud_test::a&&>));
}