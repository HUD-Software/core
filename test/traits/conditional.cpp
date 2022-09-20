#include <core/traits/conditional.h>

TEST(traits, conditional) {
    ASSERT_TRUE((hud::conditional_t<true, hud::true_type, hud::false_type>::value));
    ASSERT_FALSE((hud::conditional_t<true, hud::false_type, hud::true_type>::value));
    ASSERT_FALSE((hud::conditional_t<false, hud::true_type, hud::false_type>::value));
    ASSERT_TRUE((hud::conditional_t<false, hud::false_type, hud::true_type>::value));
}