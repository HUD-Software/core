#include <core/traits/disjunction.h>

GTEST_TEST(traits, disjunction)
{
    hud_assert_true((hud::disjunction_v<hud::true_type, hud::true_type>));
    hud_assert_true((hud::disjunction_v<hud::true_type, hud::false_type>));
    hud_assert_true((hud::disjunction_v<hud::false_type, hud::true_type>));
    hud_assert_false((hud::disjunction_v<hud::false_type, hud::false_type>));
}