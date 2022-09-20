#include <core/traits/disjunction.h>

TEST(traits, disjunction) {
    ASSERT_TRUE((hud::disjunction_v<hud::true_type, hud::true_type>));
    ASSERT_TRUE((hud::disjunction_v<hud::true_type, hud::false_type>));
    ASSERT_TRUE((hud::disjunction_v<hud::false_type, hud::true_type>));
    ASSERT_FALSE((hud::disjunction_v<hud::false_type, hud::false_type>));
}