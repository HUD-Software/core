#include <core/traits/is_pointer.h>

GTEST_TEST(traits, is_pointer)
{
    hud_assert_false((hud::is_pointer_v<i32>));
    hud_assert_true((hud::is_pointer_v<i32 *>));
    hud_assert_true((hud::is_pointer_v<i32 **>));
    hud_assert_true((hud::is_pointer_v<i32 (*)(i32)>));
}