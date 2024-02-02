#include <core/traits/decay.h>

GTEST_TEST(traits, decay)
{
    hud_assert_true((hud::is_same_v<hud::decay_t<i32>, i32>));
    hud_assert_true((hud::is_same_v<hud::decay_t<i32 &>, i32>));
    hud_assert_true((hud::is_same_v<hud::decay_t<i32 &&>, i32>));
    hud_assert_true((hud::is_same_v<hud::decay_t<const i32 &>, i32>));
    hud_assert_false((hud::is_same_v<hud::decay_t<i32[]>, i32>));
    hud_assert_true((hud::is_same_v<hud::decay_t<i32[]>, i32 *>));
    hud_assert_false((hud::is_same_v<hud::decay_t<i32[2]>, i32>));
    hud_assert_true((hud::is_same_v<hud::decay_t<i32[2]>, i32 *>));
    hud_assert_false((hud::is_same_v<hud::decay_t<i32 *>, i32>));
    hud_assert_true((hud::is_same_v<hud::decay_t<i32 *>, i32 *>));
    hud_assert_false((hud::is_same_v<hud::decay_t<i32(i32)>, i32>));
    hud_assert_true((hud::is_same_v<hud::decay_t<i32(i32)>, i32 (*)(i32)>));
}
