#include <core/traits/is_rvalue_reference.h>

GTEST_TEST(traits, is_rvalue_reference)
{
    hud_assert_false((hud::is_rvalue_reference_v<i32>));
    hud_assert_false((hud::is_rvalue_reference_v<i32 &>));
    hud_assert_true((hud::is_rvalue_reference_v<i32 &&>));
}