#include <core/traits/is_trivially_move_assignable.h>

namespace {
    struct a { };
    struct b {
        b& operator=(a&&) noexcept { return *this; }
    };
    struct c {
        c& operator=(c&&) noexcept { return *this; }
    };
}

TEST(traits, is_trivially_move_assignable) {
    ASSERT_TRUE((hud::is_trivially_move_assignable_v<i32>));
    ASSERT_TRUE((hud::is_trivially_move_assignable_v<i32&>));
    ASSERT_FALSE((hud::is_trivially_move_assignable_v<i32[]>));
    ASSERT_FALSE((hud::is_trivially_move_assignable_v<i32[32]>));
    ASSERT_TRUE((hud::is_trivially_move_assignable_v<i32*>));
    ASSERT_TRUE((hud::is_trivially_move_assignable_v<i32, f32>));

    ASSERT_TRUE((hud::is_trivially_move_assignable_v<a>));
    ASSERT_TRUE((hud::is_trivially_move_assignable_v<a, a>));
    ASSERT_TRUE((hud::is_trivially_move_assignable_v<b>));
    ASSERT_TRUE((hud::is_trivially_move_assignable_v<b, b>));
    ASSERT_FALSE((hud::is_trivially_move_assignable_v<c>));
}