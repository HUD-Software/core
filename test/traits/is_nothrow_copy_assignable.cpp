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

GTEST_TEST(traits, is_nothrow_copy_assignable) {   
    GTEST_ASSERT_TRUE((hud::is_nothrow_copy_assignable_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_copy_assignable_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_copy_assignable_v<hud_test::b>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_copy_assignable_v<hud_test::c>));
    GTEST_ASSERT_TRUE((hud::is_nothrow_copy_assignable_v<hud_test::c, hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_copy_assignable_v<hud_test::c, hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_nothrow_copy_assignable_v<hud_test::b, hud_test::c>));

}