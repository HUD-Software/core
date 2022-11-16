#include <core/traits/is_void.h>

namespace hud_test {
    class a {};
}

GTEST_TEST(traits, is_void) {
    ASSERT_FALSE((hud::is_void_v<i32>));
    ASSERT_FALSE((hud::is_void_v<i32&>));
    ASSERT_FALSE((hud::is_void_v<i32[]>));
    ASSERT_FALSE((hud::is_void_v<i32[32]>));
    ASSERT_FALSE((hud::is_void_v<i32*>));

    ASSERT_TRUE((hud::is_void_v<void>));
    ASSERT_TRUE((hud::is_void_v<const void>));
    ASSERT_TRUE((hud::is_void_v<volatile void>));
    ASSERT_TRUE((hud::is_void_v<const volatile void>));
    ASSERT_TRUE((hud::is_void_v<hud::void_t<>>));
    ASSERT_TRUE((hud::is_void_v<const hud::void_t<>>));
    ASSERT_TRUE((hud::is_void_v<volatile hud::void_t<>>));
    ASSERT_TRUE((hud::is_void_v<const volatile hud::void_t<>>));
    ASSERT_TRUE((hud::is_void_v<decltype(void())>));
    ASSERT_FALSE((hud::is_void_v<hud_test::a>));
}
