#include <core/traits/negation.h>

GTEST_TEST(traits, negation)
{
    hud_assert_true((hud::negation_v<hud::integral_constant<bool, 0>>));
    hud_assert_false((hud::negation_v<hud::integral_constant<bool, 1>>));
    hud_assert_true((hud::negation_v<hud::integral_constant<bool, false>>));
    hud_assert_false((hud::negation_v<hud::integral_constant<bool, true>>));

    hud_assert_true((hud::negation_v<hud::bool_constant<false>>));
    hud_assert_false((hud::negation_v<hud::bool_constant<true>>));
    hud_assert_true((hud::negation_v<hud::false_type>));
    hud_assert_false((hud::negation_v<hud::true_type>));
}