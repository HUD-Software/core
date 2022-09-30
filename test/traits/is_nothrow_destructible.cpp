#include <core/traits/is_nothrow_destructible.h>

namespace hud_test {
    struct a { };
    struct b { ~b() {} };
    struct c { ~c() noexcept(false) {} };
}

TEST(traits, is_nothrow_destructible) {
    ASSERT_TRUE((hud::is_nothrow_destructible_v<i32>));
    ASSERT_TRUE((hud::is_nothrow_destructible_v<hud_test::a>));
    ASSERT_TRUE((hud::is_nothrow_destructible_v<hud_test::b>));
    ASSERT_FALSE((hud::is_nothrow_destructible_v<hud_test::c>));
}