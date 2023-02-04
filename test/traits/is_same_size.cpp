#include <core/traits/is_same_size.h>

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        i32 a;
    };

    struct c
    {
        i64 a;
    };

    struct d
    {
        f32 a;
    };
} // namespace hud_test

GTEST_TEST(traits, is_same_size)
{
    hud_assert_true((hud::is_same_size_v<i32, const i32>));
    hud_assert_true((hud::is_same_size_v<hud_test::a, std::tuple<>>));
    hud_assert_false((hud::is_same_size_v<hud_test::a, hud_test::b>));
    hud_assert_false((hud::is_same_size_v<hud_test::b, hud_test::c>));
    hud_assert_true((hud::is_same_size_v<hud_test::b, hud_test::d>));
    hud_assert_true((hud::is_same_size_v<hud_test::b, i32>));
    hud_assert_true((hud::is_same_size_v<hud_test::b[2], i32[1]>));
}