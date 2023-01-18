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
    GTEST_ASSERT_TRUE((hud::is_same_size_v<i32, const i32>));
    GTEST_ASSERT_TRUE((hud::is_same_size_v<hud_test::a, std::tuple<>>));
    GTEST_ASSERT_FALSE((hud::is_same_size_v<hud_test::a, hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_same_size_v<hud_test::b, hud_test::c>));
    GTEST_ASSERT_TRUE((hud::is_same_size_v<hud_test::b, hud_test::d>));
    GTEST_ASSERT_TRUE((hud::is_same_size_v<hud_test::b, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_size_v<hud_test::b[2], i32[1]>));
}