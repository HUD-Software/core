#include <core/traits/is_assignable.h>

namespace {
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
    ASSERT_TRUE((hud::is_assignable_v<a, a>)); // a() = a() compile
    ASSERT_TRUE((hud::is_assignable_v<a&, a>)); // a a ; a = a() compile
    ASSERT_FALSE((hud::is_assignable_v<const a&, a>)); // const a a ; a = a() do not compile
    ASSERT_TRUE((hud::is_assignable_v<a, c>)); // a() = c() compile
    ASSERT_FALSE((hud::is_assignable_v<a, b>)); // a() = b() do not compile
    ASSERT_TRUE((hud::is_assignable_v<d, b>)); // d() = b() compile
}
