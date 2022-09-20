#include <core/traits/is_trivially_copy_constructible.h>

namespace {
    struct a { };
    struct b { b(const b&) {} };
    struct c { virtual void fn() {} };
    struct d { d(a&&) {} };
}

TEST(traits, is_trivially_copy_constructible) {
    ASSERT_TRUE((hud::is_trivially_copy_constructible_v<i32>));
    ASSERT_TRUE((hud::is_trivially_copy_constructible_v<i32&>));
    ASSERT_FALSE((hud::is_trivially_copy_constructible_v<i32[]>));
    ASSERT_FALSE((hud::is_trivially_copy_constructible_v<i32[32]>));
    ASSERT_TRUE((hud::is_trivially_copy_constructible_v<i32*>));
    ASSERT_TRUE((hud::is_trivially_copy_constructible_v<i32, f32>));

    ASSERT_TRUE((hud::is_trivially_copy_constructible_v<a>));
    ASSERT_TRUE((hud::is_trivially_copy_constructible_v<a, a>));
    ASSERT_FALSE((hud::is_trivially_copy_constructible_v<b>));
    ASSERT_FALSE((hud::is_trivially_copy_constructible_v<c>));
    ASSERT_TRUE((hud::is_trivially_copy_constructible_v<d>));
    ASSERT_TRUE((hud::is_trivially_copy_constructible_v<d, d>));
    ASSERT_FALSE((hud::is_trivially_copy_constructible_v<d, a>));
}