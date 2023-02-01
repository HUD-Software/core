#include <core/traits/is_floating_point.h>

GTEST_TEST(traits, is_floating_point)
{
    hud_assert_true((hud::is_floating_point_v<f32>));
    hud_assert_true((hud::is_floating_point_v<const f32>));
    hud_assert_true((hud::is_floating_point_v<const volatile f32>));
    hud_assert_true((hud::is_floating_point_v<f64>));
    hud_assert_true((hud::is_floating_point_v<const f64>));
    hud_assert_true((hud::is_floating_point_v<const volatile f64>));
    hud_assert_false((hud::is_floating_point_v<class a>));
}
