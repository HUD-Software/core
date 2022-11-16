#include <core/traits/is_bool.h>

GTEST_TEST(traits, is_bool) {
    ASSERT_TRUE(hud::is_bool_v<bool>);
    ASSERT_TRUE(hud::is_bool_v<const bool>);
    ASSERT_TRUE(hud::is_bool_v<volatile bool>);
    ASSERT_TRUE(hud::is_bool_v<const volatile bool>);
}