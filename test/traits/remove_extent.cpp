#include <core/traits/remove_extent.h>

GTEST_TEST(traits, remove_extent)
{
    hud_assert_true((hud::is_same_v<hud::remove_extent_t<i32>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_extent_t<i32[]>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_extent_t<i32[10]>, i32>));
    hud_assert_true((hud::is_same_v<hud::remove_extent_t<i32[][10]>, i32[10]>));
}