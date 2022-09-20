#include <core/traits/negation.h>


TEST(traits, negation) {
    ASSERT_TRUE((hud::negation_v<hud::integral_constant<bool, 0>>));
    ASSERT_FALSE((hud::negation_v<hud::integral_constant<bool, 1>>));
    ASSERT_TRUE((hud::negation_v<hud::integral_constant<bool, false>>));
    ASSERT_FALSE((hud::negation_v<hud::integral_constant<bool, true>>));

    ASSERT_TRUE((hud::negation_v<hud::bool_constant<false>>));
    ASSERT_FALSE((hud::negation_v<hud::bool_constant<true>>));
    ASSERT_TRUE((hud::negation_v<hud::false_type>));
    ASSERT_FALSE((hud::negation_v<hud::true_type>));
}