#include <core/traits/is_floating_point.h>

GTEST_TEST(traits, is_floating_point) {
    ASSERT_TRUE((hud::is_floating_point_v<f32>));
    ASSERT_TRUE((hud::is_floating_point_v<const f32>));
    ASSERT_TRUE((hud::is_floating_point_v<const volatile f32>));
    ASSERT_TRUE((hud::is_floating_point_v<f64>));
    ASSERT_TRUE((hud::is_floating_point_v<const f64>));
    ASSERT_TRUE((hud::is_floating_point_v<const volatile f64>));
    ASSERT_FALSE((hud::is_floating_point_v<class a>));
}
