#include <core/traits/is_trivially_copy_constructible.h>

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        b(const b &)
        {
        }
    };

    struct c
    {
        virtual void fn()
        {
        }
    };

    struct d
    {
        d(a &&)
        {
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_trivially_copy_constructible)
{
    hud_assert_true((hud::is_trivially_copy_constructible_v<i32>));
    hud_assert_true((hud::is_trivially_copy_constructible_v<i32 &>));
    hud_assert_false((hud::is_trivially_copy_constructible_v<i32[]>));
    hud_assert_false((hud::is_trivially_copy_constructible_v<i32[32]>));
    hud_assert_true((hud::is_trivially_copy_constructible_v<i32 *>));
    hud_assert_true((hud::is_trivially_copy_constructible_v<i32, f32>));

    hud_assert_true((hud::is_trivially_copy_constructible_v<hud_test::a>));
    hud_assert_true((hud::is_trivially_copy_constructible_v<hud_test::a, hud_test::a>));
    hud_assert_false((hud::is_trivially_copy_constructible_v<hud_test::b>));
    hud_assert_false((hud::is_trivially_copy_constructible_v<hud_test::c>));
    hud_assert_true((hud::is_trivially_copy_constructible_v<hud_test::d>));
    hud_assert_true((hud::is_trivially_copy_constructible_v<hud_test::d, hud_test::d>));
    hud_assert_false((hud::is_trivially_copy_constructible_v<hud_test::d, hud_test::a>));
}