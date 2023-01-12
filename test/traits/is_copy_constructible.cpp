#include <core/traits/is_copy_constructible.h>

namespace hud_test
{
    struct a
    {
    };
    struct b
    {
        b(b &&) noexcept {}
    };
    struct c
    {
        c(const c &) noexcept {}
    };
    struct d
    {
        d(const c &) noexcept {}
    };
}

GTEST_TEST(traits, is_copy_constructible)
{
    GTEST_ASSERT_TRUE((hud::is_copy_constructible_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_copy_constructible_v<i32 &>));
    GTEST_ASSERT_TRUE((hud::is_copy_constructible_v<volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_copy_constructible_v<const i32>));
    GTEST_ASSERT_TRUE((hud::is_copy_constructible_v<const volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_copy_constructible_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_copy_constructible_v<hud_test::b>));
    GTEST_ASSERT_TRUE((hud::is_copy_constructible_v<hud_test::c>));
    GTEST_ASSERT_TRUE((hud::is_copy_constructible_v<hud_test::d, hud_test::c>));
    GTEST_ASSERT_TRUE((hud::is_copy_constructible_v<hud_test::d, hud_test::d>));
    GTEST_ASSERT_FALSE((hud::is_copy_constructible_v<hud_test::d, hud_test::b>));
}