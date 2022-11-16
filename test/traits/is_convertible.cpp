#include <core/traits/is_convertible.h>

namespace{
    struct a {};
    struct b {};
    struct c : public a { };
}

GTEST_TEST(traits, is_convertible) {
    ASSERT_TRUE((hud::is_convertible_v<i32, i32>));
    ASSERT_TRUE((hud::is_convertible_v<i32, const i32>));
    ASSERT_TRUE((hud::is_convertible_v<i32, const volatile i32>));
    ASSERT_TRUE((hud::is_convertible_v<i32, f32>));
    ASSERT_TRUE((hud::is_convertible_v<c, a>));
    ASSERT_TRUE((hud::is_convertible_v<c, const a>));
    ASSERT_TRUE((hud::is_convertible_v<c, const volatile a>));
    ASSERT_FALSE((hud::is_convertible_v<a, c>));
    ASSERT_FALSE((hud::is_convertible_v<b, a>));

    ASSERT_TRUE((hud::is_convertible_v<c*, a*>)); // a* ptr = new c is valid
    ASSERT_TRUE((hud::is_convertible_v<a(*)[], const a(*)[]>)); // Array-to-pointer is only convertible if same type with CV ( const a (*)[] = declval(a (*)[]) )
    ASSERT_FALSE((hud::is_convertible_v<const a(*)[], a(*)[]>)); // Here we loose const, this is not convertible
    ASSERT_FALSE((hud::is_convertible_v<a*, c*>)); // c* ptr = new a is not valid
}