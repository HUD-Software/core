#include <core/traits/conjunction.h>

GTEST_TEST(traits, conjunction)
{
    GTEST_ASSERT_TRUE(hud::conjunction_v<hud::true_type>);
    GTEST_ASSERT_FALSE(hud::conjunction_v<hud::false_type>);
    GTEST_ASSERT_FALSE((hud::conjunction_v<hud::true_type, hud::false_type>));
    GTEST_ASSERT_FALSE((hud::conjunction_v<hud::false_type, hud::true_type>));
    GTEST_ASSERT_TRUE((hud::conjunction_v<hud::true_type, hud::true_type, hud::true_type>));
    GTEST_ASSERT_FALSE((hud::conjunction_v<hud::true_type, hud::true_type, hud::false_type>));
    GTEST_ASSERT_FALSE((hud::conjunction_v<hud::false_type, hud::false_type>));
}