#include <core/traits/is_copy_assignable.h>

namespace hud_test
{
    struct b
    {
    };

    struct a
    {
        a &operator=(const b &) noexcept
        {
            return *this;
        }
    };

    struct c : public a
    {
    };

    struct d
    {
        d &operator=(const b &) noexcept
        {
            return *this;
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_copy_assignable)
{

    hud_assert_true((hud::is_copy_assignable_v<i32>));
    hud_assert_false((hud::is_copy_assignable_v<const i32>));
    hud_assert_false((hud::is_copy_assignable_v<const volatile i32>));

    hud_assert_true((hud::is_copy_assignable_v<hud_test::a>));
    hud_assert_false((hud::is_copy_assignable_v<const hud_test::a>));
    hud_assert_false((hud::is_copy_assignable_v<const volatile hud_test::a>));
    hud_assert_true((hud::is_copy_assignable_v<hud_test::c>)); // c = c compile
    hud_assert_true((hud::is_copy_assignable_v<hud_test::d>)); // d = d do not compile no operator =
    hud_assert_true((hud::is_copy_assignable_v<hud_test::a, hud_test::b>));
    hud_assert_false((hud::is_copy_assignable_v<hud_test::a, hud_test::d>));
}