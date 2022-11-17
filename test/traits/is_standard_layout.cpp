#include <core/traits/is_standard_layout.h>

namespace hud_test {
    struct a { int m; };
    struct b { int m1; private: [[maybe_unused]] int m2; };
    struct c { virtual void foo() {}; };
    struct d : a { int j; };
    struct E { int m; short a; int b; };
}

GTEST_TEST(traits, is_standard_layout) {
    GTEST_ASSERT_TRUE((hud::is_standard_layout_v<hud_test::a>));
    GTEST_ASSERT_TRUE((hud::is_standard_layout_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_standard_layout_v<i32[]>));
    GTEST_ASSERT_TRUE((hud::is_standard_layout_v<i32[3]>));
    GTEST_ASSERT_TRUE((hud::is_standard_layout_v<i32*>));
    GTEST_ASSERT_FALSE((hud::is_standard_layout_v<hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_standard_layout_v<hud_test::c>));
    GTEST_ASSERT_FALSE((hud::is_standard_layout_v<hud_test::d>));
    GTEST_ASSERT_TRUE((hud::is_standard_layout_v<hud_test::E>));
}
