#include <core/traits/is_volatile.h>

GTEST_TEST(traits, is_volatile) {
    GTEST_ASSERT_FALSE((hud::is_volatile_v<int>));
    GTEST_ASSERT_TRUE((hud::is_volatile_v<volatile int>));
    GTEST_ASSERT_FALSE((hud::is_volatile_v<const int>));
    GTEST_ASSERT_TRUE((hud::is_volatile_v<const volatile int>));
    GTEST_ASSERT_FALSE((hud::is_volatile_v<volatile int*>));
    GTEST_ASSERT_TRUE((hud::is_volatile_v<volatile int* volatile>));
}
