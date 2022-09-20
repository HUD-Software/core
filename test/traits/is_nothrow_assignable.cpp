#include <core/traits/is_nothrow_assignable.h>

namespace {
    struct a { };
    struct b {
        b& operator= (const a&) noexcept { return *this; }
        b& operator= (const b&) { return *this; }
        b& operator= (a&&) noexcept { return *this; }
        b& operator= (b&&) { return *this; }
    };
}

TEST(traits, is_nothrow_assignable) {
    ASSERT_TRUE((hud::is_nothrow_assignable_v<a, a>));
    ASSERT_TRUE((hud::is_nothrow_assignable_v<b, a&>));
    ASSERT_FALSE((hud::is_nothrow_assignable_v<b, b&>));
    ASSERT_TRUE((hud::is_nothrow_assignable_v<b, a&&>));
    ASSERT_FALSE((hud::is_nothrow_assignable_v<b, b&&>));
}