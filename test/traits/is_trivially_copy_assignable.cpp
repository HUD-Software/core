#include <core/traits/is_trivially_copy_assignable.h>

namespace {
    struct a { };
    struct b : a {};
    struct c { c& operator= (const a&) { return *this; } };
    struct d {
        d& operator= (const d&) { return *this; }
        d& operator= (const a&) { return *this; }
    };
}

TEST(traits, is_trivially_copy_assignable) {
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<i32>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<i32&>));
    ASSERT_FALSE((hud::is_trivially_copy_assignable_v<i32[]>));
    ASSERT_FALSE((hud::is_trivially_copy_assignable_v<i32[32]>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<i32*>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<i32, f32>));

    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<a>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<a, a>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<b>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<b, b>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<c>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<c, c>));
    ASSERT_FALSE((hud::is_trivially_copy_assignable_v<d>));
    ASSERT_FALSE((hud::is_trivially_copy_assignable_v<d, a>));
}
