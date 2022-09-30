#include <core/traits/is_copy_assignable.h>

namespace hud_test {
    struct b {};
    struct a {
        a& operator=(const b&) noexcept { return *this; }
    };
    struct c : public a {};
    struct d {
        d& operator=(const b&) noexcept { return *this; }
    };
}

TEST(traits, is_copy_assignable) {  

    ASSERT_TRUE((hud::is_copy_assignable_v<i32>));
    ASSERT_FALSE((hud::is_copy_assignable_v<const i32>));
    ASSERT_FALSE((hud::is_copy_assignable_v<const volatile i32>));

    ASSERT_TRUE((hud::is_copy_assignable_v<hud_test::a>));
    ASSERT_FALSE((hud::is_copy_assignable_v<const hud_test::a>));
    ASSERT_FALSE((hud::is_copy_assignable_v<const volatile hud_test::a>));
    ASSERT_TRUE((hud::is_copy_assignable_v<hud_test::c>)); // c = c compile
    ASSERT_TRUE((hud::is_copy_assignable_v<hud_test::d>)); // d = d do not compile no operator =
    ASSERT_TRUE((hud::is_copy_assignable_v<hud_test::a, hud_test::b>));
    ASSERT_FALSE((hud::is_copy_assignable_v<hud_test::a, hud_test::d>));
}