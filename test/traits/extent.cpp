#include <core/traits/extent.h>


GTEST_TEST(traits, extent) {
    GTEST_ASSERT_EQ(hud::extent_v<i32>, 0u);
    GTEST_ASSERT_EQ((hud::extent_v<i32, 1>), 0u);

    GTEST_ASSERT_EQ(hud::extent_v<i32[]>, 0u);
    GTEST_ASSERT_EQ((hud::extent_v<i32[], 1>), 0u);

    GTEST_ASSERT_EQ(hud::extent_v<i32[][8]>, 0u);
    GTEST_ASSERT_EQ((hud::extent_v<i32[][8], 1>), 8u);
    GTEST_ASSERT_EQ((hud::extent_v<i32[][8], 2>), 0u);

    GTEST_ASSERT_EQ(hud::extent_v<i32[][8][16]>, 0u);
    GTEST_ASSERT_EQ((hud::extent_v<i32[][8][16], 1>), 8u);
    GTEST_ASSERT_EQ((hud::extent_v<i32[][8][16], 2>), 16u);
    GTEST_ASSERT_EQ((hud::extent_v<i32[][8][16], 3>), 0u);
}