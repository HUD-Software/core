#include <core/traits/is_trivially_assignable.h>

namespace{
    struct a { };
    struct b : a {};
    struct c { c& operator= (const a&) { return *this; } };
    struct d { d& operator= (const d&) { return *this; } };
}

GTEST_TEST(traits, is_trivially_assignable) {
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<i32, i32>));
    GTEST_ASSERT_TRUE((hud::is_trivially_assignable_v<i32&, i32>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<i32[], i32[]>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<i32[32], i32[32]>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<i32*, i32*>));
    GTEST_ASSERT_TRUE((hud::is_trivially_assignable_v<a, a>));
    GTEST_ASSERT_TRUE((hud::is_trivially_assignable_v<a, b>));
    GTEST_ASSERT_TRUE((hud::is_trivially_assignable_v<a, b&>));
    GTEST_ASSERT_TRUE((hud::is_trivially_assignable_v<a, b&&>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<a, c>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<a, d>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<b, a>));
    GTEST_ASSERT_TRUE((hud::is_trivially_assignable_v<b, b>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<b, c>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<b, d>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<c, a>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<c, b>));
    GTEST_ASSERT_TRUE((hud::is_trivially_assignable_v<c, c&>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<c, d>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<d, a>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<d, b>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<d, c>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<d, d&>));
    GTEST_ASSERT_FALSE((hud::is_trivially_assignable_v<d, d&&>));
}