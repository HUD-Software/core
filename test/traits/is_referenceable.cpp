#include <core/traits/is_referenceable.h>

GTEST_TEST(traits, is_referenceable)
{
    hud_assert_true((hud::is_referenceable_v<i32>));
    hud_assert_true((hud::is_referenceable_v<i32 &>));
    hud_assert_true((hud::is_referenceable_v<i32 &&>));
    hud_assert_true((hud::is_referenceable_v<i32 *>));
    hud_assert_true((hud::is_referenceable_v<i32 (*)(i32)>));
    hud_assert_false((hud::is_referenceable_v<int() const>));
}