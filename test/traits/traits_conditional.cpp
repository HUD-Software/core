#include <core/traits/conditional.h>

GTEST_TEST(traits, conditional)
{
    hud_assert_true((hud::conditional_t<true, hud::true_type, hud::false_type>::value));
    hud_assert_false((hud::conditional_t<true, hud::false_type, hud::true_type>::value));
    hud_assert_false((hud::conditional_t<false, hud::true_type, hud::false_type>::value));
    hud_assert_true((hud::conditional_t<false, hud::false_type, hud::true_type>::value));
}