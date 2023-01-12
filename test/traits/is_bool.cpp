#include <core/traits/is_bool.h>

GTEST_TEST(traits, is_bool)
{
    GTEST_ASSERT_TRUE(hud::is_bool_v<bool>);
    GTEST_ASSERT_TRUE(hud::is_bool_v<const bool>);
    GTEST_ASSERT_TRUE(hud::is_bool_v<volatile bool>);
    GTEST_ASSERT_TRUE(hud::is_bool_v<const volatile bool>);
}