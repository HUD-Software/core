#include <core/traits/remove_extent.h>

GTEST_TEST(traits, remove_extent) {
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::remove_extent_t<i32>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::remove_extent_t<i32[]>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::remove_extent_t<i32[10]>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::remove_extent_t<i32[][10]>, i32[10]>));
}