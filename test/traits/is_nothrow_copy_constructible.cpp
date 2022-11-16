#include <core/traits/is_nothrow_copy_constructible.h>

namespace hud_test {
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

GTEST_TEST(traits, is_nothrow_copy_constructible) {
    ASSERT_TRUE((hud::is_nothrow_copy_constructible_v<i32>));
    ASSERT_TRUE((hud::is_nothrow_copy_constructible_v<hud_test::a>));
    ASSERT_FALSE((hud::is_nothrow_copy_constructible_v<hud_test::b>));
    ASSERT_TRUE((hud::is_nothrow_copy_constructible_v<hud_test::c>));
    ASSERT_TRUE((hud::is_nothrow_copy_constructible_v<hud_test::c, hud_test::b>));
    ASSERT_FALSE((hud::is_nothrow_copy_constructible_v<hud_test::c, hud_test::a>));
}
