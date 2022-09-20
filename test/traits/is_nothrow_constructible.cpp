#include <core/traits/is_nothrow_constructible.h>

namespace {
    struct a { };
    struct b {
        b() {}
        b(const a&) noexcept {}
        b(a&&) noexcept {}
    };
}

TEST(traits, is_nothrow_constructible) {
    ASSERT_TRUE((hud::is_nothrow_constructible_v<i32>));
    ASSERT_TRUE((hud::is_nothrow_constructible_v<a>));
    ASSERT_FALSE((hud::is_nothrow_constructible_v<b>));
    ASSERT_TRUE((hud::is_nothrow_constructible_v<b, a&>));
    ASSERT_TRUE((hud::is_nothrow_constructible_v<b, a&&>));
}