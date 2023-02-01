#include <core/traits/extent.h>

GTEST_TEST(traits, extent)
{
    hud_assert_eq(hud::extent_v<i32>, 0u);
    hud_assert_eq((hud::extent_v<i32, 1>), 0u);

    hud_assert_eq(hud::extent_v<i32[]>, 0u);
    hud_assert_eq((hud::extent_v<i32[], 1>), 0u);

    hud_assert_eq(hud::extent_v<i32[][8]>, 0u);
    hud_assert_eq((hud::extent_v<i32[][8], 1>), 8u);
    hud_assert_eq((hud::extent_v<i32[][8], 2>), 0u);

    hud_assert_eq(hud::extent_v<i32[][8][16]>, 0u);
    hud_assert_eq((hud::extent_v<i32[][8][16], 1>), 8u);
    hud_assert_eq((hud::extent_v<i32[][8][16], 2>), 16u);
    hud_assert_eq((hud::extent_v<i32[][8][16], 3>), 0u);
}