#include <core/traits/conjunction.h>

GTEST_TEST(traits, conjunction)
{
    hud_assert_true(hud::conjunction_v<hud::true_type>);
    hud_assert_false(hud::conjunction_v<hud::false_type>);
    hud_assert_false((hud::conjunction_v<hud::true_type, hud::false_type>));
    hud_assert_false((hud::conjunction_v<hud::false_type, hud::true_type>));
    hud_assert_true((hud::conjunction_v<hud::true_type, hud::true_type, hud::true_type>));
    hud_assert_false((hud::conjunction_v<hud::true_type, hud::true_type, hud::false_type>));
    hud_assert_false((hud::conjunction_v<hud::false_type, hud::false_type>));
}