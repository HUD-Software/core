#include <core/traits/is_assignable.h>

namespace hud_test {
    struct a {};
    struct b {};
    struct c : public a {};
    struct d {
        d& operator=(const b&) { return *this; }
    };
}

TEST(traits, is_assignable) {
    
    ASSERT_FALSE((hud::is_assignable_v<i32, i32>)); // 1 = 1 do not compile
    ASSERT_TRUE((hud::is_assignable_v<i32&, i32>));
    ASSERT_TRUE((hud::is_assignable_v<i32&, const i32>));
    ASSERT_TRUE((hud::is_assignable_v<i32&, const volatile i32>));
    ASSERT_FALSE((hud::is_assignable_v<const i32&, i32>));
    ASSERT_FALSE((hud::is_assignable_v<const volatile i32&, const i32>));
    ASSERT_FALSE((hud::is_assignable_v<const i32&, const i32>));
    ASSERT_FALSE((hud::is_assignable_v<const volatile i32&, const volatile i32>));
    ASSERT_TRUE((hud::is_assignable_v<hud_test::a, hud_test::a>)); // hud_test::a() = hud_test::a() compile
    ASSERT_TRUE((hud::is_assignable_v<hud_test::a&, hud_test::a>)); // hud_test::a hud_test::a ; hud_test::a = hud_test::a() compile
    ASSERT_FALSE((hud::is_assignable_v<const hud_test::a&, hud_test::a>)); // const hud_test::a hud_test::a ; hud_test::a = hud_test::a() do not compile
    ASSERT_TRUE((hud::is_assignable_v<hud_test::a, hud_test::c>)); // hud_test::a() = hud_test::c() compile
    ASSERT_FALSE((hud::is_assignable_v<hud_test::a, hud_test::b>)); // hud_test::a() = hud_test::b() do not compile
    ASSERT_TRUE((hud::is_assignable_v<hud_test::d, hud_test::b>)); // hud_test::d() = hud_test::b() compile
}
