#include <core/traits/is_nothrow_copy_constructible.h>

namespace {
    struct a { };
    struct b {
        b(const b&) {}
    };
    struct c {
        c(const c&) noexcept {}
        c(const b&) noexcept {}
        c(const a&) {}
    };
}

TEST(traits, is_nothrow_copy_constructible) {
    ASSERT_TRUE((hud::is_nothrow_copy_constructible_v<i32>));
    ASSERT_TRUE((hud::is_nothrow_copy_constructible_v<a>));
    ASSERT_FALSE((hud::is_nothrow_copy_constructible_v<b>));
    ASSERT_TRUE((hud::is_nothrow_copy_constructible_v<c>));
    ASSERT_TRUE((hud::is_nothrow_copy_constructible_v<c, b>));
    ASSERT_FALSE((hud::is_nothrow_copy_constructible_v<c, a>));
}
