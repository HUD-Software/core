#include <core/traits/is_trivially_copyable.h>

namespace {
    struct a { int i; };
    struct b {
        int i, j;
        b(const b& x) : i(x.i), j(x.j) {};
    };
    struct c {
        int i, j;
        c(const a& x) : i(x.i), j(1) {};
    };
}

TEST(traits, is_trivially_copyable) {
    ASSERT_TRUE((hud::is_trivially_copyable_v<i32>));
    ASSERT_FALSE((hud::is_trivially_copyable_v<i32&>));
    ASSERT_TRUE((hud::is_trivially_copyable_v<i32[]>));
    ASSERT_TRUE((hud::is_trivially_copyable_v<i32[32]>));
    ASSERT_TRUE((hud::is_trivially_copyable_v<i32*>));

    ASSERT_TRUE((hud::is_trivially_copyable_v<a>));
    ASSERT_FALSE((hud::is_trivially_copyable_v<b>));
    ASSERT_TRUE((hud::is_trivially_copyable_v<c>));
}