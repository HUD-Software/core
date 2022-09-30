#include <core/traits/is_trivially_copy_constructible.h>

namespace hud_test {
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

    ASSERT_TRUE((hud::is_trivially_copy_constructible_v<hud_test::a>));
    ASSERT_TRUE((hud::is_trivially_copy_constructible_v<hud_test::a, hud_test::a>));
    ASSERT_FALSE((hud::is_trivially_copy_constructible_v<hud_test::b>));
    ASSERT_FALSE((hud::is_trivially_copy_constructible_v<hud_test::c>));
    ASSERT_TRUE((hud::is_trivially_copy_constructible_v<hud_test::d>));
    ASSERT_TRUE((hud::is_trivially_copy_constructible_v<hud_test::d, hud_test::d>));
    ASSERT_FALSE((hud::is_trivially_copy_constructible_v<hud_test::d, hud_test::a>));
}