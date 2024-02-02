#include <core/traits/is_unbounded_array.h>

GTEST_TEST(traits, is_unbounded_array)
{
    hud_assert_false((hud::is_unbounded_array_v<i32>));
    hud_assert_false((hud::is_unbounded_array_v<i32 &>));
    hud_assert_true((hud::is_unbounded_array_v<i32[]>));
    hud_assert_false((hud::is_unbounded_array_v<i32[32]>));
    hud_assert_false((hud::is_unbounded_array_v<i32 *>));
}
