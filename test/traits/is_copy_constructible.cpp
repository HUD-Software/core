#include <core/traits/is_copy_constructible.h>

namespace {
    struct a { };
    struct b { b(b&&) noexcept {} };
    struct c { c(const c&) noexcept {} };
    struct d { d(const c&) noexcept {} };
}

TEST(traits, is_copy_constructible) {
    ASSERT_TRUE((hud::is_copy_constructible_v<i32>));
    ASSERT_TRUE((hud::is_copy_constructible_v<i32&>));
    ASSERT_TRUE((hud::is_copy_constructible_v<volatile i32>));
    ASSERT_TRUE((hud::is_copy_constructible_v<const i32>));
    ASSERT_TRUE((hud::is_copy_constructible_v<const volatile i32>));
    ASSERT_TRUE((hud::is_copy_constructible_v<a>));
    ASSERT_FALSE((hud::is_copy_constructible_v<b>));
    ASSERT_TRUE((hud::is_copy_constructible_v<c>));
    ASSERT_TRUE((hud::is_copy_constructible_v<d, c>));
    ASSERT_TRUE((hud::is_copy_constructible_v<d, d>));
    ASSERT_FALSE((hud::is_copy_constructible_v<d, b>));
}