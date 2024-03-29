#include <core/traits/remove_all_extents.h>

GTEST_TEST(traits, remove_all_extents)
{
    hud_assert_true((hud::is_same_v<hud::remove_all_extents_t<i32>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_all_extents_t<i32[]>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_all_extents_t<i32[][2]>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_all_extents_t<i32[12][21]>, i32>));
}
