#include <core/traits/decay.h>

GTEST_TEST(traits, decay) {
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32&>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32&&>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::decay_t<const i32&>, i32>));
    GTEST_ASSERT_FALSE((hud::is_same_v<hud::decay_t<i32[]>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32[]>, i32*>));
    GTEST_ASSERT_FALSE((hud::is_same_v<hud::decay_t<i32[2]>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32[2]>, i32*>));
    GTEST_ASSERT_FALSE((hud::is_same_v<hud::decay_t<i32*>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32*>, i32*>));
    GTEST_ASSERT_FALSE((hud::is_same_v<hud::decay_t<i32(i32)>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::decay_t<i32(i32)>, i32(*)(i32)>));
}

