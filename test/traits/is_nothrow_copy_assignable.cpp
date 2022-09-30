#include <core/traits/is_nothrow_copy_assignable.h>

namespace hud_test {
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
    ASSERT_TRUE((hud::is_nothrow_copy_assignable_v<hud_test::a>));
    ASSERT_FALSE((hud::is_nothrow_copy_assignable_v<hud_test::b>));
    ASSERT_TRUE((hud::is_nothrow_copy_assignable_v<hud_test::c>));
    ASSERT_TRUE((hud::is_nothrow_copy_assignable_v<hud_test::c, hud_test::b>));
    ASSERT_FALSE((hud::is_nothrow_copy_assignable_v<hud_test::c, hud_test::a>));
    ASSERT_FALSE((hud::is_nothrow_copy_assignable_v<hud_test::b, hud_test::c>));

}