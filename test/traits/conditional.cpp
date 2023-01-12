#include <core/traits/conditional.h>

GTEST_TEST(traits, conditional)
{
    GTEST_ASSERT_TRUE((hud::conditional_t<true, hud::true_type, hud::false_type>::value));
    GTEST_ASSERT_FALSE((hud::conditional_t<true, hud::false_type, hud::true_type>::value));
    GTEST_ASSERT_FALSE((hud::conditional_t<false, hud::true_type, hud::false_type>::value));
    GTEST_ASSERT_TRUE((hud::conditional_t<false, hud::false_type, hud::true_type>::value));
}