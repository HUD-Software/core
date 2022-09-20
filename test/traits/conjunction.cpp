#include <core/traits/conjunction.h>

TEST(traits, conjunction) {
    ASSERT_TRUE(hud::conjunction_v<hud::true_type>);
    ASSERT_FALSE(hud::conjunction_v<hud::false_type>);
    ASSERT_FALSE((hud::conjunction_v<hud::true_type, hud::false_type>));
    ASSERT_FALSE((hud::conjunction_v<hud::false_type, hud::true_type>));
    ASSERT_TRUE((hud::conjunction_v<hud::true_type, hud::true_type, hud::true_type>));
    ASSERT_FALSE((hud::conjunction_v<hud::true_type, hud::true_type, hud::false_type>));
    ASSERT_FALSE((hud::conjunction_v<hud::false_type, hud::false_type>));
}