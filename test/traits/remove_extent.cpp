#include <core/traits/remove_extent.h>

TEST(traits, remove_extent) {
    ASSERT_TRUE((hud::is_same_v<hud::remove_extent_t<i32>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_extent_t<i32[]>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_extent_t<i32[10]>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_extent_t<i32[][10]>, i32[10]>));
}