#include <core/traits/is_assignable.h>

namespace hud_test
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

    struct d
    {
        d &operator=(const b &)
        {
            return *this;
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_assignable)
{

    hud_assert_false((hud::is_assignable_v<i32, i32>)); // 1 = 1 do not compile
    hud_assert_true((hud::is_assignable_v<i32 &, i32>));
    hud_assert_true((hud::is_assignable_v<i32 &, const i32>));
    hud_assert_true((hud::is_assignable_v<i32 &, const volatile i32>));
    hud_assert_false((hud::is_assignable_v<const i32 &, i32>));
    hud_assert_false((hud::is_assignable_v<const volatile i32 &, const i32>));
    hud_assert_false((hud::is_assignable_v<const i32 &, const i32>));
    hud_assert_false((hud::is_assignable_v<const volatile i32 &, const volatile i32>));
    hud_assert_true((hud::is_assignable_v<hud_test::a, hud_test::a>));          // hud_test::a() = hud_test::a() compile
    hud_assert_true((hud::is_assignable_v<hud_test::a &, hud_test::a>));        // hud_test::a hud_test::a ; hud_test::a = hud_test::a() compile
    hud_assert_false((hud::is_assignable_v<const hud_test::a &, hud_test::a>)); // const hud_test::a hud_test::a ; hud_test::a = hud_test::a() do not compile
    hud_assert_true((hud::is_assignable_v<hud_test::a, hud_test::c>));          // hud_test::a() = hud_test::c() compile
    hud_assert_false((hud::is_assignable_v<hud_test::a, hud_test::b>));         // hud_test::a() = hud_test::b() do not compile
    hud_assert_true((hud::is_assignable_v<hud_test::d, hud_test::b>));          // hud_test::d() = hud_test::b() compile
}
