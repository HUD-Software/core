#include <core/traits/is_trivially_move_assignable.h>

namespace hud_test {
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

    ASSERT_TRUE((hud::is_trivially_move_assignable_v<hud_test::a>));
    ASSERT_TRUE((hud::is_trivially_move_assignable_v<hud_test::a, hud_test::a>));
    ASSERT_TRUE((hud::is_trivially_move_assignable_v<hud_test::b>));
    ASSERT_TRUE((hud::is_trivially_move_assignable_v<hud_test::b, hud_test::b>));
    ASSERT_FALSE((hud::is_trivially_move_assignable_v<hud_test::c>));
}