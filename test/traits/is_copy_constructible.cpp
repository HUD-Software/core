#include <core/traits/is_copy_constructible.h>

namespace hud_test {
    struct a { };
    struct b { b(b&&) noexcept {} };
    struct c { c(const c&) noexcept {} };
    struct d { d(const c&) noexcept {} };
}

GTEST_TEST(traits, is_copy_constructible) {
    ASSERT_TRUE((hud::is_copy_constructible_v<i32>));
    ASSERT_TRUE((hud::is_copy_constructible_v<i32&>));
    ASSERT_TRUE((hud::is_copy_constructible_v<volatile i32>));
    ASSERT_TRUE((hud::is_copy_constructible_v<const i32>));
    ASSERT_TRUE((hud::is_copy_constructible_v<const volatile i32>));
    ASSERT_TRUE((hud::is_copy_constructible_v<hud_test::a>));
    ASSERT_FALSE((hud::is_copy_constructible_v<hud_test::b>));
    ASSERT_TRUE((hud::is_copy_constructible_v<hud_test::c>));
    ASSERT_TRUE((hud::is_copy_constructible_v<hud_test::d, hud_test::c>));
    ASSERT_TRUE((hud::is_copy_constructible_v<hud_test::d, hud_test::d>));
    ASSERT_FALSE((hud::is_copy_constructible_v<hud_test::d, hud_test::b>));
}