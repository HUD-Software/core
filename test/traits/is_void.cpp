#include <core/traits/is_void.h>

namespace hud_test
{
    class a
    {
    };
}

GTEST_TEST(traits, is_void)
{
    GTEST_ASSERT_FALSE((hud::is_void_v<i32>));
    GTEST_ASSERT_FALSE((hud::is_void_v<i32 &>));
    GTEST_ASSERT_FALSE((hud::is_void_v<i32[]>));
    GTEST_ASSERT_FALSE((hud::is_void_v<i32[32]>));
    GTEST_ASSERT_FALSE((hud::is_void_v<i32 *>));

    GTEST_ASSERT_TRUE((hud::is_void_v<void>));
    GTEST_ASSERT_TRUE((hud::is_void_v<const void>));
    GTEST_ASSERT_TRUE((hud::is_void_v<volatile void>));
    GTEST_ASSERT_TRUE((hud::is_void_v<const volatile void>));
    GTEST_ASSERT_TRUE((hud::is_void_v<hud::void_t<>>));
    GTEST_ASSERT_TRUE((hud::is_void_v<const hud::void_t<>>));
    GTEST_ASSERT_TRUE((hud::is_void_v<volatile hud::void_t<>>));
    GTEST_ASSERT_TRUE((hud::is_void_v<const volatile hud::void_t<>>));
    GTEST_ASSERT_TRUE((hud::is_void_v<decltype(void())>));
    GTEST_ASSERT_FALSE((hud::is_void_v<hud_test::a>));
}
