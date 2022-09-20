#include <core/traits/is_nothrow_copy_assignable.h>

namespace {
    struct a { };
    struct b {
        b& operator= (const b&) { return *this; }
    };
    struct c {
        c& operator= (const c&) noexcept { return *this; }
        c& operator= (const b&) noexcept { return *this; }
        c& operator= (const a&) { return *this; }
    };
}

TEST(traits, is_nothrow_copy_assignable) {   
    ASSERT_TRUE((hud::is_nothrow_copy_assignable_v<i32>));
    ASSERT_TRUE((hud::is_nothrow_copy_assignable_v<a>));
    ASSERT_FALSE((hud::is_nothrow_copy_assignable_v<b>));
    ASSERT_TRUE((hud::is_nothrow_copy_assignable_v<c>));
    ASSERT_TRUE((hud::is_nothrow_copy_assignable_v<c, b>));
    ASSERT_FALSE((hud::is_nothrow_copy_assignable_v<c, a>));
    ASSERT_FALSE((hud::is_nothrow_copy_assignable_v<b, c>));

}