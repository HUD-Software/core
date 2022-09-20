#include <core/traits/is_trivially_move_constructible.h>

namespace {
    struct a { };
    struct b { b(b&&) noexcept {} };
    struct c { virtual void fn() {} };
}

TEST(traits, is_trivially_move_constructible) {
    ASSERT_TRUE((hud::is_trivially_move_constructible_v<i32>));
    ASSERT_TRUE((hud::is_trivially_move_constructible_v<i32&>));
    ASSERT_FALSE((hud::is_trivially_move_constructible_v<i32[]>));
    ASSERT_FALSE((hud::is_trivially_move_constructible_v<i32[32]>));
    ASSERT_TRUE((hud::is_trivially_move_constructible_v<i32*>));
    ASSERT_TRUE((hud::is_trivially_move_constructible_v<i32, f32>));

    ASSERT_TRUE((hud::is_trivially_move_constructible_v<a>));
    ASSERT_TRUE((hud::is_trivially_move_constructible_v<a, a>));
    ASSERT_FALSE((hud::is_trivially_move_constructible_v<b>));
    ASSERT_FALSE((hud::is_trivially_move_constructible_v<c>));
    ASSERT_FALSE((hud::is_trivially_move_constructible_v<c, c>));
}