#include <core/traits/is_floating_point.h>

GTEST_TEST(traits, is_floating_point)
{
    GTEST_ASSERT_TRUE((hud::is_floating_point_v<f32>));
    GTEST_ASSERT_TRUE((hud::is_floating_point_v<const f32>));
    GTEST_ASSERT_TRUE((hud::is_floating_point_v<const volatile f32>));
    GTEST_ASSERT_TRUE((hud::is_floating_point_v<f64>));
    GTEST_ASSERT_TRUE((hud::is_floating_point_v<const f64>));
    GTEST_ASSERT_TRUE((hud::is_floating_point_v<const volatile f64>));
    GTEST_ASSERT_FALSE((hud::is_floating_point_v<class a>));
}
