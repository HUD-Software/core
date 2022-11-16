#include <core/traits/is_nothrow_default_constructible.h>

namespace hud_test {
    struct a {
        a() noexcept {}
    };
    struct b {
        b() noexcept(false) {}
    };

    struct c {
        c() { throw 20; }
    };
    struct d {
        d() noexcept : c() {}
        c c;
    };
    struct e {
        e() : c() {}
        c c;
    };
}

GTEST_TEST(traits, is_nothrow_default_constructible) {    
    ASSERT_TRUE((hud::is_nothrow_default_constructible_v<hud_test::a>));
    ASSERT_FALSE((hud::is_nothrow_default_constructible_v<hud_test::b>));
    ASSERT_FALSE((hud::is_nothrow_default_constructible_v<hud_test::c>));
    ASSERT_TRUE((hud::is_nothrow_default_constructible_v<hud_test::d>));
    ASSERT_FALSE((hud::is_nothrow_default_constructible_v<hud_test::e>));
}