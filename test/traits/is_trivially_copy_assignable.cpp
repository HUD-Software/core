#include <core/traits/is_trivially_copy_assignable.h>

namespace hud_test {
    struct a { };
    struct b : a {};
    struct c { c& operator= (const a&) { return *this; } };
    struct d {
        d& operator= (const d&) { return *this; }
        d& operator= (const a&) { return *this; }
    };
}

GTEST_TEST(traits, is_trivially_copy_assignable) {
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<i32>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<i32&>));
    ASSERT_FALSE((hud::is_trivially_copy_assignable_v<i32[]>));
    ASSERT_FALSE((hud::is_trivially_copy_assignable_v<i32[32]>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<i32*>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<i32, f32>));

    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<hud_test::a>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<hud_test::a, hud_test::a>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<hud_test::b>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<hud_test::b, hud_test::b>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<hud_test::c>));
    ASSERT_TRUE((hud::is_trivially_copy_assignable_v<hud_test::c, hud_test::c>));
    ASSERT_FALSE((hud::is_trivially_copy_assignable_v<hud_test::d>));
    ASSERT_FALSE((hud::is_trivially_copy_assignable_v<hud_test::d, hud_test::a>));
}
