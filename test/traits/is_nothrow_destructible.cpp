#include <core/traits/is_nothrow_destructible.h>

namespace {
    struct a { };
    struct b { ~b() {} };
    struct c { ~c() noexcept(false) {} };
}

TEST(traits, is_nothrow_destructible) {
    ASSERT_TRUE((hud::is_nothrow_destructible_v<i32>));
    ASSERT_TRUE((hud::is_nothrow_destructible_v<a>));
    ASSERT_TRUE((hud::is_nothrow_destructible_v<b>));
    ASSERT_FALSE((hud::is_nothrow_destructible_v<c>));
}