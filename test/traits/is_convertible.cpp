#include <core/traits/is_convertible.h>

namespace{
    struct a {};
    struct b {};
    struct c : public a { };
}

GTEST_TEST(traits, is_convertible) {
    GTEST_ASSERT_TRUE((hud::is_convertible_v<i32, i32>));
    GTEST_ASSERT_TRUE((hud::is_convertible_v<i32, const i32>));
    GTEST_ASSERT_TRUE((hud::is_convertible_v<i32, const volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_convertible_v<i32, f32>));
    GTEST_ASSERT_TRUE((hud::is_convertible_v<c, a>));
    GTEST_ASSERT_TRUE((hud::is_convertible_v<c, const a>));
    GTEST_ASSERT_TRUE((hud::is_convertible_v<c, const volatile a>));
    GTEST_ASSERT_FALSE((hud::is_convertible_v<a, c>));
    GTEST_ASSERT_FALSE((hud::is_convertible_v<b, a>));

    GTEST_ASSERT_TRUE((hud::is_convertible_v<c*, a*>)); // a* ptr = new c is valid
    GTEST_ASSERT_TRUE((hud::is_convertible_v<a(*)[], const a(*)[]>)); // Array-to-pointer is only convertible if same type with CV ( const a (*)[] = declval(a (*)[]) )
    GTEST_ASSERT_FALSE((hud::is_convertible_v<const a(*)[], a(*)[]>)); // Here we loose const, this is not convertible
    GTEST_ASSERT_FALSE((hud::is_convertible_v<a*, c*>)); // c* ptr = new a is not valid
}