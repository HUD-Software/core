#include <core/traits/is_nothrow_move_assignable.h>

namespace{
    struct a { };
    struct b {
        b& operator=(b&&) { return *this; }
    };
    struct c {
        c& operator=(c&&) noexcept { return *this; }
        c& operator=(a&&) noexcept { return *this; }
        c& operator=(b&&) { return *this; }
    };
}

GTEST_TEST(traits, is_nothrow_move_assignable) {   
    ASSERT_TRUE((hud::is_nothrow_move_assignable_v<i32>));
    ASSERT_TRUE((hud::is_nothrow_move_assignable_v<a>));
    ASSERT_FALSE((hud::is_nothrow_move_assignable_v<b>));
    ASSERT_TRUE((hud::is_nothrow_move_assignable_v<c>));
    ASSERT_TRUE((hud::is_nothrow_move_assignable_v<c, a>));
    ASSERT_FALSE((hud::is_nothrow_move_assignable_v<c, b>));
}
