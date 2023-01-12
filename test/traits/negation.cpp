#include <core/traits/negation.h>

GTEST_TEST(traits, negation)
{
    GTEST_ASSERT_TRUE((hud::negation_v<hud::integral_constant<bool, 0>>));
    GTEST_ASSERT_FALSE((hud::negation_v<hud::integral_constant<bool, 1>>));
    GTEST_ASSERT_TRUE((hud::negation_v<hud::integral_constant<bool, false>>));
    GTEST_ASSERT_FALSE((hud::negation_v<hud::integral_constant<bool, true>>));

    GTEST_ASSERT_TRUE((hud::negation_v<hud::bool_constant<false>>));
    GTEST_ASSERT_FALSE((hud::negation_v<hud::bool_constant<true>>));
    GTEST_ASSERT_TRUE((hud::negation_v<hud::false_type>));
    GTEST_ASSERT_FALSE((hud::negation_v<hud::true_type>));
}