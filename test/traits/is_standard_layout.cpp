#include <core/traits/is_standard_layout.h>

namespace {
    struct a { int m; };
    struct b { int m1; private: [[maybe_unused]] int m2; };
    struct c { virtual void foo() {}; };
    struct d : a { int j; };
    struct E { int m; short a; int b; };
}

TEST(traits, is_standard_layout) {
    ASSERT_TRUE((hud::is_standard_layout_v<a>));
    ASSERT_TRUE((hud::is_standard_layout_v<i32>));
    ASSERT_TRUE((hud::is_standard_layout_v<i32[]>));
    ASSERT_TRUE((hud::is_standard_layout_v<i32[3]>));
    ASSERT_TRUE((hud::is_standard_layout_v<i32*>));
    ASSERT_FALSE((hud::is_standard_layout_v<b>));
    ASSERT_FALSE((hud::is_standard_layout_v<c>));
    ASSERT_FALSE((hud::is_standard_layout_v<d>));
    ASSERT_TRUE((hud::is_standard_layout_v<E>));
}
