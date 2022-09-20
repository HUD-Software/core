#include <core/traits/is_trivially_assignable.h>

namespace{
    struct a { };
    struct b : a {};
    struct c { c& operator= (const a&) { return *this; } };
    struct d { d& operator= (const d&) { return *this; } };
}

TEST(traits, is_trivially_assignable) {
    ASSERT_FALSE((hud::is_trivially_assignable_v<i32, i32>));
    ASSERT_TRUE((hud::is_trivially_assignable_v<i32&, i32>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<i32[], i32[]>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<i32[32], i32[32]>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<i32*, i32*>));
    ASSERT_TRUE((hud::is_trivially_assignable_v<a, a>));
    ASSERT_TRUE((hud::is_trivially_assignable_v<a, b>));
    ASSERT_TRUE((hud::is_trivially_assignable_v<a, b&>));
    ASSERT_TRUE((hud::is_trivially_assignable_v<a, b&&>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<a, c>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<a, d>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<b, a>));
    ASSERT_TRUE((hud::is_trivially_assignable_v<b, b>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<b, c>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<b, d>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<c, a>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<c, b>));
    ASSERT_TRUE((hud::is_trivially_assignable_v<c, c&>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<c, d>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<d, a>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<d, b>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<d, c>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<d, d&>));
    ASSERT_FALSE((hud::is_trivially_assignable_v<d, d&&>));
}