#include <core/traits/is_move_assignable.h>

namespace hud_test {
    struct a { };
    struct b { b& operator= (b&&) = delete; };
    struct c { c& operator= (b&&) noexcept { return *this; } };
}

GTEST_TEST(traits, is_move_assignable) {
    GTEST_ASSERT_TRUE((hud::is_move_assignable_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_move_assignable_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_move_assignable_v<hud_test::b>));
    GTEST_ASSERT_TRUE((hud::is_move_assignable_v<hud_test::c, hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_move_assignable_v<hud_test::b, hud_test::c>));
}