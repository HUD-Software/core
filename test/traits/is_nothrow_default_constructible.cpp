#include <core/traits/is_nothrow_default_constructible.h>

namespace {
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

TEST(traits, is_nothrow_default_constructible) {    
    ASSERT_TRUE((hud::is_nothrow_default_constructible_v<a>));
    ASSERT_FALSE((hud::is_nothrow_default_constructible_v<b>));
    ASSERT_FALSE((hud::is_nothrow_default_constructible_v<c>));
    ASSERT_TRUE((hud::is_nothrow_default_constructible_v<d>));
    ASSERT_FALSE((hud::is_nothrow_default_constructible_v<e>));
}