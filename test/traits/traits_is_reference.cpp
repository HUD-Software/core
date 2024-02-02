#include <core/traits/is_reference.h>

GTEST_TEST(traits, is_reference)
{
    hud_assert_false((hud::is_reference_v<i32>));
    hud_assert_false((hud::is_reference_v<i32 *>));
    hud_assert_true((hud::is_reference_v<i32 &>));
    hud_assert_true((hud::is_reference_v<i32 &&>));
}