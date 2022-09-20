#include <core/traits/is_copy_assignable.h>

namespace {
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

    ASSERT_TRUE((hud::is_copy_assignable_v<a>));
    ASSERT_FALSE((hud::is_copy_assignable_v<const a>));
    ASSERT_FALSE((hud::is_copy_assignable_v<const volatile a>));
    ASSERT_TRUE((hud::is_copy_assignable_v<c>)); // c = c compile
    ASSERT_TRUE((hud::is_copy_assignable_v<d>)); // d = d do not compile no operator =
    ASSERT_TRUE((hud::is_copy_assignable_v<a, b>));
    ASSERT_FALSE((hud::is_copy_assignable_v<a, d>));
}