#include <core/traits/is_convertible.h>

namespace
{
    struct a
    {
    };

    struct b
    {
    };

    struct c : public a
    {
    };
} // namespace

GTEST_TEST(traits, is_convertible)
{
    hud_assert_true((hud::is_convertible_v<i32, i32>));
    hud_assert_true((hud::is_convertible_v<i32, const i32>));
    hud_assert_true((hud::is_convertible_v<i32, const volatile i32>));
    hud_assert_true((hud::is_convertible_v<i32, f32>));
    hud_assert_true((hud::is_convertible_v<c, a>));
    hud_assert_true((hud::is_convertible_v<c, const a>));
    hud_assert_true((hud::is_convertible_v<c, const volatile a>));
    hud_assert_false((hud::is_convertible_v<a, c>));
    hud_assert_false((hud::is_convertible_v<b, a>));

    hud_assert_true((hud::is_convertible_v<c *, a *>));              // a* ptr = new c is valid
    hud_assert_true((hud::is_convertible_v<a(*)[], const a(*)[]>));  // Array-to-pointer is only convertible if same type with CV ( const a (*)[] = declval(a (*)[]) )
    hud_assert_false((hud::is_convertible_v<const a(*)[], a(*)[]>)); // Here we loose const, this is not convertible
    hud_assert_false((hud::is_convertible_v<a *, c *>));             // c* ptr = new a is not valid
}