#include <core/traits/is_move_assignable.h>

namespace {
    struct a { };
    struct b { b& operator= (b&&) = delete; };
    struct c { c& operator= (b&&) noexcept { return *this; } };
}

TEST(traits, is_move_assignable) {
    ASSERT_TRUE((hud::is_move_assignable_v<i32>));
    ASSERT_TRUE((hud::is_move_assignable_v<a>));
    ASSERT_FALSE((hud::is_move_assignable_v<b>));
    ASSERT_TRUE((hud::is_move_assignable_v<c, b>));
    ASSERT_FALSE((hud::is_move_assignable_v<b, c>));
}